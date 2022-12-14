/**
  ******************************************************************************
  * @file    se_low_level.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for Secure Engine low level interface.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SE_LOW_LEVEL_H
#define SE_LOW_LEVEL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "se_def.h"

/*  Tip: To avoid modifying this file each time you need to switch between these
        devices, you can define the device in your toolchain compiler preprocessor.
  */
#if !defined  (USE_HAL_DRIVER)
/*
 * Comment the line below if you will not use the peripherals drivers.
 * In this case, these drivers will not be included and the application code will
 * be based on direct access to peripherals registers
 */
#define USE_HAL_DRIVER */
#endif /* USE_HAL_DRIVER */

#if defined (USE_HAL_DRIVER)
#include "stm32familyxx_hal.h"
#endif /* USE_HAL_DRIVER */


/** @addtogroup SE
  * @{
  */

/** @addtogroup SE_HARDWARE
  * @{
  */

/** @defgroup SE_HARDWARE_Exported_Macros Exported Macros
  * @{
  */

/** @defgroup SE_HARDWARE_Exported_Macros_CodeIsolation Code Isolation Settings
  * @brief These settings configure the way the Secure Engine is isolated from the rest of the code.
  *        \li When using MPU isolation, only the SE code and data is isolated.
  *        \li When using Firewall isolation, the header of slot #0 and the swap area are also protected.
  * @{
  */
/* == Code isolation : FIREWALL example ==*/
/**
  * Set the FIREWALL pre arm bit to avoid reset when fetching non protected
  * code after exiting from protected code execution area
  */
#define ENTER_PROTECTED_AREA()   CLEAR_BIT(FIREWALL->CR, FW_CR_FPA)

/**
  * @brief Set FPA bit for a proper closure of the Firewall when exiting the protected function
  */
#define EXIT_PROTECTED_AREA() __HAL_FIREWALL_PREARM_ENABLE()


/* == Code isolation : MPU example ==*/
/**
 * This switch enables the Secure Engine isolation based on the MPU usage.
 * This switch must be enabled if the STM32 platform in use does not have a Firewall.
 */
// #define SFU_ISOLATE_SE_WITH_MPU

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup SE_HARDWARE_Exported_Functions
  * @{
  */

/** @addtogroup SE_HARDWARE_Exported_CRC_Functions
  * @{
  */
SE_ErrorStatus SE_LL_CRC_Config(void);
SE_ErrorStatus SE_LL_CRC_Init(void);
SE_ErrorStatus SE_LL_CRC_DeInit(void);
uint32_t SE_LL_CRC_Calculate(uint32_t pBuffer[], uint32_t uBufferLength);

/**
  * @}
  */

/** @addtogroup SE_HARDWARE_Exported_FLASH_Functions
  * @{
  */
SE_ErrorStatus SE_LL_FLASH_Erase(void *pStart, uint32_t Length);
SE_ErrorStatus SE_LL_FLASH_Write(void *pDestination, const void *pSource, uint32_t Length);
SE_ErrorStatus SE_LL_FLASH_Read(void *pDestination, const void *pSource, uint32_t Length);
void FLASH_IRQHandler(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* SE_LOW_LEVEL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

