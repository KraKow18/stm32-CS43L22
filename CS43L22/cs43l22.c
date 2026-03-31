/*
 * cs43l22.c
 *
 *  Created on: Mar 26, 2026
 *      Author: mars
 */

#include "cs43l22.h"

HAL_StatusTypeDef CS43L22_Initialization(CS43L22_HandleTypeDef* cs43l22, I2C_HandleTypeDef* i2c, uint16_t i2c_adr){
	// power-up sequence
	uint8_t datasToWrite;
	uint8_t tempRegisterValueRead;

	// 1) Hold /RESET low until power supplies are stable
	HAL_GPIO_WritePin(cs43l22->Init.resetPort, cs43l22->Init.resetPin, GPIO_PIN_RESET);
	HAL_Delay(20);

	// 2) Bring /RESET high
	HAL_GPIO_WritePin(cs43l22->Init.resetPort, cs43l22->Init.resetPin, GPIO_PIN_SET);

	// 3) Laod "power up" value in power_control_1 register
	datasToWrite = 0x9E;
	writeToRegister(cs43l22, REG_POWER_CTRL_1, &datasToWrite);

	// 4) Required initialization settings
	datasToWrite = 0x99;
	writeToRegister(cs43l22, REG_INIT_00, &datasToWrite);

	datasToWrite = 0x80;
	writeToRegister(cs43l22, REG_INIT_47, &datasToWrite);

	readRegister(cs43l22, REG_INIT_32, &tempRegisterValueRead);
	tempRegisterValueRead |= (1 << 7);
	datasToWrite = tempRegisterValueRead;
	writeToRegister(cs43l22, REG_INIT_32, &datasToWrite);

	readRegister(cs43l22, REG_INIT_32, &tempRegisterValueRead);
	datasToWrite = tempRegisterValueRead & ~(1 << 7);
	writeToRegister(cs43l22, REG_INIT_32, &datasToWrite);

	datasToWrite = 0x99;
	writeToRegister(cs43l22, REG_INIT_32, &datasToWrite);

	datasToWrite = 0x00;
	writeToRegister(cs43l22, REG_INIT_00, &datasToWrite);

	// 5) Clock: 12MHz, Sample Rate = 96kHz
	/* slave mode
	 * auto_detect = 0
	 * Speed mode = 00 double-speed mode (slave mode)
	 * 32k sample rate group = 0
	 * videoclk = 0
	 * mclk/lrck ratio = 01
	 * mclkdiv2 = 0
	 * => 0b00000010 = 0x02
	 */

	// set slave mode
	readRegister(cs43l22, REG_INTERFACE_CTRL_1, &tempRegisterValueRead);
	datasToWrite = tempRegisterValueRead & ~(1 << 7);
	writeToRegister(cs43l22, REG_INTERFACE_CTRL_1, &datasToWrite);

	// set clock settings
	datasToWrite = 0x02;
	writeToRegister(cs43l22, REG_CLOCKING_CTRL, &datasToWrite);

	// 6) Set power_control_1 at 0x9E
	datasToWrite = 0x9E;
	writeToRegister(cs43l22, REG_POWER_CTRL_1, &datasToWrite);
}

