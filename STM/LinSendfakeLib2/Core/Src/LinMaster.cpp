#include "LinMaster.hpp"
#include <stdio.h>
#include <string.h>

LinMaster::LinMaster(UART_HandleTypeDef* uartHandle, uint8_t linId, const std::string& initialData) {
    huart = uartHandle;
    id = (linId > 0x3F) ? 0x01 : linId;  // Limit ID to 6 bits, default to 1 if invalid
    data = initialData;
    memset(lin_frame, 0, sizeof(lin_frame));  // Clear frame buffer
}

uint8_t LinMaster::computeParity(uint8_t id) {
    uint8_t p0 = (id & 0x01) ^ ((id >> 1) & 0x01) ^ ((id >> 2) & 0x01) ^ ((id >> 4) & 0x01);
    uint8_t p1 = ~(((id >> 1) & 0x01) ^ ((id >> 3) & 0x01) ^ ((id >> 4) & 0x01) ^ ((id >> 5) & 0x01)) & 0x01;
    return (p0 & 0x01) | ((p1 & 0x01) << 1);
}

uint8_t LinMaster::computeChecksum(const uint8_t* data, uint8_t len, uint8_t pid) {
    uint16_t sum = pid;
    for (uint8_t i = 0; i < len; i++) {
        sum += data[i];
        if (sum >= 256) sum -= 255;
    }
    return (uint8_t)(~sum);
}

void LinMaster::sendFrame() {
    uint8_t sync_field = 0x55;  // Sync
    uint8_t pid = id | (computeParity(id) << 6);  // PID with parity
    uint8_t data_len = data.length();
    const uint8_t* data_bytes = reinterpret_cast<const uint8_t*>(data.c_str());
    uint8_t checksum = computeChecksum(data_bytes, data_len, pid);

    // Build the frame
    lin_frame[0] = sync_field;
    lin_frame[1] = pid;
    for (uint8_t i = 0; i < data_len; i++) {
        lin_frame[i + 2] = data_bytes[i];
    }
    lin_frame[data_len + 2] = checksum;

    // Send Break
    HAL_LIN_SendBreak(huart);

    // Send the frame
    HAL_UART_Transmit(huart, lin_frame, data_len + 3, HAL_MAX_DELAY);

    // SWV output
    std::string output = "Sent LIN Frame - Sync: 0x" + std::to_string(sync_field) +
                         ", PID: 0x" + std::to_string(pid) + ", Data: ";
    for (uint8_t i = 0; i < data_len; i++) {
        output += "0x" + std::to_string(data_bytes[i]) + " ";
    }
    output += ", Checksum: 0x" + std::to_string(checksum) + "\n";
    printf("%s", output.c_str());
}
