#ifdef USE_MODEM_UG96
/**
  ******************************************************************************
  * @file    plf_modem_config.h
  * @author  MCD Application Team
  * @brief   This file contains the modem configuration for UG96
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
#ifndef PLF_MODEM_CONFIG_H
#define PLF_MODEM_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* You can specify in project configuration the modem and hardware interface used.
*  If this is not specified, default configuration is specifed below.
*/
#if defined(HWREF_B_CELL_UG96)
/* already explicitly defined:
 * using HWREF_B_CELL_UG96 directly on STMOD+ connector
 */
#else
/* set default config */
#define HWREF_B_CELL_UG96
#endif /* HWREF_B_CELL_UG96 */

/* MODEM parameters */
//#define USE_MODEM_UG96
#define CONFIG_MODEM_UART_BAUDRATE (115200U)
#define CONFIG_MODEM_USE_STMOD_CONNECTOR

#define UDP_SERVICE_SUPPORTED                (1U)
#define CONFIG_MODEM_UDP_SERVICE_CONNECT_IP  ((uint8_t *)"127.0.0.1")
#define CONFIG_MODEM_MAX_SOCKET_TX_DATA_SIZE ((uint32_t)1460U)
#define CONFIG_MODEM_MAX_SOCKET_RX_DATA_SIZE ((uint32_t)1500U)

/* UART flow control settings */
#if defined(USER_FLAG_MODEM_FORCE_NO_FLOW_CTRL)
#define CONFIG_MODEM_UART_RTS_CTS  (0)
#elif defined(USER_FLAG_MODEM_FORCE_HW_FLOW_CTRL)
#define CONFIG_MODEM_UART_RTS_CTS  (1)
#else /* default FLOW CONTROL setting for UG96 */
#define CONFIG_MODEM_UART_RTS_CTS  (1)
#endif /* user flag for modem flow control */

/* PDP context parameters (for AT+CGDONT) BEGIN */
#define PDP_CONTEXT_DEFAULT_MODEM_CID          1   /* CID numeric value */
#define PDP_CONTEXT_DEFAULT_MODEM_CID_STRING  "1"  /* CID string value */
#define PDP_CONTEXT_DEFAULT_TYPE              "IP" /* defined in project config files */
#define PDP_CONTEXT_DEFAULT_APN               ""   /* defined in project config files */
/* PDP context parameters (for AT+CGDONT) END */

#ifdef __cplusplus
}
#endif

#endif /*_PLF_MODEM_CONFIG_H */

#endif /* USE_MODEM_UG96 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
