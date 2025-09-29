/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include "dma.h"
#include "dma2d.h"
#include "ltdc.h"
#include "quadspi.h"
#include "rtc.h"
#include "sdmmc.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sdram.h"
#include "lcd_show.h"
#include "lcd_test.h"
#include "touch_800x480.h"
#include <complex.h>
#include <stdint.h>
#include "qspi_w25q64.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define W25Qxx_NumByteToTest 32 * 1024 // 测试数据的长度，64K

int32_t QSPI_Status; // 检测标志位

uint32_t W25Qxx_TestAddr = 0;                     // 测试地址
uint8_t W25Qxx_WriteBuffer[W25Qxx_NumByteToTest]; //	写数据数组
uint8_t W25Qxx_ReadBuffer[W25Qxx_NumByteToTest];
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */
int8_t QSPI_W25Qxx_Test(void) // Flash读写测试
{
    uint32_t i = 0; // 计数变量

    // 擦除
    QSPI_Status = QSPI_W25Qxx_BlockErase_32K(W25Qxx_TestAddr); // 擦除32K字节

    if (QSPI_Status == QSPI_W25Qxx_OK) {
        uint8_t erase_success[] = "\r\nerase success";
        HAL_UART_Transmit(&huart1, erase_success, sizeof(erase_success),
                          HAL_MAX_DELAY);
    } else {
        uint8_t erase_failed[] = "\r\nerase failed";
        HAL_UART_Transmit(&huart1, erase_failed, sizeof(erase_failed),
                          HAL_MAX_DELAY);
        while (1);
    }

    // 写入
    for (i = 0; i < W25Qxx_NumByteToTest; i++) // 先将数据写入数组
    {
        W25Qxx_WriteBuffer[i] = i;
    }

    QSPI_Status = QSPI_W25Qxx_WriteBuffer(W25Qxx_WriteBuffer, W25Qxx_TestAddr,
                                          W25Qxx_NumByteToTest); // 写入数据

    if (QSPI_Status == QSPI_W25Qxx_OK) {
        uint8_t write_success[] = "\r\nwrite success";
        HAL_UART_Transmit(&huart1, write_success, sizeof(write_success),
                          HAL_MAX_DELAY);
    } else {
        uint8_t write_failed[] = "\r\nwrite failed";
        HAL_UART_Transmit(&huart1, write_failed, sizeof(write_failed),
                          HAL_MAX_DELAY);
        while (1);
    }

    // 读取
    QSPI_Status = QSPI_W25Qxx_ReadBuffer(W25Qxx_ReadBuffer, W25Qxx_TestAddr,
                                         W25Qxx_NumByteToTest); // 读取数据

    if (QSPI_Status == QSPI_W25Qxx_OK) {
        uint8_t read_success[] = "\r\nread success";
        HAL_UART_Transmit(&huart1, read_success, sizeof(read_success),
                          HAL_MAX_DELAY);
    } else {
        uint8_t read_failed[] = "\r\nread failed";
        HAL_UART_Transmit(&huart1, read_failed, sizeof(read_failed),
                          HAL_MAX_DELAY);
        while (1);
    }

    // 数据校验
    for (i = 0; i < W25Qxx_NumByteToTest;
         i++) // 验证读出的数据是否等于写入的数据
    {
        if (W25Qxx_WriteBuffer[i] !=
            W25Qxx_ReadBuffer[i]) // 如果数据不相等，则返回0
        {
            uint8_t data_error[] = "\r\ndata error";
            HAL_UART_Transmit(&huart1, data_error, sizeof(data_error),
                              HAL_MAX_DELAY);
            while (1);
        }
    }
    uint8_t qspi_test_result[] = "\r\nQSPI_W25Qxx_Test success";
    HAL_UART_Transmit(&huart1, qspi_test_result, sizeof(qspi_test_result),
                      HAL_MAX_DELAY);

    // 读取整片Flash的数据0x20020000
    for (i = 0; i < W25Qxx_FlashSize / (W25Qxx_NumByteToTest);
         i++) // 每次读取 W25Qxx_NumByteToTest 字节的数据
    {
        QSPI_Status = QSPI_W25Qxx_ReadBuffer(W25Qxx_ReadBuffer, W25Qxx_TestAddr,
                                             W25Qxx_NumByteToTest);
        W25Qxx_TestAddr = W25Qxx_TestAddr + W25Qxx_NumByteToTest;
    }

    if (QSPI_Status == QSPI_W25Qxx_OK) {
        uint8_t read_success[] = "\r\nread success";
        HAL_UART_Transmit(&huart1, read_success, sizeof(read_success),
                          HAL_MAX_DELAY);
    } else {
        uint8_t read_failed[] = "\r\nread failed";
        HAL_UART_Transmit(&huart1, read_failed, sizeof(read_failed),
                          HAL_MAX_DELAY);
        while (1);
    }

    return QSPI_W25Qxx_OK; // 测试通过
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
__attribute__((section(".buffer"))) __attribute__((aligned(4))) uint8_t test[] =
    "0";
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MPU
     * Configuration--------------------------------------------------------*/
    MPU_Config();

    /* Enable the CPU Cache */

    /* Enable I-Cache---------------------------------------------------------*/
    SCB_EnableICache();

    /* Enable D-Cache---------------------------------------------------------*/
    SCB_EnableDCache();

    /* MCU
     * Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the
     * Systick. */
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
    MX_USART1_UART_Init();
    MX_FMC_Init();
    MX_DMA2D_Init();
    MX_LTDC_Init();
    MX_QUADSPI_Init();
    // MX_SDMMC1_SD_Init();
    MX_RTC_Init();
    /* USER CODE BEGIN 2 */
    // SDRAM_Initialization_Sequence(&hsdram1);
    LCD_RGB_Init();
    Touch_Init();
    QSPI_W25Qxx_Init(); // 初始化W25Q64
    // QSPI_W25Qxx_Test(); // Flash读写测试

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */

    while (1) {
        SCB_CleanDCache_by_Addr((uint32_t*)test, 1);
        HAL_UART_Transmit_DMA(&huart1, test, sizeof(test));
        test[0] += 1;
        HAL_Delay(1000);
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

    /** Supply configuration update enable
     */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

    /** Configure the main internal regulator output voltage
     */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType =
        RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState      = RCC_HSE_ON;
    RCC_OscInitStruct.LSIState      = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState  = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM      = 5;
    RCC_OscInitStruct.PLL.PLLN      = 192;
    RCC_OscInitStruct.PLL.PLLP      = 2;
    RCC_OscInitStruct.PLL.PLLQ      = 4;
    RCC_OscInitStruct.PLL.PLLR      = 2;
    RCC_OscInitStruct.PLL.PLLRGE    = RCC_PLL1VCIRANGE_2;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN  = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { Error_Handler(); }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                                  RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct = {0};

    /* Disables the MPU */
    HAL_MPU_Disable();

    /** Initializes and configures the Region and the memory to be protected
     */
    MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
    MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
    MPU_InitStruct.BaseAddress      = 0x0C000000;
    MPU_InitStruct.Size             = MPU_REGION_SIZE_32MB;
    MPU_InitStruct.SubRegionDisable = 0x0;
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
    MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
    MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /** Initializes and configures the Region and the memory to be protected
     */
    MPU_InitStruct.Number = MPU_REGION_NUMBER1;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    /* Enables the MPU */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state
     */
    __disable_irq();
    while (1) {}
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
    /* User can add his own implementation to report the file name and line
       number, ex: printf("test parameters value: file %s on line %d\r\n",
       file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
