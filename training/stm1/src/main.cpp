#include "main.h"
#include <vector>
#include <stdio.h>

UART_HandleTypeDef huart2;

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART2_UART_Init(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    printf("Master ready, sending data on frame ID 0x10\n");
    while (1)
    {
        std::vector<uint8_t> data = {0x01, 0x02, 0x03};
        printf("Sending: ");
        for (uint8_t byte : data)
        {
            printf("0x%02X ", byte);
        }
        printf("\n");
        uint8_t frame[8] = {0};
        frame[0] = 0x10;
        for (size_t i = 0; i < data.size() && i < 8; i++)
        {
            frame[i + 1] = data[i];
        }
        HAL_UART_Transmit(&huart2, frame, data.size() + 1, HAL_MAX_DELAY);
        printf("Frame sent\n");
        HAL_Delay(1000);
    }
}

void SystemClock_Config(void) { /* ... */ }
void MX_GPIO_Init(void) { /* ... */ }
void MX_USART2_UART_Init(void) { /* ... */ }
int _write(int file, char *ptr, int len) { /* ... */ }