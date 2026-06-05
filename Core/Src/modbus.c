#include "stm32h5xx_hal.h"   // HAL functions
#include "stm32h5xx_hal_spi.h"
#include "stm32h5xx_hal_uart.h" 
#include "stdint.h"          // for uint8_t, uint16_t
#include "modbus.h"
extern UART_HandleTypeDef huart3;


// CRC16 function (Modbus standard)
uint16_t Modbus_CRC16(uint8_t *buf, uint16_t len) {
    uint16_t crc = 0xFFFF;
    for (uint16_t pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos];
        for (int i = 0; i < 8; i++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

// Example: send Modbus response frame
void Modbus_SendResponse(uint8_t slave_id, uint8_t function_code,
                         uint8_t *data, uint16_t data_len) {
    uint8_t frame[256];
    uint16_t idx = 0;

    // Slave ID
    frame[idx++] = slave_id;

    // Function Code
    frame[idx++] = function_code;

    // Byte Count
    frame[idx++] = data_len;

    // Data
    for (uint16_t i = 0; i < data_len; i++) {
        frame[idx++] = data[i];
    }

    // CRC16
    uint16_t crc = Modbus_CRC16(frame, idx);
    frame[idx++] = crc & 0xFF;        // CRC Low byte
    frame[idx++] = (crc >> 8) & 0xFF; // CRC High byte

    // Send via UART3
    HAL_UART_Transmit(&huart3, frame, idx, HAL_MAX_DELAY);
}
