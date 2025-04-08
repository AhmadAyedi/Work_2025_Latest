// Core/Inc/LinFrameTransfer.hpp
#ifndef LINFRAMETRANSFER_HPP
#define LINFRAMETRANSFER_HPP

#include "stm32l4xx_hal.h"
#include <vector>
#include <optional>

class LinFrameTransfer
{
public:
    static constexpr bool writeReadback_verify = false;  // Disabled for now
    static constexpr bool writeReadback_throw = false;

    static constexpr uint8_t BREAK_FIELD = 0x00;
    static constexpr uint8_t SYNC_FIELD = 0x55;
    static constexpr uint8_t FRAME_ID_MASK = 0b00111111;

    enum FRAME_ID : const uint8_t
    {
        MASTER_REQUEST = 0x3C,
        SLAVE_REQUEST = 0x3D
    };

    LinFrameTransfer(UART_HandleTypeDef* uartHandle, int verbose = -1) :
        huart(uartHandle),
        verboseLevel(verbose)
    {
    }

    UART_HandleTypeDef* huart;

    int verboseLevel;
    unsigned long baud = 19200;
    int8_t rxPin = -1;
    int8_t txPin = -1;

    bool writeFrame(const uint8_t frameID, const std::vector<uint8_t>& data);
    bool writeEmptyFrame(const uint8_t frameID);

    std::optional<std::vector<uint8_t>> readFrame(const uint8_t frameID, uint8_t expectedDataLength = 8);

protected:
    bool writeFrameHead(const uint8_t protectedID);  // Changed to bool return type
    size_t writeBreak();
    constexpr uint8_t getProtectedID(const uint8_t frameID);

    std::optional<std::vector<uint8_t>> receiveFrameExtractData(uint8_t protectedID, size_t expectedDataLength);
    bool receiveFrameHead(uint8_t protectedID);

    static uint8_t getChecksumLin2x(uint8_t protectedID, const std::vector<uint8_t>& data);
    static uint8_t getChecksumLin13(const uint8_t protectedID, const std::vector<uint8_t>& data);
    static uint8_t getChecksumClassic(const std::vector<uint8_t>& data);
    static uint8_t getChecksumEnhanced(const uint8_t protectedID, const std::vector<uint8_t>& data);
};

#endif /* LINFRAMETRANSFER_HPP */
