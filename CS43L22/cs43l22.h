/*
 * cs43l22.h
 *
 *  Created on: Mar 26, 2026
 *      Author: mars
 */

#ifndef CS43L22_H_
#define CS43L22_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

// MACROS
#define REG_OPERATION_CHECK(operation) do{ HAL_StatusTypeDef status = (operation); \
											if(status != HAL_OK) return status; \
										}while(0);

// I2C
#define DEVICE_ADDRESS 			0x94
#define DEVICE_PORT				GPIOD
#define DEVICE_PIN				GPIO_PIN_14

// REGISTERS
#define REG_INIT_00				0x00
#define REG_INIT_47				0x47
#define REG_INIT_32				0x32
#define REG_POWER_CTRL_1		0x02
#define REG_CLOCKING_CTRL 		0x05
#define REG_INTERFACE_CTRL_1	0x06
#define REG_PLAYBACK_CTRL_1 	0x0D
#define REG_PLAYBACK_CTRL_2 	0x0F
#define REG_MASTER_VOL_A		0x20
#define REG_HEADPHONE_VOL		0x22
#define REG_SPEAKER_VOL			0X24


// settings for startup
typedef struct{
	uint8_t		resetPort;
	uint8_t		resetPin;
	uint32_t	sampleRateFrequency;
	uint8_t		initialVolume;
}CS43L22_InitTypeDef;

// settings for usage
typedef struct{
	I2C_HandleTypeDef*   i2c;
	CS43L22_InitTypeDef  Init;
	uint8_t deviceAddress;
	uint8_t masterVolume;
	uint8_t speakerVolume;
	uint8_t headphoneVolume;
	uint8_t volumeMuted;
}CS43L22_HandleTypeDef;

// FUNCTIONS
HAL_StatusTypeDef CS43L22_Initialization(CS43L22_HandleTypeDef* cs43l22, I2C_HandleTypeDef* i2c, uint16_t i2c_adr);
HAL_StatusTypeDef CS43L22_Reset(CS43L22_HandleTypeDef* cs43l22, I2C_HandleTypeDef* i2c, uint16_t i2c_adr);

HAL_StatusTypeDef writeToRegister(CS43L22_HandleTypeDef* cs43l22, uint16_t registerAddress, uint8_t* datasToWrite);
HAL_StatusTypeDef readRegister(CS43L22_HandleTypeDef* cs43l22, uint16_t registerAddress, uint8_t* datasRead);

HAL_StatusTypeDef setVolume(CS43L22_HandleTypeDef* cs43l22, uint8_t targetVolume);
HAL_StatusTypeDef muteOutput(CS43L22_HandleTypeDef* cs43l22);
HAL_StatusTypeDef unmuteOutput(CS43L22_HandleTypeDef* cs43l22);


#endif /* CS43L22_H_ */
