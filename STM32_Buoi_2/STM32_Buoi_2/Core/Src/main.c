/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
void SystemClock_Config(void);
//int count;
//uint16_t time = 0, value;
//unsigned char Status, LastStatus;


//unsigned char button_status()
//{
//	return count >= 4 ? GPIO_PIN_SET : GPIO_PIN_RESET;
//}

//void button_begin()
//{
//	value = 0;
//	for( unsigned char i = 0; i < 5; i++)
//	{
//		unsigned char read = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
//		value <<= 1;
//		value = read;
//		count += read;
//	}
//	time = 0;
//	Status = button_status();
//	LastStatus = 0;
//}

//void button_pro()
//{
//	unsigned char read = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
//	value <<= 1;
//	value = read;
//	count += read;
//	
//}
#define button_nhan 1
#define button_nha 0
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  while (1)
  {
		// cach 1 
		/* 
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1 )
		{
			while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1 );
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		}
		*/
		// cach 2 
		static int truoc, hientai,dem =0, count = 0;
		truoc =hientai;
		hientai=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
				if(hientai == button_nha && truoc == button_nha ) {
		HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_12);
		}
		if(hientai ==button_nhan && truoc==button_nhan){
				HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_13);
			}
		if(hientai == button_nha && truoc == button_nhan ) {
		HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_14);
				dem++;
		}
	
				if(hientai == button_nhan && truoc == button_nha ) {
		HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_15);
					count++;
			
		}

		HAL_Delay(50);
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
