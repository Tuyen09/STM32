
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
uint16_t tick = 0, time = 0,_time = 0, mode = 0, tk = 0;
void SystemClock_Config(void);
void PWM_Soft(uint16_t GPIO_Pin,uint8_t delay_on, uint8_t delay_off)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_Pin, GPIO_PIN_SET);
	HAL_Delay(delay_on);
	HAL_GPIO_WritePin(GPIOD, GPIO_Pin, GPIO_PIN_RESET);
	HAL_Delay(delay_off);
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
  MX_GPIO_Init();
  while (1)
  {
		// y 1 o day
		static int pin = -1, GPIO, set, ret = 0;
		
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 1 )
		{
			ret = 0;
			while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 1 )
			{
				_time++;
				ret = 0;
				set = 1;
				if(_time >= 100) 
				{
					mode = !mode;
					_time = 0;
					while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 1 && mode == 1 )
					{
						//pin--;
						break;
					};			
					while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 1 && mode == 0 ) {
						HAL_Delay(20);
						ret = 1;
						while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 1 && mode == 0 ) 
						{
							HAL_Delay(20);
							if( ret == 1 ) break;
						}
					}
				}
				if( mode == 0 ) PWM_Soft(GPIO, 10, 10);
				if( mode == 1 ) {
					HAL_GPIO_WritePin(GPIOD, GPIO, GPIO_PIN_RESET);
					HAL_Delay(20);
				}
			}
			_time = 0; 
			if( mode == 0 ) 
			{
				pin++;
				if( pin > 2 ) pin = 0;
			}
			
			if( pin == 0 && ret == 0) 
			{
				GPIO = GPIO_PIN_12;
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
			}
			
			if( pin == 1 && ret == 0) 
			{
				GPIO = GPIO_PIN_13;
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
			}
			if( pin == 2 && ret == 0) 
			{
				GPIO = GPIO_PIN_13 | GPIO_PIN_12;
				
			}
			ret = 0;
		}
		// y 2 ne
		static int _mode = 0;
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1)
		{
			while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1) 
			{
				set = 2;
				time++;
				if(time >= 100)
				{
					tick = tick + 5;
					if( tick >= 20 ) tick = 20;
					time = 0;
				}
				PWM_Soft(GPIO_PIN_12 | GPIO_PIN_13, tick,20-tick);
			}
			if(_mode == 0 ) tick = tick + 5;
			if(_mode == 1 ) tick = tick - 5;
			
			time = 0;
			if(tick >= 20 ) 
			{
				tick = 20;
				_mode = !_mode;
			}
			if(tick <= 1 ) 
			{
				tick = 0;
				_mode = !_mode;
			}
		}
		
		switch(set)
		{
			case 1:
			{
				if( mode == 0 ) PWM_Soft(GPIO, 10, 10);
				if( mode == 1 ) HAL_GPIO_WritePin(GPIOD, GPIO, GPIO_PIN_RESET);
				break;
			}
			case 2: 
			{
				if( tick == 0 ) {
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13, GPIO_PIN_RESET);
				}
				else if( tick >= 20 ) 
				{
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13, GPIO_PIN_SET);
				}
				else PWM_Soft(GPIO_PIN_12 | GPIO_PIN_13, tick,20-tick);
				break;
			}
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

#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
