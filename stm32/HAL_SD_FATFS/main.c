/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "fatfs.h"
#include "sdio.h"
#include "usart.h"
#include "gpio.h"

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
//FATFS fs;													/* FatFs文件系统对象 */
//SDFile												/* 文件对象 */
FRESULT f_res;                    /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[]= "今天是个好日子，原标题：唐德诉高云翔案庭审结束，张起淮透露其澳洲案情：法院无任何证据10月8日，唐德影视晒出与天猫技术签订的合作协议，宣布此前受主演负面事件影响，未能播出的电视剧《赢天下》将启动重拍，高云翔和范冰冰将全部被另行指定的“一线演员”顶替。重拍将通过重新布景拍摄、技术处理、重新配音等方式进行，保证原有的观赏性和表达方式不受影响，预计总花费将超过6000万元。\n";/* 写缓冲区*/  

static void printf_fatfs_error(FRESULT fresult);

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
  MX_SDIO_SD_Init();
  MX_USART3_UART_Init();
  MX_FATFS_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	printf("OK");
	if(retSD == 0)
  {
	//在SD卡挂载文件系统，文件系统挂载时会对SD卡初始化
	f_res = f_mount(&SDFatFS,(TCHAR const*)SDPath,1);
	printf_fatfs_error(f_res);
    /*----------------------- 格式化测试 ---------------------------*/  
    /* 如果没有文件系统就格式化创建创建文件系统 */
    if(f_res == FR_NO_FILESYSTEM)
    {
      printf("》SD卡还没有文件系统，即将进行格式化...\r\n");
      /* 格式化 */
      f_res=f_mkfs((TCHAR const*)SDPath,0,0);							
      
      if(f_res == FR_OK)
      {
        printf("》SD卡已成功格式化文件系统。\r\n");
        /* 格式化后，先取消挂载 */
        f_res = f_mount(NULL,(TCHAR const*)SDPath,1);			
        /* 重新挂载	*/			
        f_res = f_mount(&SDFatFS,(TCHAR const*)SDPath,1);
      }
      else
      {
        printf("《《格式化失败。》》\r\n");
        while(1);
      }
    }
    else if(f_res!=FR_OK)
    {
      printf("！！SD卡挂载文件系统失败。(%d)\r\n",f_res);
      printf_fatfs_error(f_res);
      while(1);
    }
    else
    {
      printf("》文件系统挂载成功，可以进行读写测试\r\n");
    }
    
    /*----------------------- 文件系统测试：写测试 -----------------------------*/
    /* 打开文件，如果文件不存在则创建它 */
    printf("****** 即将进行文件写入测试... ******\r\n");	
    f_res = f_open(&SDFile, "FatFs读写测试文件.txt",FA_CREATE_ALWAYS | FA_WRITE );
    if ( f_res == FR_OK )
    {
      printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
      /* 将指定存储区内容写入到文件内 */
      f_res=f_write(&SDFile,WriteBuffer,sizeof(WriteBuffer),&fnum);
      if(f_res==FR_OK)
      {
        printf("》文件写入成功，写入字节数据：%d\r\n",fnum);
        printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
      }
      else
      {
        printf("！！文件写入失败：(%d)\r\n",f_res);
      }    
      /* 不再读写，关闭文件 */
      f_close(&SDFile);
    }
    else
    {	
      printf("！！打开/创建文件失败。\r\n");
    }
    
    /*------------------- 文件系统测试：读测试 ------------------------------------*/
    printf("****** 即将进行文件读取测试... ******\r\n");
    f_res = f_open(&SDFile, "FatFs读写测试文件.txt", FA_OPEN_EXISTING | FA_READ); 	 
    if(f_res == FR_OK)
    {
      printf("》打开文件成功。\r\n");
      f_res = f_read(&SDFile, ReadBuffer, sizeof(ReadBuffer), &fnum); 
      if(f_res==FR_OK)
      {
        printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
        printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);	
      }
      else
      {
        printf("！！文件读取失败：(%d)\r\n",f_res);
      }		
    }
    else
    {
      printf("！！打开文件失败。\r\n");
    }
    /* 不再读写，关闭文件 */
    f_close(&SDFile);
    
    /* 不再使用，取消挂载 */
    f_res = f_mount(NULL,(TCHAR const*)SDPath,1);	
  
  /* 注销一个FatFS设备：SD卡 */
    FATFS_UnLinkDriver(SDPath);
	}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Initializes the CPU, AHB and APB busses clocks 
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
  /** Initializes the CPU, AHB and APB busses clocks 
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
}

/* USER CODE BEGIN 4 */
static void printf_fatfs_error(FRESULT fresult)
{
  switch(fresult)
  {
    case FR_OK:                   //(0)
      printf("》操作成功。\r\n");
    break;
    case FR_DISK_ERR:             //(1)
      printf("！！硬件输入输出驱动出错。\r\n");
    break;
    case FR_INT_ERR:              //(2)
      printf("！！断言错误。\r\n");
    break;
    case FR_NOT_READY:            //(3)
      printf("！！物理设备无法工作。\r\n");
    break;
    case FR_NO_FILE:              //(4)
      printf("！！无法找到文件。\r\n");
    break;
    case FR_NO_PATH:              //(5)
      printf("！！无法找到路径。\r\n");
    break;
    case FR_INVALID_NAME:         //(6)
      printf("！！无效的路径名。\r\n");
    break;
    case FR_DENIED:               //(7)
    case FR_EXIST:                //(8)
      printf("！！拒绝访问。\r\n");
    break;
    case FR_INVALID_OBJECT:       //(9)
      printf("！！无效的文件或路径。\r\n");
    break;
    case FR_WRITE_PROTECTED:      //(10)
      printf("！！逻辑设备写保护。\r\n");
    break;
    case FR_INVALID_DRIVE:        //(11)
      printf("！！无效的逻辑设备。\r\n");
    break;
    case FR_NOT_ENABLED:          //(12)
      printf("！！无效的工作区。\r\n");
    break;
    case FR_NO_FILESYSTEM:        //(13)
      printf("！！无效的文件系统。\r\n");
    break;
    case FR_MKFS_ABORTED:         //(14)
      printf("！！因函数参数问题导致f_mkfs函数操作失败。\r\n");
    break;
    case FR_TIMEOUT:              //(15)
      printf("！！操作超时。\r\n");
    break;
    case FR_LOCKED:               //(16)
      printf("！！文件被保护。\r\n");
    break;
    case FR_NOT_ENOUGH_CORE:      //(17)
      printf("！！长文件名支持获取堆空间失败。\r\n");
    break;
    case FR_TOO_MANY_OPEN_FILES:  //(18)
      printf("！！打开太多文件。\r\n");
    break;
    case FR_INVALID_PARAMETER:    // (19)
      printf("！！参数无效。\r\n");
    break;
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
