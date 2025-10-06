/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "rng.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "ILI9341_Touchscreen.h"
#include "flash.h"

#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"

#include "snow_tiger.h"

#include "ui_manager.h"
#include "moodeng.h"
#include "stm32f7xx_hal.h"
// #include "string.h"
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
/* Private variables ---------------------------------------------------------*/
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
// PetState_t currentState = STATE_IDLE_STATUS;
bool shouldClearScreen = false;
// PetState_t selectNextState = STATE_IDLE_STATUS;

// const char* stateNames[STATE_COUNT] = {
//     "Idle",
//     "Meal Time",
//     "Mini Game",
//     "Sleep",
//     "Cleanup",
//     "Medicine"
// };

UIManager_t ui;
Moodeng_t moodeng;
uint32_t lastUpdateTime = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define VREF_mV 3300u
#define ADC_MAX 4095u
#define ADC_CH_COUNT 1
uint16_t adc_buf[ADC_CH_COUNT];
volatile uint32_t adc_val = 0;
static const uint8_t channel_map[ADC_CH_COUNT] = {10};

static void print_adc_line()
{
    char msg[160];

    uint16_t raw = adc_buf[0];
    uint32_t millivolts = (raw * VREF_mV + (ADC_MAX / 2)) / ADC_MAX;

    uint32_t volts_whole = millivolts / 1000;
    uint32_t volts_frac = (millivolts % 1000) / 10; // 2 decimal places

    // Format: ADC1_CHxx 0x0000xxxx  Vin = x.xx V
    int n = snprintf(msg, sizeof(msg),
                     "ADC1_CH%-2d  0x%08lX  Vin = %lu.%02lu V\r\n",
                     channel_map[0],
                     (unsigned long)raw,
                     (unsigned long)volts_whole,
                     (unsigned long)volts_frac);

    if (n > 0)
    {
        HAL_UART_Transmit(&huart3, (uint8_t *)msg, n, HAL_MAX_DELAY);
    }
}

void printStart()
{
    char msg[32];
    int n = snprintf(msg, sizeof(msg), "Start %.2f\r\n", 0.0f);
    if (n > 0)
    {
        HAL_UART_Transmit(&huart3, (uint8_t *)msg, n, HAL_MAX_DELAY);
    }
}


void printHunger(float value)
{
    char msg[32];
    int n = snprintf(msg, sizeof(msg), "Hunger: %.2f\r\n", value);
    if (n > 0)
    {
        HAL_UART_Transmit(&huart3, (uint8_t *)msg, n, HAL_MAX_DELAY);
    }
}

void printHappy(float value)
{
    char msg[32];
    int n = snprintf(msg, sizeof(msg), "Happy: %.2f\r\n", value);
    if (n > 0)
    {
        HAL_UART_Transmit(&huart3, (uint8_t *)msg, n, HAL_MAX_DELAY);
    }
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        print_adc_line();
        const char newline[] = "\r\n";
        HAL_UART_Transmit(&huart3, (uint8_t *)newline, strlen(newline), HAL_MAX_DELAY);

        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buf, ADC_CH_COUNT);
    }
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* Enable the CPU Cache */

    /* Enable I-Cache---------------------------------------------------------*/
    SCB_EnableICache();

    /* Enable D-Cache---------------------------------------------------------*/
    SCB_EnableDCache();

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
    MX_DMA_Init();
    MX_USART3_UART_Init();
    MX_SPI5_Init();
    MX_TIM1_Init();
    MX_RNG_Init();
    MX_ADC1_Init();
    /* USER CODE BEGIN 2 */
    if (!Flash_IsDataValid(NEXTDECAYHAPPY_ADDRESS))
    {
        Moodeng_Init(&moodeng);
        Flash_Write_NUM(NEXTDECAYHAPPY_ADDRESS, moodeng.nextDecayHappy);
        Flash_Write_NUM(NEXTDECAYHUNGER_ADDRESS, moodeng.nextDecayHunger);
    }
    float happy = Flash_Read_NUM(NEXTDECAYHAPPY_ADDRESS);
    float hunger = Flash_Read_NUM(NEXTDECAYHUNGER_ADDRESS);
    printStart();
//    printValueMemory(weight);

    ILI9341_Init(); // initial driver setup to drive ili9341
    ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
    ILI9341_Fill_Screen(DARKGREY);
    //  Display_Screen();

    UIManager_Init(&ui);
    ui.menuState = MENU_MAIN;     // confirmed state
    ui.selectedState = MENU_MAIN; // highlighted state

    HAL_ADC_Start(&hadc1);

    //  if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buf, ADC_CH_COUNT) != HAL_OK) {
    //      Error_Handler();
    //  }

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        printHappy(happy);
        printHunger(hunger);
        HAL_Delay(500);

        //    if (shouldClearScreen)
        //    {
        //      shouldClearScreen = false;
        //      Display_Screen();
        //    }

        // printValue(moodeng.nextDecayHappy);
        // HAL_Delay(100);
        // uint32_t currentTime = HAL_GetTick();

        // UIManager_Update(&ui, currentTime);

        // if (currentTime - lastUpdateTime >= 100)
        // {
        //     UIManager_Draw(&ui);
        //     lastUpdateTime = currentTime;
        // }

        // if (shouldClearScreen)
        // {
        //     shouldClearScreen = false;
        //     ILI9341_Fill_Screen(DARKGREY);
        //     UIManager_Draw(&ui);
        // }

        //      // Example of switching menu by button:
        //      if (ButtonPressed(FEED_BUTTON)) {
        //          UIManager_SetState(&ui, MENU_FEED);
        //      } else if (ButtonPressed(PLAY_BUTTON)) {
        //          UIManager_SetState(&ui, MENU_PLAY);
        //      }

        /* USER CODE END 3 */
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

    /** Configure LSE Drive Capability
     */
    HAL_PWR_EnableBkUpAccess();

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 216;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 9;
    RCC_OscInitStruct.PLL.PLLR = 2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Activate the Over-Drive mode
     */
    if (HAL_PWREx_EnableOverDrive() != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
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
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
