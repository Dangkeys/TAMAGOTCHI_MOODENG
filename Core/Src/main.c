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

#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"

#include "snow_tiger.h"

#include "ui_manager.h"
#include "moodeng.h"
#include "timer.h"
#include "stm32f7xx_hal.h"
#include "buzzer.h"
#include <string.h>
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
bool shouldClearScreen = false;

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
Clock_t gameClock;
uint32_t lastUpdateTime = 0;

typedef enum {
    MEAL = 0,
    SNACK
} Food_t;
Food_t foodSelected = MEAL;

//Sound
extern const int mario[2][52];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void printStatus(void)
{
  const char* emotionStr;
  switch (moodeng.emotion) {
    case NORMAL:
      emotionStr = "NORMAL";
      break;
    case SILLY:
      emotionStr = "SILLY";
      break;
    case SCOLDED:
      emotionStr = "SCOLDED";
      break;
    default:
      emotionStr = "UNKNOWN";
      break;
  }

  char msg[512];
  int n = snprintf(msg, sizeof(msg),
    "Time: %02d:%02d:%02d\r\n"
    "Happy: %d\r\n"
    "Weight: %d\r\n"
    "Hunger: %d\r\n"
    "PoopCount: %d\r\n"
    "PoopRate: %.2f\r\n"
    "isSick: %d\r\n"
    "HealRate: %.2f\r\n"
    "Discipline: %d\r\n"
    "isTired: %d\r\n"
    "Evolution: %d\r\n"
    "isAlive: %d\r\n"
    "Emotion: %s\r\n"
    "nextDecayHappy: %d\r\n"
    "nextDecayHunger: %d\r\n"
    "nextPoopTime: %d\r\n"
    "nextSickTime: %d\r\n"
    "nextHurtTime: %d\r\n"
    "nextDirtyTime: %d\r\n"
    "nextSleepyTime: %d\r\n"
    "sleepingTime: %d\r\n"
    "isSleeping: %d\r\n"
    "--------------------------\r\n",
    gameClock.hour, gameClock.minute, gameClock.second,
    moodeng.happy,
    moodeng.weight,
    moodeng.hunger,
    moodeng.poopCount,
    moodeng.poopRate,
    moodeng.isSick,
    moodeng.healRate,
    moodeng.discipline,
    moodeng.isTired,
    moodeng.evolution,
    moodeng.isAlive,
    emotionStr,
    moodeng.nextDecayHappy,
    moodeng.nextDecayHunger,
    moodeng.nextPoopTime,
    moodeng.nextSickTime,
    moodeng.nextHurtTime,
    moodeng.nextDirtyTime,
    moodeng.nextSleepyTime,
    moodeng.sleepingTime,
    moodeng.isSleeping
  );

  if (n > 0)
  {
    HAL_UART_Transmit(&huart3, (uint8_t *)msg, n, HAL_MAX_DELAY);
  }
}

void printValue(int value)
{
    char msg[32];
    int n = snprintf(msg, sizeof(msg), "Value: %02d\r\n", value);
    if (n > 0)
    {
        HAL_UART_Transmit(&huart3, (uint8_t *)msg, n, HAL_MAX_DELAY);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
      case GPIO_PIN_0:
        Handle_Button_Yellow();
        break;

      case GPIO_PIN_3:
        Handle_Button_Red();
        break;

      case GPIO_PIN_5:
        Handle_Button_Blue();
        break;

      default:
        break;
    }
}

void Handle_Button_Yellow(void);
void Handle_Button_Red(void);
void Handle_Button_Blue(void);

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
    MX_TIM2_Init();
    MX_TIM3_Init();
    
    /* USER CODE BEGIN 2 */
    Moodeng_Init(&moodeng);
    Timer_Init(&gameClock);
    ILI9341_Init(); // initial driver setup to drive ili9341
    ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
    ILI9341_Fill_Screen(DARKGREY);
    //  Display_Screen();

  UIManager_Init(&ui);
  ui.menuState = MENU_MAIN;     // confirmed state
  ui.selectedState = MENU_MAIN; // highlighted state

    HAL_ADC_Start(&hadc1);
    HAL_TIM_Base_Start_IT(&htim1);
    //Buzzer
    HAL_TIM_Base_Start_IT(&htim3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 500); //duty cycle 50% (loudest)
    buzzer_play_sound(mario);
//    __HAL_TIM_SET_PRESCALER(&htim2, 53);

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

    //    if (shouldClearScreen)
    //    {
    //      shouldClearScreen = false;
    //      Display_Screen();
    //    }

    printStatus();
    HAL_Delay(100);
    uint32_t currentTime = HAL_GetTick();

    UIManager_Update(&ui, currentTime);

    if (currentTime - lastUpdateTime >= 100)
    {
        UIManager_Draw(&ui);
        lastUpdateTime = currentTime;
    }

    if (shouldClearScreen)
    {
        ILI9341_Draw_Text("Medicine", 120, 10, DARKGREY, 2, DARKGREY);
        ILI9341_Draw_Text("Medicine", 120, 40, DARKGREY, 2, DARKGREY);
        shouldClearScreen = false;
    }

    /* USER CODE END 3 */
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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
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
void Handle_Button_Yellow(void)
  {
    static uint32_t lastTick = 0;
    uint32_t now = HAL_GetTick();
    if (now - lastTick < 200) return; // debounce
    lastTick = now;

    switch (ui.menuState)
    {
      case MENU_MAIN:
        //select action(menu)
        ui.selectedState = (ui.selectedState + 1) % 6;
        //skip select main menu
        if (ui.selectedState == MENU_MAIN)
          ui.selectedState = MENU_FEED;
        break;
      
      case MENU_FEED:
        //select food
        foodSelected = (foodSelected == MEAL) ? SNACK : MEAL;
        break;

      case MENU_PLAY:
        //guess left
        //return true if win
        if (Moodeng_Minigame(&moodeng, 0)) {
          ui.activeAnim = &miniGameCorrectAnim;
        } 
        else {
          ui.activeAnim = &miniGameWrongAnim;
        }
        break;
      
      case MENU_SLEEP:
        //isSleeping = confirm
        if (moodeng.isSleeping == false) {
          moodeng.sleepingTime = 0;
          moodeng.isSleeping = true;
        }
        break;

      default:
        break;
    }
    shouldClearScreen = true;
  }

