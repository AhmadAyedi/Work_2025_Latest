/*
 * linlib.h
 *
 *  Created on: Apr 4, 2025
 *      Author: Mediatek
 */

#ifndef INC_LINLIB_H_
#define INC_LINLIB_H_

#include "stm32l4xx_hal.h"  // For UART_HandleTypeDef and HAL functions
#include <stdint.h>

// Function prototypes
uint8_t Compute_Parity(uint8_t id);
uint8_t Compute_Checksum(uint8_t *data, uint8_t len, uint8_t pid);
void Send_LIN_Frame(UART_HandleTypeDef *huart);

#endif /* INC_LINLIB_H_ */
