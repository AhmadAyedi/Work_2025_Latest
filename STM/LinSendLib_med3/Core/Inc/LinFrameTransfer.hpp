/*
 * LinFrameTransfer.hpp
 *
 *  Created on: Apr 7, 2025
 *      Author: Mediatek
 */

#ifndef INC_LINFRAMETRANSFER_HPP_
#define INC_LINFRAMETRANSFER_HPP_


#include "stm32l4xx_hal.h"  // STM32 HAL header
#include <vector>
#include <optional>

// Forward declaration of UART Handle
extern UART_HandleTypeDef huart2;  // Assuming UART2 is used, adjust as needed

class LinFrameTransfer
{
public:
    // Do readback written bytes and verify
    static constexpr bool writeReadback_verify = true;
    // Do readback written bytes and ignore
    static constexpr bool writeReadback_throw = false;

    static constexpr uint8_t BREAK_FIELD = 0x00;
    static constexpr uint8_t SYNC_FIELD = 0x55;
    static constexpr uint8_t FRAME_ID_MASK = 0b00111111;

    enum FRAME_ID : const uint8_t
    {
        MASTER_REQUEST = 0x3C,
        SLAVE_REQUEST = 0x3D
    };

    // Constructor takes UART handle instead of HardwareSerial
    LinFrameTransfer(UART_HandleTypeDef* uartHandle, int verbose = -1) :
        huart(uartHandle),
        verboseLevel(verbose)
    {
        // Debug stream not implemented in this basic version
        // Could be added later with another UART instance if needed
    }

    UART_HandleTypeDef* huart;  // Pointer to UART handle

    int verboseLevel;
    unsigned long baud = 19200;
    int8_t rxPin = -1;  // Not used in STM32 HAL directly
    int8_t txPin = -1;  // Not used in STM32 HAL directly

    bool writeFrame(const uint8_t frameID, const std::vector<uint8_t>& data);
    bool writeEmptyFrame(const uint8_t frameID);

    std::optional<std::vector<uint8_t>> readFrame(const uint8_t frameID, uint8_t expectedDataLength = 8);

protected:
    inline void writeFrameHead(const uint8_t protectedID);
    inline size_t writeBreak();
    inline constexpr uint8_t getProtectedID(const uint8_t frameID);

    std::optional<std::vector<uint8_t>> receiveFrameExtractData(uint8_t protectedID, size_t expectedDataLength);
    bool receiveFrameHead(uint8_t protectedID);

    inline static uint8_t getChecksumLin2x(uint8_t protectedID, const std::vector<uint8_t>& data);
    inline static uint8_t getChecksumLin13(const uint8_t protectedID, const std::vector<uint8_t>& data);
    inline static uint8_t getChecksumClassic(const std::vector<uint8_t>& data);
    static uint8_t getChecksumEnhanced(const uint8_t protectedID, const std::vector<uint8_t>& data);
};


#endif /* INC_LINFRAMETRANSFER_HPP_ */
