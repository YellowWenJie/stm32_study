/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../../User/Inc/motor.h"
#include "../../User/Inc/oled.h"
#include "../../User/Inc/key.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t Encoder_Count;

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  OLED_Clear();
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  // 清除中断，否则一开机就会进入一次中断
  __HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
  // 启动计数
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
  // 更新中断用于溢出计数
  __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);
  // 定义一个显示数字的变量
  uint16_t num = 0;
  // 马达速度
  int speed = 0;
  uint16_t TOP_UP_Key_Count, TOP_MID_Key_Count, TOP_DOWN_Key_Count;

  uint16_t BOTTON_LEFT_Key_Count, BOTTON_MID_Key_Count, BOTTON_RIGHT_Key_Count;

  Motor(Top_Motor, 0);
  Motor(Bottom_Motor, 0);
  OLED_ShowString(0, 0, "T_U:", 8);
  OLED_ShowString(0, 1, "T_M:", 8);
  OLED_ShowString(0, 2, "T_D:", 8);
  OLED_ShowString(0, 3, "Speed:", 8);

  OLED_ShowString(60, 0, "B_L:", 8);
  OLED_ShowString(60, 1, "B_M:", 8);
  OLED_ShowString(60, 2, "B_R:", 8);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    OLED_ShowNum(28, 0, Get_Key_Status(TOP_UP_GPIO_Port, TOP_UP_Pin), 1, 8);
    OLED_ShowNum(36, 0, TOP_UP_Key_Count, 4, 8);
    OLED_ShowNum(28, 1, Get_Key_Status(TOP_MID_GPIO_Port, TOP_MID_Pin), 1, 8);
    OLED_ShowNum(36, 1, TOP_MID_Key_Count, 4, 8);
    OLED_ShowNum(28, 2, Get_Key_Status(TOP_DOWN_GPIO_Port, TOP_DOWN_Pin), 1, 8);
    OLED_ShowNum(36, 2, TOP_DOWN_Key_Count, 4, 8);

    OLED_ShowNum(88, 0, Get_Key_Status(BOTTON_LEFT_GPIO_Port, BOTTON_LEFT_Pin), 1, 8);
    OLED_ShowNum(96, 0, BOTTON_LEFT_Key_Count, 4, 8);
    OLED_ShowNum(88, 1, Get_Key_Status(BOTTON_MID_GPIO_Port, BOTTON_MID_Pin), 1, 8);
    OLED_ShowNum(96, 1, BOTTON_MID_Key_Count, 4, 8);
    OLED_ShowNum(88, 2, Get_Key_Status(BOTTON_RIGHT_GPIO_Port, BOTTON_RIGHT_Pin), 1, 8);
    OLED_ShowNum(96, 2, BOTTON_RIGHT_Key_Count, 4, 8);

    OLED_ShowNum(48, 3, Encoder_Count, 4, 8);

    Set_Pwm(Encoder_Count);

    if (Get_Key_Status(TOP_UP_GPIO_Port, TOP_UP_Pin) == 1)
    {
      // TOP_UP_Key_Count += 1;
      if (Get_Key_Status(BOTTON_RIGHT_GPIO_Port, BOTTON_RIGHT_Pin) == 1)
      {
        // BOTTON_RIGHT_Key_Count += 1;
        Motor(Top_Motor, 0);
        Motor(Bottom_Motor, -1);
      }
      else if (Get_Key_Status(BOTTON_LEFT_GPIO_Port, BOTTON_LEFT_Pin) == 1)
      {
        // BOTTON_LEFT_Key_Count += 1;
        Motor(Top_Motor, -1);
        Motor(Bottom_Motor, 0);
      }
      else
      {
        Motor(Bottom_Motor, 0);
        Motor(Top_Motor, 0);
      }
    }
    if (Get_Key_Status(TOP_DOWN_GPIO_Port, TOP_DOWN_Pin) == 1)
    {
      // TOP_DOWN_Key_Count += 1;
      if (Get_Key_Status(BOTTON_LEFT_GPIO_Port, BOTTON_LEFT_Pin) == 1)
      {
        // BOTTON_LEFT_Key_Count += 1;
        Motor(Top_Motor, 0);
        Motor(Bottom_Motor, 1);
      }
      else if (Get_Key_Status(BOTTON_RIGHT_GPIO_Port, BOTTON_RIGHT_Pin) == 1)
      {
        // BOTTON_RIGHT_Key_Count += 1;
        Motor(Top_Motor, 1);
        Motor(Bottom_Motor, 0);
      }
      else
      {
        Motor(Bottom_Motor, 0);
        Motor(Top_Motor, 0);
      }
    }

    // HAL_Delay(200);
    // Motor(Top_Motor, -1);
    // Motor(Bottom_Motor, -1);
    // HAL_Delay(200);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  uint32_t val;

  if (htim == &htim2)
  {
    val = (uint32_t)(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2));
    if (val == 1)
    {
      Encoder_Count += 100;
    }
    if (val == 0)
    {
      Encoder_Count -= 100;
    }
    if (Encoder_Count > 8400)
    {
      Encoder_Count = 0;
    }
  }
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
