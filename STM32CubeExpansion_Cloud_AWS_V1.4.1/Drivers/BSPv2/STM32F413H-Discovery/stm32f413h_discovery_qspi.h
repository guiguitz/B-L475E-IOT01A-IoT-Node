/**
  ******************************************************************************
  * @file    stm32f413h_discovery_qspi.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32f413h_discovery_qspi.c driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F413H_DISCOVERY
  * @{
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32F413H_DISCOVERY_QSPI_H
#define STM32F413H_DISCOVERY_QSPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f413h_discovery_conf.h"
#include "stm32f413h_discovery_errno.h"
#include "../Components/n25q128a/n25q128a.h"

/** @addtogroup STM32F413H_DISCOVERY_QSPI
  * @{
  */
/* Exported types ------------------------------------------------------------*/
/** @defgroup STM32F413H_DISCOVERY_QSPI_Exported_Types QSPI Exported Types
  * @{
  */
#define BSP_QSPI_Info_t                 N25Q128A_Info_t
#define BSP_QSPI_Interface_t            N25Q128A_Interface_t
#define BSP_QSPI_Transfer_t             N25Q128A_Transfer_t
#define BSP_QSPI_DualFlash_t            N25Q128A_DualFlash_t
#define BSP_QSPI_Erase_t                N25Q128A_Erase_t
#define BSP_QSPI_ODS_t                  N25Q128A_ODS_t

/* QSPI Info */
typedef struct
{
  uint32_t FlashSize;            /*!< Size of the flash */
  uint32_t EraseSectorSize;      /*!< Size of sectors for the erase operation */
  uint32_t EraseSectorsNumber;   /*!< Number of sectors for the erase operation */
  uint32_t ProgPageSize;         /*!< Size of pages for the program operation */
  uint32_t ProgPagesNumber;      /*!< Number of pages for the program operation */
} QSPI_Info_t;

typedef enum
{
  QSPI_ACCESS_NONE = 0,          /*!<  Instance not initialized,             */
  QSPI_ACCESS_INDIRECT,          /*!<  Instance use indirect mode access     */
  QSPI_ACCESS_MMP                /*!<  Instance use Memeoy Mapped Mode read  */
} QSPI_Access_t;

typedef struct
{
  QSPI_Access_t        IsInitialized;   /*!<  Instance access Flash method     */
  BSP_QSPI_Interface_t InterfaceMode;   /*!<  Flash Interface mode of Instance */
  BSP_QSPI_Transfer_t  TransferRate;    /*!<  Flash Transfer mode of Instance  */
  uint32_t             DualFlashMode;   /*!<  Flash dual mode                  */
  uint32_t             IsMspCallbacksValid;
} QSPI_Ctx_t;

typedef struct
{
  BSP_QSPI_Interface_t        InterfaceMode;   /*!<  Current Flash Interface mode */
  BSP_QSPI_Transfer_t         TransferRate;    /*!<  Current Flash Transfer mode  */
  BSP_QSPI_DualFlash_t        DualFlashMode;   /*!<  Dual Flash mode              */
} BSP_QSPI_Init_t;

typedef struct
{
  uint32_t FlashSize;
  uint32_t ClockPrescaler;
  uint32_t SampleShifting;
  uint32_t DualFlashMode;
}MX_QSPI_Config;

