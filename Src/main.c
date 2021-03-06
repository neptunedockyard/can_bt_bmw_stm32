/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan;

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

uint8_t transmitBuffer[32];
uint8_t receiveBuffer[32];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI2_Init(void);
static void MX_CAN_Init(void);
static void MX_NVIC_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void LOG(char* pData, int length);
void rawMouseCommand(uint8_t btns, uint8_t x, uint8_t y);
void rawKeyCommand(uint8_t modifiers, uint8_t key1, uint8_t key2, uint8_t key3, uint8_t key4, uint8_t key5, uint8_t key6);
void rawConsumerCommand(uint8_t mask0, uint8_t mask1);
void sleep(uint32_t time);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_SPI2_Init();
  MX_CAN_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();

  /* USER CODE BEGIN 2 */

  //Init UART2 interrupt driven RX for keyboard input
  LOG("\033[2J", 5);
  LOG("\033[H", 4);
  LOG("Starting system...OK\r\n", 22);
  LOG("Initializing UART1\r\n", 22);
  LOG("Initializing UART2\r\n", 22);
  HAL_UART_Receive_IT(&huart1, receiveBuffer, 32);

  //Init CAN bus and conversion system here
  LOG("Initializing CAN bus\r\n", 22);

//  if(HAL_CAN_Receive_IT(&hcan, CAN_FIFO0) != HAL_OK) {
//	  //Handle errors, Reception error
//	  LOG("No CAN data received\r\n", 22);
//  } else {
//	  //Process CAN info
//	  LOG("CAN data received\r\n", 19);
//  }

  LOG("Initializing Bluetooth HID device\r\n", 35);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	  //the code below currently spams the bluetooth module and transmits way too
	  //fast, have to find a way to slow it down, delay doesn't work
//	  LOG("Right\r\n", 7);
//	  rawMouseCommand(0, -100, 0);
//	  sleep(10000000);
////	  LOG("Down\r\n", 6);
//	  rawMouseCommand(0, 0, -100);
//	  sleep(10000000);
////	  LOG("Left\r\n", 6);
//	  rawMouseCommand(0, 100, 0);
//	  sleep(10000000);
////	  LOG("Up\r\n", 4);
//	  rawMouseCommand(0, 0, 100);
//	  sleep(10000000);
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
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

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/** NVIC Configuration
*/
static void MX_NVIC_Init(void)
{
  /* I2C1_EV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
  /* I2C1_ER_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/* CAN init function */
static void MX_CAN_Init(void)
{

  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 16;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SJW = CAN_SJW_1TQ;
  hcan.Init.BS1 = CAN_BS1_1TQ;
  hcan.Init.BS2 = CAN_BS2_1TQ;
  hcan.Init.TTCM = DISABLE;
  hcan.Init.ABOM = DISABLE;
  hcan.Init.AWUM = DISABLE;
  hcan.Init.NART = DISABLE;
  hcan.Init.RFLM = DISABLE;
  hcan.Init.TXFP = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_CAN_IRQHandler(&hcan);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

}

/* SPI2 init function */
static void MX_SPI2_Init(void)
{

  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_UART_IRQHandler(&huart1);
}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_UART_IRQHandler(&huart2);
}

/** Pinout Configuration
*/
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/*
 * Logging system, will write logs to UART1, console
 */
void LOG(char* pData, int length){
	HAL_UART_Transmit_IT(&huart1, pData, length);
	while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY){}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart){
	uint16_t uartDATA[16];
	HAL_UART_Transmit(&huart1, "test", 4, 30);
	if(HAL_UART_Receive_IT(&huart1, receiveBuffer, 32) != HAL_OK) {
		//Handle errors
		LOG("No UART data received\r\n", 23);
	} else {
		//Process UART info
		LOG("UART data received\r\n", 20);
		LOG(uartDATA, 16);
	}

}

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan){
	uint8_t canDATA[8];
	HAL_CAN_Receive_IT(&hcan, canDATA);
	LOG(canDATA, 8);
}

void rawMouseCommand(uint8_t btns, uint8_t x, uint8_t y) {
	HAL_UART_Transmit(&huart2, 0xFD, sizeof(uint8_t), 30);
//	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY){}
	HAL_UART_Transmit(&huart2, 0x00, sizeof(uint8_t), 30);
//	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY){}
	HAL_UART_Transmit(&huart2, 0x03, sizeof(uint8_t), 30);
//	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY){}
	HAL_UART_Transmit(&huart2, btns, sizeof(uint8_t), 30);
//	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY){}
	HAL_UART_Transmit(&huart2, x, sizeof(uint8_t), 30);
//	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY){}
	HAL_UART_Transmit(&huart2, y, sizeof(uint8_t), 30);
//	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY){}
	HAL_UART_Transmit(&huart2, 0x00, sizeof(uint8_t), 30);
//	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY){}
	HAL_UART_Transmit(&huart2, 0x00, sizeof(uint8_t), 30);
//	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY){}
	HAL_UART_Transmit(&huart2, 0x00, sizeof(uint8_t), 30);
//	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY){}
}

void rawKeyCommand(uint8_t modifiers, uint8_t key1, uint8_t key2, uint8_t key3, uint8_t key4, uint8_t key5, uint8_t key6) {
	//for keys not used, set them to 0 when calling
	HAL_UART_Transmit_IT(&huart2, 0xFD, sizeof(uint8_t));
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
	HAL_UART_Transmit_IT(&huart2, modifiers, sizeof(uint8_t));
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
	HAL_UART_Transmit_IT(&huart2, 0x00, sizeof(uint8_t));
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
	HAL_UART_Transmit_IT(&huart2, key1, sizeof(uint8_t));
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
	HAL_UART_Transmit_IT(&huart2, key2, sizeof(uint8_t));
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
	HAL_UART_Transmit_IT(&huart2, key3, sizeof(uint8_t));
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
	HAL_UART_Transmit_IT(&huart2, key4, sizeof(uint8_t));
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
	HAL_UART_Transmit_IT(&huart2, key5, sizeof(uint8_t));
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
	HAL_UART_Transmit_IT(&huart2, key6, sizeof(uint8_t));
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
}

void rawConsumerCommand(uint8_t mask0, uint8_t mask1){
	HAL_UART_Transmit_IT(&huart2, 0xFD, sizeof(uint8_t));
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
	HAL_UART_Transmit_IT(&huart2, 0x00, sizeof(uint8_t));
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
	HAL_UART_Transmit_IT(&huart2, 0x02, sizeof(uint8_t));
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
	HAL_UART_Transmit_IT(&huart2, mask0, sizeof(uint8_t));
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
	HAL_UART_Transmit_IT(&huart2, mask1, sizeof(uint8_t));
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
	HAL_UART_Transmit_IT(&huart2, 0x00, sizeof(uint8_t));
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
	HAL_UART_Transmit_IT(&huart2, 0x00, sizeof(uint8_t));
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
	HAL_UART_Transmit_IT(&huart2, 0x00, sizeof(uint8_t));
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
	HAL_UART_Transmit_IT(&huart2, 0x00, sizeof(uint8_t));
	while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);
}

void sleep(uint32_t time){
	time*=72000000;
	while(time--);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
