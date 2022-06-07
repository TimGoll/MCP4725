/**
  ******************************************************************************
  * @file           MCP4725.h
  * @author         Tim Goll
  * @date           23.09.2021
  * @brief          Handles the communication with the external DACs.
  ******************************************************************************
  * Since the MCU has limited ports, those analog outputs that have no time
  * sensitivity are handled via external DACs. Those have to be interfaced which is
  * done via this library.
  *
  ******************************************************************************
  */

#pragma once

#include "stm32h7xx_hal.h" // Needed for I2C

/**
 * @brief An enum containing the different modes for the output resistance.
 */
typedef enum {
	MCP4725_MODE_NORMAL = 0b00000000,
	MCP4725_MODE_1k = 0b01000000,
	MCP4725_MODE_100k = 0b10000000,
	MCP4725_MODE_500k = 0b11000000
} MCP4725_Mode_t;

/**
 * @brief An enum specifying if the eeprom should be used.
 */
typedef enum {
	MCP4725_DAC_AND_EEPROM = 0,
	MCP4725_DAC_ONLY = 1
} MCP4725_DataMode_t;

/**
 * @brief A struct that has all sensor specific data.
 */
typedef struct {
	I2C_HandleTypeDef *i2cHandle; ///< I2C handle
	HAL_StatusTypeDef i2cState; ///< Store the state of the i2c state here
	uint16_t address; ///< The address of the specific i2c device
	MCP4725_Mode_t mode; ///< The current shutdown mode
	uint16_t output_voltage; ///< The output voltage as an integer
} MCP4725_t;

/**
 * Initializes one external DAC. The set data is then added to the sensor data struct.
 *
 * @param [in] *dev The sensor data struct that contains everything regarding the specific sensor instance
 * @param [in] *i2cHandle The handler to the i2c port which is connected to the external DAC
 * @param [in] address The 7-bit i2c address
 *
 * @retval HAL_StatusTypeDef HAL Status
 */
HAL_StatusTypeDef MCP4725_Initialize(MCP4725_t *dev, I2C_HandleTypeDef *i2cHandle, uint16_t address);

/**
 * Changes the output mode of the external DAC. Normally this should be kept to the default value.
 *
 * @param [in] *dev The sensor data struct that contains everything regarding the specific sensor instance
 * @param [in] mode The DAC output mode that should be set
 * @param [in] eeprom_or_dac Defines if the eeprom of the DAC should be used
 *
 * @retval HAL_StatusTypeDef HAL Status
 */
HAL_StatusTypeDef MCP4725_SetMode(MCP4725_t *dev, MCP4725_Mode_t mode, MCP4725_DataMode_t eeprom_or_dac);

/**
 * Sets the voltage of the external DAC.
 *
 * @param [in] *dev The sensor data struct that contains everything regarding the specific sensor instance
 * @param [in] voltage The new voltage that should be set; it is capped at the max value
 * @param [in] eeprom_or_dac Defines if the eeprom of the DAC should be used
 *
 * @retval HAL_StatusTypeDef HAL Status
 */
HAL_StatusTypeDef MCP4725_SetVoltage(MCP4725_t *dev, uint16_t voltage, MCP4725_DataMode_t eeprom_or_dac);

/**
 * Reads the current data from the chip and updates the sensor data object.
 *
 * @param [out] *dev The sensor data struct that contains everything regarding the specific sensor instance
 *
 * @retval HAL_StatusTypeDef HAL Status
 */
HAL_StatusTypeDef MCP4725_GetDataFromChip(MCP4725_t *dev);

/**
 * A helper function that streamlines reading data from the external DAC.
 *
 * @param [in] *dev The sensor data struct that contains everything regarding the specific sensor instance
 * @param [out] *data The buffer array that is used to receive the data
 * @param [in] length The amount of bytes that should be read
 *
 * @retval HAL_StatusTypeDef HAL Status
 */
HAL_StatusTypeDef MCP4725_ReadData(MCP4725_t *dev, uint8_t *data, uint8_t length);

/**
 * A helper function that streamlines reading writing to the external DAC. It writes the data that is contained within the sensor data struct.
 *
 * @param [in] *dev The sensor data struct that contains everything regarding the specific sensor instance
 * @param [in] eeprom_or_dac Defines if the eeprom of the DAC should be used
 *
 * @retval HAL_StatusTypeDef HAL Status
 */
HAL_StatusTypeDef MCP4725_WriteData(MCP4725_t *dev, MCP4725_DataMode_t eeprom_or_dac);
