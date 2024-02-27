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

int recv_val = 0;
int recv_flag = 0;
int led = 0;
int flag = 0;

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END 0 */

/* Sets up pins PB10 and PB11 as USART3 RX/TX */
void setup_usart(void)
{
	/* Setup PB10 as USART TX */
	// Set to alternate function output mode
	GPIOB->MODER &= ~(1<<20);
	GPIOB->MODER |= (1<<21);
	// Setup alternate function
	GPIOB->AFR[1] &= ~(1<<11);
	GPIOB->AFR[1] |= (1<<10);
	GPIOB->AFR[1] &= ~(1<<9);
	GPIOB->AFR[1] &= ~(1<<8);
	
	/* Setup PB11 as USART RX */
	// Set to alternate function output mode
	GPIOB->MODER &= ~(1<<22);
	GPIOB->MODER |= (1<<23);
	// Setup alternate function
	GPIOB->AFR[1] &= ~(1<<15);
	GPIOB->AFR[1] |= (1<<14);
	GPIOB->AFR[1] &= ~(1<<13);
	GPIOB->AFR[1] &= ~(1<<12);
	
	/* USART3 Setup */
	// Enable transceiver and receiver
	USART3->CR1 |= (1<<2);
	USART3->CR1 |= (1<<3);
	// Enable interrupt
	USART3->CR1 |= (1<<5);
	// Set baud rate to 9600 bits/sec
	USART3->BRR = HAL_RCC_GetHCLKFreq() / 9600;
	// Enable USART3
	USART3->CR1 |= (1<<0);
}


/* Sets up and initializes all 4 LEDs to low */
void initialize_leds(void)
{
	/* Setup Red LED (PC6) */
	// Set to general purpose output mode
	GPIOC->MODER |= (1<<12);
	GPIOC->MODER &= ~(1<<13);
	// Set to push-pull mode
	GPIOC->OTYPER &= ~(1<<6);
	// Set to low speed
	GPIOC->OSPEEDR &= ~(1<<12);
	GPIOC->OSPEEDR &= ~(1<<13);
	// Set no pull-up, no pull-down
	GPIOC->PUPDR &= ~(1<<12);
	GPIOC->PUPDR &= ~(1<<13);
	// Initialize to low
	GPIOC->ODR &= ~(1<<6);

	/* Setup Blue LED (PC7) */
	// Set to general purpose output mode
	GPIOC->MODER |= (1<<14);
	GPIOC->MODER &= ~(1<<15);
	// Set to push-pull mode
	GPIOC->OTYPER &= ~(1<<7);
	// Set to low speed
	GPIOC->OSPEEDR &= ~(1<<14);
	GPIOC->OSPEEDR &= ~(1<<15);
	// Set no pull-up, no pull-down
	GPIOC->PUPDR &= ~(1<<14);
	GPIOC->PUPDR &= ~(1<<15);
	// Initialize to low
	GPIOC->ODR &= ~(1<<7);
	
	/* Setup Orange LED (PC8) */
	// Set to general purpose output mode
	GPIOC->MODER |= (1<<16);
	GPIOC->MODER &= ~(1<<17);
	// Set to push-pull mode
	GPIOC->OTYPER &= ~(1<<8);
	// Set to low speed
	GPIOC->OSPEEDR &= ~(1<<16);
	GPIOC->OSPEEDR &= ~(1<<17);
	// Set no pull-up, no pull-down
	GPIOC->PUPDR &= ~(1<<16);
	GPIOC->PUPDR &= ~(1<<17);
	// Initialize to low
	GPIOC->ODR &= ~(1<<8);
	
	/* Setup Green LED (PC9) */
	// Set to general purpose output mode
	GPIOC->MODER |= (1<<18);
	GPIOC->MODER &= ~(1<<19);
	// Set to push-pull mode
	GPIOC->OTYPER &= ~(1<<9);
	// Set to low speed
	GPIOC->OSPEEDR &= ~(1<<18);
	GPIOC->OSPEEDR &= ~(1<<19);
	// Set no pull-up, no pull-down
	GPIOC->PUPDR &= ~(1<<18);
	GPIOC->PUPDR &= ~(1<<19);
	// Initialize to low
	GPIOC->ODR &= ~(1<<9);
}



/** 
	* @brief Transmits a single character on the USART 
  */
void transmit_char(char c)
{
	while(1) 
	{
		if ((USART3->ISR & (1<<7)) == (1<<7))
		{
			break;
		}
	}
	// Write char c to USART
	USART3->TDR = c;
}

/** 
	* @brief Transmits a string on the USART 
  */
void transmit_string(const char* str)
{
	while(*str != '\0')
	{
		transmit_char(*str);
		str++;
	}
}


void USART3_4_IRQHandler(void)
{
	int cmd_text_printed = 0;
	while (!(USART3->ISR & USART_ISR_RXNE)) {}
	
	recv_val = (USART3->RDR) &= 0xff;
	
	switch (recv_val)
	{
		case 'r':
			transmit_string("Commanded LED: Red\n");
			led = 6;
			break;
		case 'b':
			transmit_string("Commanded LED: Blue\n");
			led = 7;
			break;
		case 'o':
			transmit_string("Commanded LED: Orange\n");
			led = 8;
			break;
		case 'g':
			transmit_string("Commanded LED: Green\n");
			led = 9;
			break;
		case '0':
			if (led != 0)
			{
				transmit_string("Commanded Action: Off\n");
				GPIOC->ODR &= ~(1<<led);
			}
			break;
		case '1':
			if (led != 0)
			{
				transmit_string("Commanded Action: On\n");
				GPIOC->ODR |= (1<<led);
			}
			break;
		case '2':
			if (led != 0)
			{
				transmit_string("Commanded Action: Toggle\n");
				GPIOC->ODR ^= (1<<led);
			}
			break;
		default:
			transmit_string("Error: Command not valid!\n");
			led = 0;
	}
	recv_flag = 1; // Turn received flag on
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
	
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	
	initialize_leds();
	setup_usart();
	
	NVIC_EnableIRQ(USART3_4_IRQn); // Enable USART3 interrupt
	NVIC_SetPriority(USART3_4_IRQn, 2); // Set the priority of the USART3 interrupt to 2

  while (1) 
	{
		if(!flag) // Check if CMD? has already been sent
		{
			transmit_string("CMD?\n");
			flag = 1;
		}
		if (recv_flag) // Reset values if receive flag is on
		{
			flag = 0;
			recv_flag = 0;
			recv_val = 0;
			led = 0;
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
