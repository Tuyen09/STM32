/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "gpio.h"

#define DS1307_I2C_ADDRESS 0xD0
uint8_t hour, minute, second, day, month, year;

void SystemClock_Config(void);

uint8_t bcdToDec(uint8_t value)
{
  return ((value / 16 * 10) + (value % 16));
}
uint8_t decToBcd(uint8_t val)
{
  return ((val / 10 * 16) + (val % 10));
}

void DS1307_ReadTime(uint8_t *hour, uint8_t *minute, uint8_t *second)
{
  uint8_t buffer[3];
  

  HAL_I2C_Mem_Read(&hi2c1, DS1307_I2C_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, buffer, 3, HAL_MAX_DELAY);
  
 
  *second = bcdToDec(buffer[0] & 0x7F);
  *minute = bcdToDec(buffer[1]);
  *hour = bcdToDec(buffer[2] & 0x3F);
}

void DS1307_SetTime(uint8_t hour, uint8_t minute, uint8_t second)
{
  uint8_t timeData[3];
  
  timeData[0] = decToBcd(second);
  timeData[1] = decToBcd(minute);
  timeData[2] = decToBcd(hour);
  
  HAL_I2C_Mem_Write(&hi2c1, DS1307_I2C_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, timeData, 3, HAL_MAX_DELAY);
}
void DS1307_ReadDate(uint8_t *day, uint8_t *month, uint8_t *year)
{
  uint8_t buffer[3];
  
 	 HAL_I2C_Mem_Read(&hi2c1, DS1307_I2C_ADDRESS, 0x04, I2C_MEMADD_SIZE_8BIT, buffer, 3, HAL_MAX_DELAY);
  *day = bcdToDec(buffer[0]);
  *month = bcdToDec(buffer[1]);
  *year = bcdToDec(buffer[2]);

  
}
void DS1307_SetDate(uint8_t day, uint8_t month, uint8_t year)
{
  uint8_t dateData[3];
  
  dateData[0] = decToBcd(day);
  dateData[1] = decToBcd(month);
  dateData[2] = decToBcd(year);
  
  HAL_I2C_Mem_Write(&hi2c1, DS1307_I2C_ADDRESS, 0x04, I2C_MEMADD_SIZE_8BIT, dateData, 3, HAL_MAX_DELAY);
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
	HAL_I2C_Init(&hi2c1);
	DS1307_SetTime(23,59,50); 
	DS1307_SetDate(10,6,23);

  while (1)
  {
		DS1307_ReadTime(&hour, &minute, &second);
		DS1307_ReadDate(&day, &month, &year);
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}
void Error_Handler(void)
{

}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
