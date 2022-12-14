/**
  ******************************************************************************
  * @file    sfu_mpu_isolation.h
  * @author  MCD Application Team
  * @brief   SFU MPU isolation primitives
  *          This file provides functions to manage the MPU isolation of the Secure Engine.
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
#ifndef SFU_MPU_ISOLATION_H
#define SFU_MPU_ISOLATION_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sfu_def.h"
/** @addtogroup SFU Secure Boot / Secure Firmware Update
  * @{
  */

/** @addtogroup SFU_CORE SBSFU Application
  * @{
  */

/** @addtogroup SFU_IMG_MPU_ISOLATION
  * @{
  */


/** @defgroup SFU_IMG_MPU_ISOLATION_Types MPU Isolation types
 *  @brief Types used by the MPU isolation functions.
  * @{
  */
/**
 *  @brief SFU BOOT Privileged operation identifier.
 *         SB_SFU can trigger a SB_SFU privileged operation by calling a syscall (@ref SFU_MPU_SysCall) with one of the values of this enum.
 */
typedef enum
{
  SB_SYSCALL_LAUNCH_APP = 1, /*!< SB_SFU service call identifier: launch the user application */
  SB_SYSCALL_RESET         /*!< SB_SFU service call identifier: system reset */
} SFU_MPU_PrivilegedOpId;

/**
  * @}
  */

/** @addtogroup SFU_BOOT_MPUIsolation_Functions
  * @{
  */

/* SVC call handler for Secure Engine: this function is defined in se_interface_common.c */
extern void SE_SVC_Handler(uint32_t *args);

/* SB_SFU SVC handling */
void MPU_SVC_Handler(uint32_t *args);
void SFU_MPU_SVC_Handler(uint32_t *args);
void SFU_MPU_SysCall(uint32_t syscall, ...);

/* Helpers for unprivileged mode handling */
void SFU_MPU_EnterUnprivilegedMode(void);
uint32_t SFU_MPU_IsUnprivileged(void);

/**
 * @brief This function sets the Stack Pointer and Program Counter to jump into a function
 * @param  vector_address Start Address (where to jump)
 * @retval void
 */
void jump_to_function(uint8_t *vector_address);

/**
 * @brief This function jump into a function to set sticky bit and to jump into application function
 * @param  Function Address (where to jump)
 * @retval void
 * @note: see startup_stm32l476xx.s
 */
void exit_sticky(uint8_t *address);

/**
 * @brief This function returns from an interrupt and launches the user application code
 * @param  applicationVectorAddress Start address of the user application (slot0+offset: vectors)
 * @param  exitFunctionAddress Function to jump into the user application code
 * @param  address Function to jump into the BL code in case of exit_sticky usage
 * @param  magic number used by BL code in case of exit_sticky usage
 * @retval void
 */
void launch_application(uint32_t applicationVectorAddress, uint32_t exitFunctionAddress, uint32_t address, uint32_t magic);

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

#endif /* SFU_MPU_ISOLATION_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

