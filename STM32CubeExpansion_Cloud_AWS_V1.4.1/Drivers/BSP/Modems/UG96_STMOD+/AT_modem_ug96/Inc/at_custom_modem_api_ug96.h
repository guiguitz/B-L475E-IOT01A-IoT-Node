#ifdef USE_MODEM_UG96
/**
  ******************************************************************************
  * @file    at_custom_modem_api.h
  * @author  MCD Application Team
  * @brief   Header for at_custom_modem_api.c module for UG96
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
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
#ifndef AT_CUSTOM_MODEM_API_H
#define AT_CUSTOM_MODEM_API_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "at_core.h"
#include "at_modem_api.h"
#include "sysctrl.h"

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void atcma_init_at_func_ptrs(atcustom_funcPtrs_t *funcPtrs);
void atcma_init_sysctrl_func_ptrs(sysctrl_funcPtrs_t *funcPtrs);

#ifdef __cplusplus
}
#endif

#endif /* AT_CUSTOM_MODEM_API_H */

#endif /* USE_MODEM_UG96 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

