#include "main.h"
#include "usart.h"
#include "gpio.h"

#include <string.h>
#include <stdio.h>

typedef enum bool {false, true} bool;

uint8_t receive;
bool Enable_write = false;
bool Done = false, flag = false;

char data_rx[100];
int data_tx[100];

int num_pre = 0, count = 0, width = 0, count_num = 0;;

void SystemClock_Config(void);

void send_uart(char* string) 
{
    uint8_t len = strlen(string);
    HAL_UART_Transmit(&huart2, (uint8_t *)string, len, 2000);
}

void bubbleSort(int arr[], int n) 
{
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{
    if (huart->Instance == huart2.Instance) 
		{
        if (Enable_write == false) {
            Enable_write = true;
            Done = false;
            count = 0;
        }
	
        if (Enable_write == true) 
				{
            data_rx[count] = receive;
						char Display[10];
            sprintf(Display, "%d", data_rx[count] - 48);
            if (data_rx[count] >= '0' && data_rx[count] <= '9') {
                num_pre = num_pre * 10 + (data_rx[count] - 48);
            }
            if (data_rx[count] == ' ') {
                data_tx[count_num++] = num_pre;
                num_pre = 0;
                width++;
            }

            count++;
        }

        if (receive == 'p' && Enable_write == true) 
				{
            data_tx[count_num++] = -1;
            Enable_write = false;
            Done = true;
            flag = true;
        }

        HAL_UART_Receive_IT(&huart2, &receive, 1);
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();

    HAL_UART_Receive_IT(&huart2, &receive, 1);

    while (1) 
		{
        if (flag == true) 
				{
            if (Done == true) 
						{
                send_uart("Finish\n");
                bubbleSort(data_tx, width);
                for (int k = 0; k < width; k++) 
								{
                    char Display2[10];
                    sprintf(Display2, "\n%d", data_tx[k]);
                    send_uart(Display2);
                }
            }

            count = 0;
            count_num = 0;
            width = 0;
            memset(data_rx, 0, 100);
            memset(data_tx, 0, 100);

            flag = false;
        }
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
  */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 72;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
  */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
