/*
 * LinFrameTransfer.cpp
 *
 *  Created on: Apr 7, 2025
 *      Author: Mediatek
 */


// Core/Src/LinFrameTransfer.cpp
#include "LinFrameTransfer.hpp"
#include "stm32l4xx_hal.h"
#include <optional>
#include <vector>
#include <numeric>

enum class debugLevel
{
    none = 0,
    error,
    verbose
};

constexpr debugLevel debug = debugLevel::verbose;
constexpr uint32_t timeout_ReadFrame = 50; // ms

class FrameReader
{
public:
    enum class State
    {
        WaitForBreak,
        WaitForSync,
        WaitForPID,
        WaitForData,
        WaitForChkSum,
        FrameComplete
    };

    using ChecksumFunction = uint8_t (*)(const uint8_t protectedID, const std::vector<uint8_t> &data);

private:
    State state;
    uint8_t protectedID;
    uint8_t len;
    std::vector<uint8_t> rxData;
    ChecksumFunction getChecksum;

public:
    FrameReader(
        const uint8_t PID,
        uint8_t expectedDataLength,
        ChecksumFunction checksumFunc) :
        state(State::WaitForBreak),  // Match declaration order
        protectedID(PID),
        len(expectedDataLength),
        getChecksum(checksumFunc)
    {
        rxData.reserve(expectedDataLength);
    }

    void reset()
    {
        state = State::WaitForBreak;
        rxData.clear();
    }

    inline bool hasHead()
    {
        return state >= State::WaitForData;
    }

    inline bool isFinish()
    {
        return state == State::FrameComplete;
    }

    std::vector<uint8_t> getData()
    {
        return rxData;
    }

    void processByte(const uint8_t newByte)
    {
        switch (state)
        {
        case State::WaitForBreak:
            if (newByte == LinFrameTransfer::BREAK_FIELD)
                state = State::WaitForSync;
            break;

        case State::WaitForSync:
            if (newByte == LinFrameTransfer::SYNC_FIELD)
                state = State::WaitForPID;
            else
                reset();
            break;

        case State::WaitForPID:
            if (newByte == protectedID)
                state = State::WaitForData;
            else
                reset();
            break;

        case State::WaitForData:
            rxData.push_back(newByte);
            if (rxData.size() >= len)
                state = State::WaitForChkSum;
            break;

        case State::WaitForChkSum:
        {
            if (!getChecksum)
            {
                reset();
                break;  // Exit case safely
            }
            uint8_t expectedChecksum = getChecksum(protectedID, rxData);
            if (newByte == expectedChecksum)
                state = State::FrameComplete;
            else
                reset();
            break;
        }
        case State::FrameComplete:
            break;  // Explicitly handle FrameComplete
        }
    }
};

bool LinFrameTransfer::writeFrame(const uint8_t frameID, const std::vector<uint8_t> &data)
{
    if (data.size() == 0)
        return writeEmptyFrame(frameID);

    const uint8_t protectedID{getProtectedID(frameID)};

    writeFrameHead(protectedID);
    HAL_UART_Transmit(huart, (uint8_t*)data.data(), data.size(), 100);
    uint8_t chksum = getChecksumLin2x(protectedID, data);
    HAL_UART_Transmit(huart, &chksum, 1, 100);

    if constexpr (writeReadback_verify)
    {
        auto result = receiveFrameExtractData(protectedID, data.size());
        if (!result || result.value() != data)
            return false;
    }
    return true;
}

bool LinFrameTransfer::writeEmptyFrame(const uint8_t frameID)
{
    const uint8_t protectedID{getProtectedID(frameID)};
    writeFrameHead(protectedID);

    if constexpr (writeReadback_verify)
    {
        return receiveFrameHead(protectedID);
    }
    return true;
}

std::optional<std::vector<uint8_t>> LinFrameTransfer::readFrame(const uint8_t frameID, uint8_t expectedDataLength)
{
    const uint8_t protectedID{getProtectedID(frameID)};
    writeFrameHead(protectedID);
    return receiveFrameExtractData(protectedID, expectedDataLength);
}

void LinFrameTransfer::writeFrameHead(uint8_t protectedID)
{
    writeBreak();
    HAL_UART_Transmit(huart, &SYNC_FIELD, 1, 100);
    HAL_UART_Transmit(huart, &protectedID, 1, 100);
}

size_t LinFrameTransfer::writeBreak()
{
    uint32_t currentBaud = huart->Init.BaudRate;
    huart->Init.BaudRate = baud / 2;
    HAL_UART_Init(huart);

    uint8_t breakChar = BREAK_FIELD;
    HAL_UART_Transmit(huart, &breakChar, 1, 100);

    huart->Init.BaudRate = currentBaud;  // Use the stored baud rate
    HAL_UART_Init(huart);

    return 1;
}

constexpr uint8_t LinFrameTransfer::getProtectedID(const uint8_t frameID)
{
    uint8_t p0 = ((frameID >> 0) & 1) ^ ((frameID >> 1) & 1) ^
                 ((frameID >> 2) & 1) ^ ((frameID >> 4) & 1);
    uint8_t p1 = ~(((frameID >> 1) & 1) ^ ((frameID >> 3) & 1) ^
                   ((frameID >> 4) & 1) ^ ((frameID >> 5) & 1));
    return ((p1 << 7) | (p0 << 6) | (frameID & FRAME_ID_MASK));
}

std::optional<std::vector<uint8_t>> LinFrameTransfer::receiveFrameExtractData(uint8_t protectedID, size_t expectedDataLength)
{
    FrameReader frameReader(protectedID, expectedDataLength, getChecksumLin2x);
    uint8_t rxByte;
    uint32_t startTime = HAL_GetTick();

    while ((HAL_GetTick() - startTime < timeout_ReadFrame) && !frameReader.isFinish())
    {
        if (HAL_UART_Receive(huart, &rxByte, 1, 1) == HAL_OK)
            frameReader.processByte(rxByte);
    }

    if (!frameReader.isFinish())
        return std::nullopt;

    return frameReader.getData();
}

bool LinFrameTransfer::receiveFrameHead(uint8_t protectedID)
{
    FrameReader frameReader(protectedID, 0, nullptr);
    uint8_t rxByte;
    uint32_t startTime = HAL_GetTick();

    while ((HAL_GetTick() - startTime < timeout_ReadFrame) && !frameReader.hasHead())
    {
        if (HAL_UART_Receive(huart, &rxByte, 1, 1) == HAL_OK)
            frameReader.processByte(rxByte);
    }

    return frameReader.hasHead();
}

uint8_t LinFrameTransfer::getChecksumLin2x(const uint8_t protectedID, const std::vector<uint8_t> &data)
{
    if ((protectedID & FRAME_ID_MASK) >= FRAME_ID::MASTER_REQUEST)
        return getChecksumEnhanced(0x00, data);
    return getChecksumEnhanced(protectedID, data);
}

uint8_t LinFrameTransfer::getChecksumLin13(const uint8_t protectedID, const std::vector<uint8_t> &data)
{
    return getChecksumEnhanced(protectedID, data);
}

uint8_t LinFrameTransfer::getChecksumClassic(const std::vector<uint8_t> &data)
{
    return getChecksumEnhanced(0x00, data);
}

uint8_t LinFrameTransfer::getChecksumEnhanced(const uint8_t protectedID, const std::vector<uint8_t> &data)
{
    uint16_t sum = protectedID;
    for (const uint8_t byte : data)
        sum += byte;

    sum = (sum & 0xFF) + (sum >> 8);
    sum += (sum >> 8);
    return static_cast<uint8_t>(~sum);
}
