/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "gpio.h"

osThreadId_t Blink01Handle;
osThreadId_t Blink02Handle;
volatile int mode=0;
uint32_t lastButtonPressTime = 0;

#define DEBOUNCE_DELAY 150 
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    UNUSED(GPIO_Pin);

    if (GPIO_Pin == GPIO_PIN_0)
    {
        uint32_t currentTime = HAL_GetTick();
        if (currentTime - lastButtonPressTime > DEBOUNCE_DELAY)
        {
            lastButtonPressTime = currentTime;
            mode = !mode;
        }
    }
}

void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
void StartBlink01(void *argument);
void StartBlink02(void *argument);

void PWM_Soft(int time_on, int time_off, GPIO_TypeDef *PORT, uint16_t PIN)
{
    HAL_GPIO_WritePin(PORT, PIN, GPIO_PIN_SET);
    osDelay(time_on);
    HAL_GPIO_WritePin(PORT, PIN, GPIO_PIN_RESET);
    osDelay(time_off);
}
void task1(uint16_t GPIO_PIN1)
{
	for( int i = 0; i < 5; i++)
	{
		for( int j = 0; j < 20; j++)
		{
			PWM_Soft( i, 5 - i, GPIOD, GPIO_PIN1);
		}
	}
	for( int i = 0; i < 5; i++)
	{
		for( int j = 0; j < 20; j++)
		{
			PWM_Soft( 5 - i, i, GPIOD, GPIO_PIN1);
		}
	}
}
void task2(uint16_t GPIO_PIN2)
{
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN2);
	osDelay(500);
}


int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  osKernelInitialize();  
  MX_FREERTOS_Init();

  const osThreadAttr_t blink01_attributes = {
    .name = "blink01",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 128
  };
  Blink01Handle = osThreadNew(StartBlink01, NULL, &blink01_attributes);

  const osThreadAttr_t blink02_attributes = {
    .name = "blink02",
    .priority = (osPriority_t) osPriorityBelowNormal,
    .stack_size = 128
  };
  Blink02Handle = osThreadNew(StartBlink02, NULL, &blink02_attributes);

  osKernelStart();

  while (1)
  {
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

void StartBlink01(void *argument)
{
		for(;;)
		{
			if( mode == 0 ) task1(GPIO_PIN_13);
			else task1(GPIO_PIN_14);
		}
		osThreadTerminate(NULL);
	
}

void StartBlink02(void *argument)
{  
		for(;;)
		{
			if( mode != 0 ) task2(GPIO_PIN_13);
			else task2(GPIO_PIN_14);
		}
		osThreadTerminate(NULL);  
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM5) {
    HAL_IncTick();
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
