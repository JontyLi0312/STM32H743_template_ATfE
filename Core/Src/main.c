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
#include "stm32h7xx.h"
#include "stm32h7xx_hal_def.h"
#include "stm32h7xx_hal_uart.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd_show.h"
#include "lcd_test.h"
#include "lv_demo_music.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lvgl.h"
#include "map.h"
#include "qspi_w25q64.h"
#include "sdram.h"
#include "touch_800x480.h"
#include <stdint.h>

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
PLACE_IN_QSPI_SECTION const uint8_t qspi[]    = "\r\nQSPI";
PLACE_IN_QSPI_SECTION const uint8_t success[] = "\r\nOK";
PLACE_IN_QSPI_SECTION const uint8_t fail[]    = "\r\nFAIL";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */
uint8_t SDRAM_Test(void)
{
    uint32_t i        = 0; // 计数变量
    uint16_t ReadData = 0; // 读取到的数据
    uint8_t ReadData_8b;

    uint32_t ExecutionTime_Begin; // 开始时间
    uint32_t ExecutionTime_End;   // 结束时间
    uint32_t ExecutionTime;       // 执行时间
    float ExecutionSpeed;         // 执行速度

    // printf("\r\n***************************************************************"
    //        "**************************************\r\n");
    // printf("\r\n进行速度测试>>>\r\n");

    // 写入
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    ExecutionTime_Begin = HAL_GetTick(); // 获取 systick 当前时间，单位ms

    for (i = 0; i < SDRAM_Size / 2; i++) {
        *(__IO uint16_t *)(SDRAM_BANK_ADDR + 2 * i) = (uint16_t)i; // 写入数据
    }
    ExecutionTime_End = HAL_GetTick(); // 获取 systick 当前时间，单位ms
    ExecutionTime =
        ExecutionTime_End - ExecutionTime_Begin; // 计算擦除时间，单位ms
    ExecutionSpeed = (float)SDRAM_Size / 1024 / 1024 / ExecutionTime *
                     1000; // 计算速度，单位 MB/S

    // printf("\r\n以16位数据宽度写入数据，大小：%d MB，耗时: %d ms, "
    //        "写入速度：%.2f MB/s\r\n",
    //        SDRAM_Size / 1024 / 1024, ExecutionTime, ExecutionSpeed);

    // 读取
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    ExecutionTime_Begin = HAL_GetTick(); // 获取 systick 当前时间，单位ms

    for (i = 0; i < SDRAM_Size / 2; i++) {
        ReadData =
            *(__IO uint16_t *)(SDRAM_BANK_ADDR + 2 * i); // 从SDRAM读出数据
    }
    ExecutionTime_End = HAL_GetTick(); // 获取 systick 当前时间，单位ms
    ExecutionTime =
        ExecutionTime_End - ExecutionTime_Begin; // 计算擦除时间，单位ms
    ExecutionSpeed = (float)SDRAM_Size / 1024 / 1024 / ExecutionTime *
                     1000; // 计算速度，单位 MB/S

    // printf(
    //     "\r\n读取数据完毕，大小：%d MB，耗时: %d ms, 读取速度：%.2f
    //     MB/s\r\n", SDRAM_Size / 1024 / 1024, ExecutionTime, ExecutionSpeed);

    // 数据校验
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    // printf("\r\n***************************************************************"
    //        "**************************************\r\n");
    // printf("\r\n进行数据校验>>>\r\n");

    for (i = 0; i < SDRAM_Size / 2; i++) {
        ReadData =
            *(__IO uint16_t *)(SDRAM_BANK_ADDR + 2 * i); // 从SDRAM读出数据
        if (ReadData !=
            (uint16_t)i) // 检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            // printf("\r\nSDRAM测试失败！！\r\n");
            return ERROR; // 返回失败标志
        }
    }

    // printf("\r\n16位数据宽度读写通过，以8位数据宽度写入数据\r\n");
    for (i = 0; i < 255; i++) {
        *(__IO uint8_t *)(SDRAM_BANK_ADDR + i) = (uint8_t)i;
    }
    // printf("写入完毕，读取数据并比较...\r\n");
    for (i = 0; i < 255; i++) {
        ReadData_8b = *(__IO uint8_t *)(SDRAM_BANK_ADDR + i);
        if (ReadData_8b !=
            (uint8_t)i) // 检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            // printf("8位数据宽度读写测试失败！！\r\n");
            // printf("请检查NBL0和NBL1的连接\r\n");
            return ERROR; // 返回失败标志
        }
    }
    // printf("8位数据宽度读写通过\r\n");
    // printf("SDRAM读写测试通过，系统正常\r\n");
    return SUCCESS; // 返回成功标志
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
    MX_RTC_Init();
    // MX_SDMMC1_SD_Init();
    /* USER CODE BEGIN 2 */
    // SDRAM_Initialization_Sequence(&hsdram1);
    LCD_RGB_Init();
    Touch_Init();
    SDRAM_Initialization_Sequence(&hsdram1);
    if (QSPI_W25Qxx_Init() != QSPI_W25Qxx_OK) { Error_Handler(); }
    if (QSPI_W25Qxx_Reset() != QSPI_W25Qxx_OK) { Error_Handler(); }
    if (QSPI_W25Qxx_MemoryMappedMode() != QSPI_W25Qxx_OK) { Error_Handler(); }
    if (SDRAM_Test() == SUCCESS) {
        SCB_CleanDCache_by_Addr((uint32_t *)success, sizeof(success));
        HAL_UART_Transmit(&huart1, success, sizeof(success), HAL_MAX_DELAY);
    } else {
        SCB_CleanDCache_by_Addr((uint32_t *)fail, sizeof(fail));
        HAL_UART_Transmit(&huart1, fail, sizeof(fail), HAL_MAX_DELAY);
    }
    // lv_init();
    // lv_port_disp_init();
    // lv_port_indev_init();

    // lv_demo_music();

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        SCB_CleanDCache_by_Addr((uint32_t *)qspi, sizeof(qspi));
        HAL_UART_Transmit_DMA(&huart1, qspi, sizeof(qspi));
        // lv_task_handler();
        Touch_Scan();
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
