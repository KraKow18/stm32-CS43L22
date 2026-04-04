# CS43L22 DRIVER FOR STM32

This basic driver is intended to be used with the headphone output only. It has been written with STM32F407 Discovery board.

## Features:
- Start device
- Stop device
- Set headphone output volume (0-100)
- Mute/Unmute headphone output
- Mute/Unmute all outputs

Future developement will include speakers monitoring and beep generator for fast debugging.

## Basic usage:

1) You need to setup I2C and I2S to make the device work.

2) By using CS43L22_HandleTypeDef you can setup the device:
   
```c
CS43L22_HandleTypeDef hcs43;

static void cs43_config()
{
	hcs43.i2c = &hi2c1;
	hcs43.i2s = &hi2s3;
	hcs43.deviceAddress = 0x94;
	hcs43.Init.resetPort = CS43_Reset_GPIO_Port;
	hcs43.Init.resetPin = CS43_Reset_Pin;
}
```

- CS43_Reset_GPIO_Port and CS43_Reset_Pin design the PIN RESET of the device (for example on the STM32F407 discovery, it's PD4).
- Device address of the I2C is defined by the manufacturer.
- The sample rate of the device is correlated to the sample rate of the I2S protocol, 96kHz in this example.
- DMA must be configured in Circular mode on the I2S TX channel, otherwise audio stops after one buffer.

```c
static void MX_I2S3_Init(void)
{
  hi2s3.Instance = SPI3;
  hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_96K;
  hi2s3.Init.CPOL = I2S_CPOL_LOW;
  hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  if (HAL_I2S_Init(&hi2s3) != HAL_OK)
  {
    Error_Handler();
  }
}
```

3) Basic usage in main program:
   
```c
  /* Initialize device settings */
  cs43_config();

  /* system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_I2S3_Init();
  
  /* Configure and start CS43L22 */
  CS43L22_Initialization(&hcs43);
  unmuteHeadphoneOutput(&hcs43);
  setHeadphoneVolume(&hcs43, 100);
```
