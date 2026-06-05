#ifndef MODBUS_H
#define MODBUS_H

#include <stdint.h>

uint16_t Modbus_CRC16(uint8_t *buf, uint16_t len);
void Modbus_SendResponse(uint8_t slave_id, uint8_t function_code,
                         uint8_t *data, uint16_t data_len);

#endif
