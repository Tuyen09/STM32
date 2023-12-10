
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
uint16_t PIN_OUT[4] = {GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};
void SystemClock_Config(void);
void PWM_Soft(uint16_t GPIO_Pin, uint16_t delay_on, uint16_t delay_off)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_Pin, GPIO_PIN_SET);
	HAL_Delay(delay_on);
	HAL_GPIO_WritePin(GPIOD, GPIO_Pin, GPIO_PIN_RESET);
	HAL_Delay(delay_off);

}
void mode1()
{
		int  j;
		for(int i = 0; i < 10; i++)
		{
			for(j = 0; j < 5; j++)
			{
				PWM_Soft(GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, i,10-i);
			}
			
		}
		for(int i = 0; i < 10; i++)
		{
			for(j = 0; j < 5; j++)
			{
				PWM_Soft(GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, 10-i,i);
			}
		}
}

void mode2()
{
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
	HAL_Delay(500);
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  while (1)
  {
		for(int tick = 0; tick < 5; tick++)
		{
			mode1();
		}
		for(int tick = 0; tick < 10; tick++)
		{
			mode2();
		}

  }
}
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
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
