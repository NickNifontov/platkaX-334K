/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "comp.h"
#include "crc.h"
#include "dac.h"
#include "dma.h"
#include "hrtim.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "plxOC_V.h"
#include "plxOC_I.h"
#include "plxADC.h"
#include "plxPID.h"
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
float resultPID = 0.0f;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */
void PWM_OutputOFF(void);
void PWM_OutputON(void);
void ResetHRTIMdef(void);
void StartHRTIM(void);
void StopHRTIM(void);
void SetDacs(void);
void ResetDacs(void);
void SetPWMCycle(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void PWM_OutputOFF(void)
{
	HRTIM1->sCommonRegs.ODISR= HRTIM_ODISR_TB1ODIS + HRTIM_ODISR_TB2ODIS;  // Disable HRTIM's output
}

void PWM_OutputON(void)
{
	HRTIM1->sCommonRegs.OENR = HRTIM_OENR_TB1OEN + HRTIM_OENR_TB2OEN;  //Enable HRTIM's outputs
}

void ResetHRTIMdef(void)
{
	HRTIM1->sTimerxRegs[1].CMP2xR=PULS_A_END;
	HRTIM1->sTimerxRegs[1].CMP4xR=PULS_B_END;
	TIM1->CCR1=0;
	TIM1->CCR2=0;
	TOP_BOTTOM=0;
	Sinus_Ind=0;
	SetDacs();
	ResetADCBuff(plxOCV,sizeBuffer);
	ResetADCBuff(plxOCI,sizeBuffer);
	BufferIndOC=0;
}

void StartHRTIM(void)
{
	PWM_OutputON(); //Enable HRTIM's outputs
	HAL_HRTIM_WaveformCountStart_IT(&hhrtim1, HRTIM_TIMERID_MASTER +  HRTIM_TIMERID_TIMER_B); // Start HRTIM's Master TIMER and TIMER B

}

void StopHRTIM(void)
{
	PWM_OutputOFF(); // Disable HRTIM's output
	HAL_HRTIM_WaveformCountStop_IT(&hhrtim1, HRTIM_TIMERID_MASTER +  HRTIM_TIMERID_TIMER_B); // Stop HRTIM's Master TIMER and TIMER B
	ResetHRTIMdef();
}

void SetDacs(void)
{
	DAC2->DHR12R1 = Current_Level;
	DAC1->DHR12R2 = OC_V_Table[Sinus_Ind];
}

void ResetDacs(void)
{
	DAC2->DHR12R1 = Current_Level_Reset;
	DAC1->DHR12R2 = Voltage_Level_Reset;
}

/*void HAL_HRTIM_Compare2EventCallback(HRTIM_HandleTypeDef * hhrtim,
                                              uint32_t TimerIdx)
{
	ResetDacs();
	SetPWMCycle();
	SetDacs();
}*/

void SetPWMCycle(void)
{
	uint32_t a_puls=PULS_A_BEGIN+(uint32_t)(4*OC_V_Table[Sinus_Ind]);
	uint32_t b_puls=PULS_B_BEGIN+(uint32_t)(4*OC_V_Table[Sinus_Ind]);

	if (a_puls>PULS_A_END) {
		a_puls=PULS_A_END;
	}

	if (b_puls>PULS_B_END) {
		b_puls=PULS_B_END;
	}

	HRTIM1->sTimerxRegs[1].CMP2xR=a_puls;
	HRTIM1->sTimerxRegs[1].CMP4xR=b_puls;
}

void HAL_HRTIM_RepetitionEventCallback(HRTIM_HandleTypeDef * hhrtim,
                                              uint32_t TimerIdx)
{
	ResetDacs();

	if (Sinus_Ind==Sinus_Table_Resolution-1)
	{
		//Sinus_Ind=0;
		TIM1->CCR1=0;
		TIM1->CCR2=0;
		PWM_OutputOFF();
		//HRTIM1->sMasterRegs.MREP=4;
	}

	Sinus_Ind++;

	if (Sinus_Ind>=MAX_SIN) {
			Sinus_Ind=0;
			if (TOP_BOTTOM==0) {
				TIM1->CCR1=1535;
				TIM1->CCR2=0;
				TOP_BOTTOM=1; //next bottom
			} else {
				TIM1->CCR1=0;
				TIM1->CCR2=1553;
				TOP_BOTTOM=0; // next top
			}
			PWM_OutputON();
			//HRTIM1->sMasterRegs.MREP=0;
	}

	SetPWMCycle();

	SetDacs();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (HAL_GPIO_ReadPin(PIN_PEREK_GPIO_Port, PIN_PEREK_Pin)==GPIO_PIN_SET)
		{
			StartHRTIM();
		} else {
			StopHRTIM();
		}
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	BufferInd++;
	BufferIndOC++;
	if (BufferInd>=sizeBuffer) {
		BufferInd=0;
	}
	if (BufferIndOC>=sizeBuffer) {
		BufferIndOC=0;
	}
	GetADC1();
	GetADC2();
	CalcFlag=1;
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
  MX_HRTIM1_Init();
  MX_IWDG_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_DAC1_Init();
  MX_DAC2_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_COMP2_Init();
  MX_COMP4_Init();
  MX_TIM2_Init();
  MX_TIM15_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_CRC_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */

  //*****************************************************//
  //Start LED_V , LED_I Timer2
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  //*****************************************************//

  //*****************************************************//
  //Start Buzzer Timer15
  HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
  //*****************************************************//

  //*****************************************************//
  //Start Cooler Timer16
  HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
  //*****************************************************//

  //*****************************************************//
  //Start IGLA Timer17
  HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);
  //*****************************************************//

  //*****************************************************//
  //Start 50HZ Timer1
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  //*****************************************************//

  //*****************************************************//
  //Start DAC2 Out1 Current
  HAL_DAC_Start(&hdac2, DAC_CHANNEL_1);
  //*****************************************************//

  //*****************************************************//
  //Start DAC1 Out2 Volatge
  HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
  //*****************************************************//

  //*****************************************************//
  //Start COMP2 (Voltage) and COMP4 (Current)
  HAL_COMP_Start(&hcomp2);
  HAL_COMP_Start(&hcomp4);
  //*****************************************************//

  //*****************************************************//
  //Pre-Init Global
  ResetADCBuff(plxV16,sizeBuffer);
  ResetADCBuff(plxRAW,sizeBuffer);
  ResetADCBuff(plxTEMP,sizeBuffer);
  BufferInd=0;
  ResetHRTIMdef();
  //*****************************************************//

  //*****************************************************//
  //Start ADC1 and ADC2
  //HAL_ADC_Start_DMA(&hadc1,
  //                        (uint32_t *)ADC1_Data,
  //						  ADC1_Cnt_Channel);
  HAL_ADCEx_Calibration_Start(&hadc2,ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED);
  HAL_ADCEx_InjectedStart(&hadc1);
  HAL_ADCEx_InjectedStart_IT(&hadc2);
  //*****************************************************//

  //*****************************************************//
  //Unblock Driver Output
  HAL_GPIO_WritePin(PIN_DRV_GPIO_Port, PIN_DRV_Pin, GPIO_PIN_RESET);
  //*****************************************************//

  //*****************************************************//
  //Start
  HAL_GPIO_EXTI_Callback(PIN_PEREK_Pin);
  //*****************************************************//

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  TIM15->CCR1=0; //stop Buzzer
  HAL_Delay(3000);
  StartHRTIM();

  while (1)
  {
	hiwdg.Instance->KR=IWDG_KEY_RELOAD; // reload IWDG

	if (CalcFlag==1) {
		gV16=FilterWindowMedium(plxV16, sizeBuffer, sizeWindow);
		gRAW=FilterWindowMedium(plxRAW, sizeBuffer, sizeWindow);
		gTEMP=FilterWindowMedium(plxTEMP, sizeBuffer, sizeWindow);

		hiwdg.Instance->KR=IWDG_KEY_RELOAD; // reload IWDG

		gOCV=FilterWindowMedium(plxOCV, sizeBuffer, sizeWindow);
		gOCI=FilterWindowMedium(plxOCI, sizeBuffer, sizeWindow);

		hiwdg.Instance->KR=IWDG_KEY_RELOAD; // reload IWDG

		// Volatge
		resultPID = 0.0f;
		pid_SetReference(2.2); //reference voltage opora
		pid_SetSaturation(-40000, 40000);
		pid_SetFeedback(gOCV, 0.0002);
		pid_SetCoefficient(50, 0, 0, 0, 0);
		pid_Compute();
		resultPID = pid_Get();

		// Current
		/*resultPID = 0.0f;
		pid_SetReference(2.2); //reference voltage current
		pid_SetSaturation(-40000, 40000);
		pid_SetFeedback(gOCV, 0.0002);
		pid_SetCoefficient(10, 0, 0, 0, 0);
		pid_Compute();
		resultPID = pid_Get();*/



		CalcFlag=0;
	}
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_HRTIM1|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_TIM1|RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_PLLCLK;
  PeriphClkInit.Hrtim1ClockSelection = RCC_HRTIM1CLK_PLLCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* HRTIM1_Master_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(HRTIM1_Master_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(HRTIM1_Master_IRQn);
  /* HRTIM1_TIMB_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(HRTIM1_TIMB_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(HRTIM1_TIMB_IRQn);
  /* HRTIM1_FLT_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(HRTIM1_FLT_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(HRTIM1_FLT_IRQn);
  /* ADC1_2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(ADC1_2_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
  /* PVD_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PVD_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(PVD_IRQn);
  /* EXTI15_10_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 4, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 4, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
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

#ifdef  USE_FULL_ASSERT
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