void Handle_Button_Red(void)
  {
    static uint32_t lastTick = 0;
    uint32_t now = HAL_GetTick();
    if (now - lastTick < 200) return;
    lastTick = now;
    //exit sleeping
    if (ui.menuState == MENU_SLEEP && moodeng.isSleeping == true){
      moodeng.isSleeping = false;
      if(moodeng.sleepingTime >= 1800){ //seconds
        moodeng.isTired = 0;
        moodeng.nextSleepyTime = 480;
      }
      moodeng.sleepingTime = 0;
    }
    //back to main menu
    else if (ui.menuState != MENU_MAIN) {
        UIManager_SetState(&ui, MENU_MAIN);
        shouldClearScreen = true;
    } 
    //in main menu => scolding
    else {
      if (moodeng.emotion != SILLY) {
        moodeng.happy--;
        if(moodeng.happy < 0) moodeng.happy = 0;
      }
      else {
        moodeng.discipline++;
        if(moodeng.discipline > 6) moodeng.discipline = 6;
        moodeng.emotion = SCOLDED;
      }
    }
  }

void Handle_Button_Blue(void)
{
  static uint32_t lastTick = 0;
  uint32_t now = HAL_GetTick();
  if (now - lastTick < 200) return;
  lastTick = now;

  switch (ui.menuState)
  {
    //confirm switch menu
    case MENU_MAIN:
      //check if moodeng agree to feeding (not if it goes silly)
      if (ui.selectedState == MENU_FEED) {
        if (Moodeng_Check_Feed(&moodeng))
            UIManager_SetState(&ui, ui.selectedState);
        else
            ui.activeAnim = &stubbornAnim;
      } 
      //check if moodeng agree to playing (not if it goes silly)
      else if (ui.selectedState == MENU_PLAY) {
        if (Moodeng_Check_Play(&moodeng))
            UIManager_SetState(&ui, ui.selectedState);
        else
            ui.activeAnim = &stubbornAnim;
      } 
      else {
        UIManager_SetState(&ui, ui.selectedState);
      }
      shouldClearScreen = true;
      break;

    case MENU_FEED: 
      //feed meal
      if (foodSelected == MEAL) {
        ui.activeAnim = &feedMealAnim;
        moodeng.hunger += 2;
        moodeng.weight += 2;
        moodeng.poopRate += 0.4f;
      } 
      //feed snack
      else {
        ui.activeAnim = &feedSnackAnim;
        moodeng.happy += 2;
        moodeng.weight += 4;
        moodeng.poopRate += 0.6f;
      }
      if(moodeng.happy > 4) moodeng.happy = 4;
      if(moodeng.hunger > 4) moodeng.hunger = 4;
      if(moodeng.weight > 99) moodeng.weight = 99;
      if(moodeng.poopRate > 1.0f) moodeng.poopRate = 1.0f;
      break;

    case MENU_PLAY:
      //guess right
      //return true if win
      if (Moodeng_Minigame(&moodeng, 1)) {
        ui.activeAnim = &miniGameCorrectAnim;
      }
      else {
        ui.activeAnim = &miniGameWrongAnim;
      }
      break;

    default:
      break;
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM3) {
		if (!buzzer.running)
			return;

		// Count down the current phase (sound or silence)
		if (buzzer.ms_left > 0) {
			buzzer.ms_left--;
			return;
		}

		if (buzzer.state == BUZZ_PLAYING) {
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);

			uint32_t totalBeat =
					buzzer.pause ?
							buzzer.pause[buzzer.idx] :
							buzzer.duration[buzzer.idx];
			uint32_t silence =
					(totalBeat > (uint32_t) buzzer.duration[buzzer.idx]) ?
							(totalBeat - (uint32_t) buzzer.duration[buzzer.idx]) :
							0;

			buzzer.ms_left = silence;
			buzzer.state = BUZZ_SILENCE;
		} else if (buzzer.state == BUZZ_SILENCE) {
			buzzer.idx++;

			if (buzzer.idx >= buzzer.size) {
				// Sequence finished
				buzzer.running = 0;
				buzzer.state = BUZZ_IDLE;
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
			} else {
				// Start next tone
				buzzer_set_freq(buzzer.tone[buzzer.idx]);
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 500);
				buzzer.ms_left = buzzer.duration[buzzer.idx];
				buzzer.state = BUZZ_PLAYING;
			}
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
