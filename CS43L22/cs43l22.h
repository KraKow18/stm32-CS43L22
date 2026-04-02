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
#define CS43_OPERATION_CHECK(operation) do{ HAL_StatusTypeDef status = (operation); \
											if(status != HAL_OK) return status; \
										}while(0);
// REGISTERS
#define REG_INIT_00				0x00
#define REG_INIT_47				0x47
#define REG_INIT_32				0x32
#define REG_POWER_CTRL_1		0x02
#define REG_POWER_CTRL_2		0x04
#define REG_CLOCKING_CTRL 		0x05
#define REG_INTERFACE_CTRL_1	0x06
#define REG_PLAYBACK_CTRL_1 	0x0D
#define REG_PLAYBACK_CTRL_2 	0x0F
#define REG_MASTER_VOL_A		0x20
#define REG_HEADPHONE_A_VOL		0x22
#define REG_HEADPHONE_B_VOL		0x23
#define REG_SPEAKER_VOL			0X24


// settings for startup
typedef struct{
	GPIO_TypeDef*	resetPort;
	uint8_t			resetPin;
	uint32_t		sampleRateFrequency;
	uint8_t			initialVolume;
}CS43L22_InitTypeDef;

// settings for usage
typedef struct{
	I2C_HandleTypeDef*   i2c;
	I2S_HandleTypeDef*   i2s;
	CS43L22_InitTypeDef  Init;
	uint8_t deviceAddress;
	uint8_t masterVolume;
	uint8_t speakerVolume;
	uint8_t headphoneVolume;
	uint8_t volumeMuted;
}CS43L22_HandleTypeDef;

// FUNCTIONS
static HAL_StatusTypeDef writeToRegister(CS43L22_HandleTypeDef* cs43l22, uint16_t registerAddress, uint8_t* datasToWrite);
static HAL_StatusTypeDef readRegister(CS43L22_HandleTypeDef* cs43l22, uint16_t registerAddress, uint8_t* datasRead);

HAL_StatusTypeDef CS43L22_Initialization(CS43L22_HandleTypeDef* cs43l22);
HAL_StatusTypeDef CS43L22_Reset(CS43L22_HandleTypeDef* cs43l22);

HAL_StatusTypeDef setHeadphoneVolume(CS43L22_HandleTypeDef* cs43l22, uint8_t targetVolume);
HAL_StatusTypeDef setMasterVolume(CS43L22_HandleTypeDef* cs43l22, uint8_t targetVolume);
HAL_StatusTypeDef muteheadphoneOutput(CS43L22_HandleTypeDef* cs43l22);
HAL_StatusTypeDef unmuteHeadphoneOutput(CS43L22_HandleTypeDef* cs43l22);


#endif /* CS43L22_H_ */
