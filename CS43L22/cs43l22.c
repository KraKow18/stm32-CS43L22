/*
 * cs43l22.c
 *
 *  Created on: Mar 26, 2026
 *      Author: mars
 */

#include "cs43l22.h"

HAL_StatusTypeDef CS43L22_Initialization(CS43L22_HandleTypeDef* cs43l22){
	uint8_t datasToWrite;
	uint8_t tempRegisterValueRead;

	// power-up sequence
	// 1) Hold /RESET low until power supplies are stable
	HAL_GPIO_WritePin(cs43l22->Init.resetPort, cs43l22->Init.resetPin, GPIO_PIN_RESET);
	HAL_Delay(20);

	// 2) Bring /RESET high
	HAL_GPIO_WritePin(cs43l22->Init.resetPort, cs43l22->Init.resetPin, GPIO_PIN_SET);
	HAL_Delay(20);

	//power-down
	datasToWrite = 0x01;
	CS43_OPERATION_CHECK(writeToRegister(cs43l22, REG_POWER_CTRL_1, &datasToWrite));

	// 3) Nothing to do

	// 4) Required initialization settings
	datasToWrite = 0x99;
	CS43_OPERATION_CHECK(writeToRegister(cs43l22, REG_INIT_00, &datasToWrite));

	datasToWrite = 0x80;
	CS43_OPERATION_CHECK(writeToRegister(cs43l22, REG_INIT_47, &datasToWrite));

	CS43_OPERATION_CHECK(readRegister(cs43l22, REG_INIT_32, &tempRegisterValueRead));
	tempRegisterValueRead |= (1 << 7);
	datasToWrite = tempRegisterValueRead;
	CS43_OPERATION_CHECK(writeToRegister(cs43l22, REG_INIT_32, &datasToWrite));

	CS43_OPERATION_CHECK(readRegister(cs43l22, REG_INIT_32, &tempRegisterValueRead));
	datasToWrite = tempRegisterValueRead & ~(1 << 7);
	CS43_OPERATION_CHECK(writeToRegister(cs43l22, REG_INIT_32, &datasToWrite));

	datasToWrite = 0x99;
	CS43_OPERATION_CHECK(writeToRegister(cs43l22, REG_INIT_32, &datasToWrite));

	datasToWrite = 0x00;
	CS43_OPERATION_CHECK(writeToRegister(cs43l22, REG_INIT_00, &datasToWrite));

	// 5) set clock settings

	// auto-detect
	CS43_OPERATION_CHECK(readRegister(cs43l22, REG_CLOCKING_CTRL, &tempRegisterValueRead));
	datasToWrite = tempRegisterValueRead | (1 << 7);
	CS43_OPERATION_CHECK(writeToRegister(cs43l22, REG_CLOCKING_CTRL, &datasToWrite));


	// Interface control 1 => not in recommended sequence but have to do it
	CS43_OPERATION_CHECK(readRegister(cs43l22, REG_INTERFACE_CTRL_1, &tempRegisterValueRead));
	datasToWrite = tempRegisterValueRead & ~(1 << 7); // set slave mode
	datasToWrite &= ~(1 << 6); // clk polarity not inverted
	datasToWrite &= ~(1 << 4); // dsp mode disabled
	datasToWrite |= (1 << 2); // dac interface format: i2s, up to 24-bit data
	datasToWrite |= (3 << 0);  // word lenght = 16 bits for i2s
	CS43_OPERATION_CHECK(writeToRegister(cs43l22, REG_INTERFACE_CTRL_1, &datasToWrite));

	// 6) Set power_control_1 at 0x9E for powerup
	datasToWrite = 0x9E;
	CS43_OPERATION_CHECK(writeToRegister(cs43l22, REG_POWER_CTRL_1, &datasToWrite));

	return HAL_OK;
}

HAL_StatusTypeDef unmuteHeadphoneOutput(CS43L22_HandleTypeDef* cs43l22){
	uint8_t datasToWrite;
	uint8_t tempRegisterValueRead;

	CS43_OPERATION_CHECK(readRegister(cs43l22, REG_PLAYBACK_CTRL_2, &tempRegisterValueRead));
	tempRegisterValueRead &= ~(3 << 4);
	datasToWrite = tempRegisterValueRead;
	CS43_OPERATION_CHECK(writeToRegister(cs43l22, REG_PLAYBACK_CTRL_2, &datasToWrite));
	cs43l22->volumeMuted = 0;

	return HAL_OK;
}

HAL_StatusTypeDef muteHeadphoneOutput(CS43L22_HandleTypeDef* cs43l22){
	uint8_t datasToWrite;
	uint8_t tempRegisterValueRead;

	CS43_OPERATION_CHECK(readRegister(cs43l22, REG_PLAYBACK_CTRL_2, &tempRegisterValueRead));
	tempRegisterValueRead |= (3 << 4);
	datasToWrite = tempRegisterValueRead;
	CS43_OPERATION_CHECK(writeToRegister(cs43l22, REG_PLAYBACK_CTRL_2, &datasToWrite));
	cs43l22->volumeMuted = 1;

	return HAL_OK;
}

HAL_StatusTypeDef setMasterVolume(CS43L22_HandleTypeDef* cs43l22, uint8_t targetVolume){
	targetVolume = 0x01;
	CS43_OPERATION_CHECK(writeToRegister(cs43l22, REG_HEADPHONE_A_VOL, &targetVolume));
	return HAL_OK;
}

HAL_StatusTypeDef setHeadphoneVolume(CS43L22_HandleTypeDef* cs43l22, uint8_t targetVolume){
	uint8_t volumeAttenuation = 0x01; // muted by default

	if(targetVolume > 100) targetVolume = 100;
	if(targetVolume < 0) targetVolume = 0;

	if(targetVolume > 0){
		volumeAttenuation = (uint8_t)(-(int8_t)((100 - targetVolume) * 128 / 100));
	}

	CS43_OPERATION_CHECK(writeToRegister(cs43l22, REG_HEADPHONE_A_VOL, &volumeAttenuation)); // HPA
	CS43_OPERATION_CHECK(writeToRegister(cs43l22, REG_HEADPHONE_B_VOL, &volumeAttenuation)); // HPB
	return HAL_OK;
}

HAL_StatusTypeDef writeToRegister(CS43L22_HandleTypeDef* cs43l22, uint16_t registerAddress, uint8_t* datasToWrite){
	CS43_OPERATION_CHECK(HAL_I2C_Mem_Write(cs43l22->i2c, cs43l22->deviceAddress, registerAddress, I2C_MEMADD_SIZE_8BIT, datasToWrite, 1, HAL_MAX_DELAY));
	return HAL_OK;
}

HAL_StatusTypeDef readRegister(CS43L22_HandleTypeDef* cs43l22, uint16_t registerAddress, uint8_t* datasRead){
	CS43_OPERATION_CHECK(HAL_I2C_Mem_Read(cs43l22->i2c, cs43l22->deviceAddress, registerAddress, I2C_MEMADD_SIZE_8BIT, datasRead, 1, HAL_MAX_DELAY));
	return HAL_OK;
}
