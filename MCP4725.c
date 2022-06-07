/*
 * MCP4725.c
 *
 *  Created on: 23.09.2021
 *      Author: Tim Goll
 */

#include "MCP4725.h"

HAL_StatusTypeDef MCP4725_Initialize(MCP4725_t *dev, I2C_HandleTypeDef *i2cHandle, uint16_t address) {
	dev->i2cHandle = i2cHandle;
	dev->address = address << 1; // shift by one to make room for read/write bit
	dev->mode = MCP4725_MODE_NORMAL;
	dev->output_voltage = 2047;

	// now set the initial data to the data that is set on the chip
	dev->i2cState = MCP4725_GetDataFromChip(dev);

	return dev->i2cState;
}

HAL_StatusTypeDef MCP4725_SetMode(MCP4725_t *dev, MCP4725_Mode_t mode, MCP4725_DataMode_t eeprom_or_dac) {
	dev->mode = mode;

	return MCP4725_WriteData(dev, eeprom_or_dac);
}

HAL_StatusTypeDef MCP4725_SetVoltage(MCP4725_t *dev, uint16_t voltage, MCP4725_DataMode_t eeprom_or_dac) {
	dev->output_voltage = voltage;

	return MCP4725_WriteData(dev, eeprom_or_dac);
}

HAL_StatusTypeDef MCP4725_GetDataFromChip(MCP4725_t *dev) {
	uint8_t data[3];

	HAL_StatusTypeDef status = MCP4725_ReadData(dev, data, 3);

	// an error occurred, don't proceed with parsing
	if (status != HAL_OK) {
		return status;
	}

	dev->mode = (MCP4725_Mode_t) (data[0] << 5) & 0b11000000;
	dev->output_voltage = (uint16_t) ((data[1] << 4) | (data[2] >> 4)) & 0b0000111111111111;

	return status;
}

HAL_StatusTypeDef MCP4725_ReadData(MCP4725_t *dev, uint8_t *data, uint8_t length) {
	return HAL_I2C_Master_Receive(dev->i2cHandle, dev->address, data, length, HAL_MAX_DELAY);
}

HAL_StatusTypeDef MCP4725_WriteData(MCP4725_t *dev, MCP4725_DataMode_t eeprom_or_dac) {
	if (eeprom_or_dac == MCP4725_DAC_AND_EEPROM) {
		uint8_t data[3];

		data[0] = (uint8_t) 0b01100000 | ((((uint8_t) dev->mode) >> 5) & 0b00000110);
		data[1] = (uint8_t) dev->output_voltage >> 4;
		data[2] = (uint8_t) (dev->output_voltage << 4) & 0b11110000;

		return HAL_I2C_Master_Transmit(dev->i2cHandle, dev->address, data, 3, HAL_MAX_DELAY);
	} else if (eeprom_or_dac == MCP4725_DAC_ONLY) {
		uint8_t data[2];

		data[0] = (uint8_t) ((((uint8_t) dev->mode) >> 2) & 0b00110000) | ((dev->output_voltage >> 8) & 0b00001111);
		data[1] = (uint8_t) dev->output_voltage;

		return HAL_I2C_Master_Transmit(dev->i2cHandle, dev->address, data, 2, HAL_MAX_DELAY);
	}

	// return HAL_ERROR if the mode is not defined
	return HAL_ERROR;
}