#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)
typedef struct
{
  pQSPI_CallbackTypeDef  pMspInitCb;
  pQSPI_CallbackTypeDef  pMspDeInitCb;
}BSP_QSPI_Cb_t;
#endif /* (USE_HAL_QSPI_REGISTER_CALLBACKS == 1) */

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup STM32F413H_DISCOVERY_QSPI_Exported_Constants QSPI Exported Constants
  * @{
  */
#define QSPI_INSTANCES_NUMBER         1U

/* Definition for QSPI modes */
#define BSP_QSPI_SPI_MODE            (BSP_QSPI_Interface_t)N25Q128A_SPI_MODE      /* 1 Cmd Line, 1 Address Line and 1 Data Line    */
#define BSP_QSPI_SPI_1I2O_MODE       (BSP_QSPI_Interface_t)N25Q128A_SPI_1I2O_MODE /* 1 Cmd Line, 1 Address Line and 2 Data Lines   */
#define BSP_QSPI_SPI_2IO_MODE        (BSP_QSPI_Interface_t)N25Q128A_SPI_2IO_MODE  /* 1 Cmd Line, 2 Address Lines and 2 Data Lines  */
#define BSP_QSPI_SPI_1I4O_MODE       (BSP_QSPI_Interface_t)N25Q128A_SPI_1I4O_MODE /* 1 Cmd Line, 1 Address Line and 4 Data Lines   */
#define BSP_QSPI_SPI_4IO_MODE        (BSP_QSPI_Interface_t)N25Q128A_SPI_4IO_MODE  /* 1 Cmd Line, 4 Address Lines and 4 Data Lines  */
#define BSP_QSPI_DPI_MODE            (BSP_QSPI_Interface_t)N25Q128A_DPI_MODE      /* 2 Cmd Lines, 2 Address Lines and 2 Data Lines */
#define BSP_QSPI_QPI_MODE            (BSP_QSPI_Interface_t)N25Q128A_QPI_MODE      /* 4 Cmd Lines, 4 Address Lines and 4 Data Lines */

/* Definition for QSPI transfer rates */
#define BSP_QSPI_STR_TRANSFER        (BSP_QSPI_Transfer_t)N25Q128A_STR_TRANSFER /* Single Transfer Rate */
#define BSP_QSPI_DTR_TRANSFER        (BSP_QSPI_Transfer_t)N25Q128A_DTR_TRANSFER /* Double Transfer Rate */

/* Definition for QSPI dual flash mode */
#define BSP_QSPI_DUALFLASH_ENABLE    (BSP_QSPI_DualFlash_t)N25Q128A_DUALFLASH_ENABLE    /* Dual flash mode disabled */
#define BSP_QSPI_DUALFLASH_DISABLE   (BSP_QSPI_DualFlash_t)N25Q128A_DUALFLASH_DISABLE   /* Dual flash mode enabled  */

/* Definition for QSPI Flash ID */
#define BSP_QSPI_FLASH_ID            QSPI_FLASH_ID_1

/* QSPI erase types */
#define BSP_QSPI_ERASE_4K            (N25Q128A_Erase_t)N25Q128A_ERASE_4K
#define BSP_QSPI_ERASE_64K           (N25Q128A_Erase_t)N25Q128A_ERASE_64K
#define BSP_QSPI_ERASE_CHIP          (N25Q128A_Erase_t)N25Q128A_ERASE_CHIP

/* QSPI block sizes */
#define BSP_QSPI_BLOCK_4K            N25Q128A_SECTOR_4K
#define BSP_QSPI_BLOCK_64K           N25Q128A_BLOCK_64K

/* Definition for QSPI clock resources */
#define QSPI_CLK_ENABLE()            __HAL_RCC_QSPI_CLK_ENABLE()
#define QSPI_CLK_DISABLE()           __HAL_RCC_QSPI_CLK_DISABLE()
#define QSPI_CS_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOG_CLK_ENABLE()
#define QSPI_CLK_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define QSPI_D0_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_D1_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_D2_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOE_CLK_ENABLE()
#define QSPI_D3_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()

#define QSPI_FORCE_RESET()           __HAL_RCC_QSPI_FORCE_RESET()
#define QSPI_RELEASE_RESET()         __HAL_RCC_QSPI_RELEASE_RESET()

/* Definition for QSPI Pins */
/* QSPI_CS */
#define QSPI_CS_PIN                  GPIO_PIN_6
#define QSPI_CS_GPIO_PORT            GPIOG
#define QSPI_CS_PIN_AF               GPIO_AF10_QSPI
/* QSPI_CLK */
#define QSPI_CLK_PIN                 GPIO_PIN_2
#define QSPI_CLK_GPIO_PORT           GPIOB
#define QSPI_CLK_PIN_AF              GPIO_AF9_QSPI
/* QSPI_D0 */
#define QSPI_D0_PIN                  GPIO_PIN_8
#define QSPI_D0_GPIO_PORT            GPIOF
#define QSPI_D0_PIN_AF               GPIO_AF10_QSPI
/* QSPI_D1 */
#define QSPI_D1_PIN                  GPIO_PIN_9
#define QSPI_D1_GPIO_PORT            GPIOF
#define QSPI_D1_PIN_AF               GPIO_AF10_QSPI
/* QSPI_D2 */
#define QSPI_D2_PIN                  GPIO_PIN_2
#define QSPI_D2_GPIO_PORT            GPIOE
#define QSPI_D2_PIN_AF               GPIO_AF9_QSPI
/* QSPI_D3 */
#define QSPI_D3_PIN                  GPIO_PIN_13
#define QSPI_D3_GPIO_PORT            GPIOD
#define QSPI_D3_PIN_AF               GPIO_AF9_QSPI
/**
  * @}
  */

/** @addtogroup STM32F413H_DISCOVERY_QSPI_Exported_Variables
  * @{
  */
extern QSPI_HandleTypeDef hqspi[];
extern QSPI_Ctx_t Qspi_Ctx[];
/**
  * @}
  */

/** @addtogroup STM32F413H_DISCOVERY_QSPI_Exported_Functions
  * @{
  */
int32_t BSP_QSPI_Init(uint32_t Instance, BSP_QSPI_Init_t *Init);
int32_t BSP_QSPI_DeInit(uint32_t Instance);
#if (USE_HAL_QSPI_REGISTER_CALLBACKS == 1)
int32_t BSP_QSPI_RegisterMspCallbacks (uint32_t Instance, BSP_QSPI_Cb_t *CallBacks);
int32_t BSP_QSPI_RegisterDefaultMspCallbacks (uint32_t Instance);
#endif /* (USE_HAL_QSPI_REGISTER_CALLBACKS == 1) */
int32_t BSP_QSPI_Read(uint32_t Instance, uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
int32_t BSP_QSPI_Write(uint32_t Instance, uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
int32_t BSP_QSPI_EraseBlock(uint32_t Instance, uint32_t BlockAddress, BSP_QSPI_Erase_t BlockSize);
int32_t BSP_QSPI_EraseChip(uint32_t Instance);
int32_t BSP_QSPI_GetStatus(uint32_t Instance);
int32_t BSP_QSPI_GetInfo(uint32_t Instance, BSP_QSPI_Info_t *pInfo);
int32_t BSP_QSPI_EnableMemoryMappedMode(uint32_t Instance);
int32_t BSP_QSPI_DisableMemoryMappedMode(uint32_t Instance);
int32_t BSP_QSPI_ReadID(uint32_t Instance, uint8_t *Id);
int32_t BSP_QSPI_ConfigFlash(uint32_t Instance, BSP_QSPI_Interface_t Mode, BSP_QSPI_Transfer_t Rate);
int32_t BSP_QSPI_SelectFlashID(uint32_t Instance, uint32_t FlashID);

/* These functions can be modified in case the current settings
   need to be changed for specific application needs */
HAL_StatusTypeDef MX_QSPI_Init(QSPI_HandleTypeDef *hqspi, MX_QSPI_Config *Config);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32F413H_DISCOVERY_QSPI_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
