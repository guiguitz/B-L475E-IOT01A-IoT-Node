/**
  ******************************************************************************
  * @file    se_utils.c
  * @author  MCD Application Team
  * @brief   Secure Engine UTILS module.
  *          This file provides set of firmware functions to manage SE Utils
  *          functionalities.
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

/* Includes ------------------------------------------------------------------*/
#include "stddef.h"
#include "se_low_level.h"
#include "se_utils.h"

/** @addtogroup SE Secure Engine
  * @{
  */

/** @addtogroup SE_CORE SE Core
  * @{
  */

/** @defgroup  SE_UTILS SE Utils
  * @{
  */

/** @defgroup SE_UTILS_Private_Variables Private Variables
  * @{
  */
static uint32_t m_uSE_SystemCoreClock;

/**
  * @}
  */

/** @defgroup SE_UTILS_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief Set Secure Engine SystemCoreClock variable.
  * @param uSystemCoreClock Secure Engine SystemCoreClock variable.
  * @retval None.
  */
void SE_SetSystemCoreClock(uint32_t uSystemCoreClock)
{
  m_uSE_SystemCoreClock = uSystemCoreClock;
}

/**
  * @brief Provide a tick value in millisecond.
  * @note This function overwrites the one declared as __weak in HAL.
  *       In this implementation, time is counted without using SysTick timer interrupts.
  * @retval tick value
  */
uint32_t HAL_GetTick(void)
{
  static uint32_t m_uTick = 0U;
  static uint32_t t1 = 0U, tdelta = 0U;
  uint32_t t2;

  t2 =  SysTick->VAL;

  if (t2 <= t1)
  {
    tdelta += t1 - t2;
  }
  else
  {
    tdelta += t1 + SysTick->LOAD - t2;
  }

  if (tdelta > (m_uSE_SystemCoreClock / 1000U))
  {
    tdelta = 0U;
    m_uTick ++;
  }

  t1 = t2;
  return m_uTick;
}

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
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
