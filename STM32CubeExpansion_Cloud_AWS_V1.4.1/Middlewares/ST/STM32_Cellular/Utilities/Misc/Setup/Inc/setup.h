/**
  ******************************************************************************
  * @file    setup.h
  * @author  MCD Application Team
  * @brief   Header for setup.c module
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
#ifndef SETUP_H
#define SETUP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "menu_utils.h"

/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* List of application using setup configuration */
typedef enum
{
  SETUP_APPLI_HTTP_CLIENT = 1,
  SETUP_APPLI_TIMEDATE,
  SETUP_APPLI_LIVEOBJECTS,
  SETUP_APPLI_CST,
  SETUP_APPLI_PING_CLIENT,
  /* Must be the last item */
  SETUP_APPLI_MAX
} setup_appli_code_t;

/* Exported constants --------------------------------------------------------*/
/* type of application version */
typedef uint16_t setup_appli_version_t;

/* Exported functions ------------------------------------------------------- */

/* setup component init */
void setup_init(void);

/* setup component start */
void setup_start(void);

/* apply all active setup configurations */
void setup_apply(void);

/* display help of version parameter */
void setup_version_help(void);

int32_t setup_record(setup_appli_code_t code_appli,
                     setup_appli_version_t version_appli,
                     uint8_t *label_appli,
                     uint32_t (*setup_fnct)(void),
                     void (*dump_fnct)(void),
                     void (*help_fnct)(void),
                     uint8_t **default_config,
                     uint32_t default_config_size);

const uint8_t *setup_get_label_appli(setup_appli_code_t code_appli);


#ifdef __cplusplus
}
#endif

#endif /* SETUP_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

