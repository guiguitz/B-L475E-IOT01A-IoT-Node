/**
  ******************************************************************************
  * @file    at_custom_socket.c
  * @author  MCD Application Team
  * @brief   This file provides common socket code for the modems
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


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "at_modem_common.h"
#include "at_modem_signalling.h"
#include "at_modem_socket.h"
#include "at_datapack.h"
#include "at_util.h"
#include "sysctrl.h"
#include "plf_config.h"

/* Private typedef -----------------------------------------------------------*/
typedef char atcm_TYPE_CHAR_t;

/* Private defines -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/
#if (USE_TRACE_ATCUSTOM_MODEM == 1U)
#if (USE_PRINTF == 0U)
#include "trace_interface.h"
#define PrintINFO(format, args...) TracePrint(DBG_CHAN_ATCMD, DBL_LVL_P0, "ATCModem:" format "\n\r", ## args)
#define PrintDBG(format, args...)  TracePrint(DBG_CHAN_ATCMD, DBL_LVL_P1, "ATCModem:" format "\n\r", ## args)
#define PrintAPI(format, args...)  TracePrint(DBG_CHAN_ATCMD, DBL_LVL_P2, "ATCModem API:" format "\n\r", ## args)
#define PrintErr(format, args...)  TracePrint(DBG_CHAN_ATCMD, DBL_LVL_ERR, "ATCModem ERROR:" format "\n\r", ## args)
#define PrintBuf(pbuf, size)       TracePrintBufChar(DBG_CHAN_ATCMD, DBL_LVL_P1, (char *)pbuf, size);
#else
#define PrintINFO(format, args...)  printf("ATCModem:" format "\n\r", ## args);
#define PrintDBG(format, args...)   do {} while(0);
#define PrintAPI(format, args...)   do {} while(0);
#define PrintErr(format, args...)   printf("ATCModem ERROR:" format "\n\r", ## args);
#define PrintBuf(format, args...)   do {} while(0);
#endif /* USE_PRINTF */
#else
#define PrintINFO(format, args...)  do {} while(0);
#define PrintDBG(format, args...)   do {} while(0);
#define PrintAPI(format, args...)   do {} while(0);
#define PrintErr(format, args...)   do {} while(0);
#define PrintBuf(format, args...)   do {} while(0);
#endif /* USE_TRACE_ATCUSTOM_MODEM */

/* Private variables ---------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private function Definition -----------------------------------------------*/

/**
  * @brief  This function reserve a modem connection Id (ID shared between at-custom and the modem)
  *         to a socket handle (ID shared between upper layers and at-custom)
  */
at_status_t atcm_socket_reserve_modem_cid(atcustom_modem_context_t *p_modem_ctxt, socket_handle_t sockHandle)
{
  at_status_t retval = ATSTATUS_OK;

  if (sockHandle == CS_INVALID_SOCKET_HANDLE)
  {
    PrintINFO("socket handle %ld not valid", sockHandle)
    return (ATSTATUS_ERROR);
  }

  p_modem_ctxt->persist.socket[sockHandle].socket_connected = AT_FALSE;
  p_modem_ctxt->persist.socket[sockHandle].socket_data_pending_urc = AT_FALSE;
  p_modem_ctxt->persist.socket[sockHandle].socket_closed_pending_urc = AT_FALSE;

  return (retval);
}

/**
  * @brief  This function release a modem connection Id (ID shared between at-custom and the modem)
  *         to a socket handle (ID shared between upper layers and at-custom)
  */
at_status_t atcm_socket_release_modem_cid(atcustom_modem_context_t *p_modem_ctxt, socket_handle_t sockHandle)
{
  at_status_t retval = ATSTATUS_OK;

  if (sockHandle == CS_INVALID_SOCKET_HANDLE)
  {
    PrintINFO("socket handle %ld not valid", sockHandle)
    return (ATSTATUS_ERROR);
  }

  if ((p_modem_ctxt->persist.socket[sockHandle].socket_data_pending_urc == AT_TRUE) ||
      (p_modem_ctxt->persist.socket[sockHandle].socket_closed_pending_urc == AT_TRUE))
  {
    PrintINFO("Warning, there was pending URC for socket handle %ld: (%d)data pending urc,(%d) closed by remote urc",
              sockHandle,
              p_modem_ctxt->persist.socket[sockHandle].socket_data_pending_urc,
              p_modem_ctxt->persist.socket[sockHandle].socket_closed_pending_urc)
  }

  p_modem_ctxt->persist.socket[sockHandle].socket_connected = AT_FALSE;
  p_modem_ctxt->persist.socket[sockHandle].socket_data_pending_urc = AT_FALSE;
  p_modem_ctxt->persist.socket[sockHandle].socket_closed_pending_urc = AT_FALSE;

  return (retval);
}

/**
  * @brief  This function returns the modem connection Id (ID shared between at-custom and the modem)
  *         corresponding to a socket handle (ID shared between upper layers and at-custom)
  */
uint32_t atcm_socket_get_modem_cid(atcustom_modem_context_t *p_modem_ctxt, socket_handle_t sockHandle)
{
  uint32_t cid;

  if (sockHandle == CS_INVALID_SOCKET_HANDLE)
  {
    PrintINFO("socket handle %ld not valid", sockHandle)
    return (0U);
  }

  /* find  connectid correspondig to this socket_handle */
  cid = (uint32_t)(p_modem_ctxt->persist.socket[sockHandle].socket_connId_value);

  return (cid);
}

/**
  * @brief  This function affect a modem connection Id (ID shared between at-custom and the modem)
  *         to a socket handle (ID shared between upper layers and at-custom)
  *         It is used when the modem cid is affected by the modem.
  */
at_status_t atcm_socket_set_modem_cid(atcustom_modem_context_t *p_modem_ctxt, socket_handle_t sockHandle, uint32_t modemcid)
{
  at_status_t retval = ATSTATUS_OK;

  if (sockHandle == CS_INVALID_SOCKET_HANDLE)
  {
    PrintINFO("socket handle %ld not valid", sockHandle)
    return (ATSTATUS_ERROR);
  }

  p_modem_ctxt->persist.socket[sockHandle].socket_connId_value = modemcid;

  return (retval);
}

/**
  * @brief  This function returns the socket handle (ID shared between upper layers and at-custom)
  *         corresponding to modem connection Id (ID shared between at-custom and the modem)
  */
socket_handle_t atcm_socket_get_socket_handle(const atcustom_modem_context_t *p_modem_ctxt, uint32_t modemCID)
{
  socket_handle_t sockHandle = CS_INVALID_SOCKET_HANDLE;

  for (uint8_t i = 0U; i < CELLULAR_MAX_SOCKETS; i++)
  {
    const atcustom_persistent_SOCKET_context_t *p_tmp;
    p_tmp = &p_modem_ctxt->persist.socket[i];
    if (p_tmp->socket_connId_value == modemCID)
    {
      sockHandle = (socket_handle_t)i;
    }
  }

  if (sockHandle == CS_INVALID_SOCKET_HANDLE)
  {
    PrintINFO("Can not find valid socket handle for modem CID=%ld", modemCID)
  }

  return (sockHandle);
}

/**
  * @brief  This function set the "socket data received" URC for a
  *         socket handle (ID shared between upper layers and at-custom)
  */
at_status_t atcm_socket_set_urc_data_pending(atcustom_modem_context_t *p_modem_ctxt, socket_handle_t sockHandle)
{
  at_status_t retval = ATSTATUS_OK;

  PrintAPI("enter atcm_socket_set_urc_data_pending sockHandle=%ld", sockHandle)

  if (sockHandle != CS_INVALID_SOCKET_HANDLE)
  {
    p_modem_ctxt->persist.socket[sockHandle].socket_data_pending_urc = AT_TRUE;
    p_modem_ctxt->persist.urc_avail_socket_data_pending = AT_TRUE;
  }
  else
  {
    retval = ATSTATUS_ERROR;
  }

  return (retval);
}

/**
  * @brief  This function set the "socket closed by remote" URC for a
  *         socket handle (ID shared between upper layers and at-custom)
  */
at_status_t atcm_socket_set_urc_closed_by_remote(atcustom_modem_context_t *p_modem_ctxt, socket_handle_t sockHandle)
{
  at_status_t retval = ATSTATUS_OK;

  PrintAPI("enter atcm_socket_set_urc_closed_by_remote sockHandle=%ld", sockHandle)

  if (sockHandle != CS_INVALID_SOCKET_HANDLE)
  {
    p_modem_ctxt->persist.socket[sockHandle].socket_closed_pending_urc = AT_TRUE;
    p_modem_ctxt->persist.urc_avail_socket_closed_by_remote = AT_TRUE;
  }
  else
  {
    retval = ATSTATUS_ERROR;
  }

  return (retval);
}

/**
  * @brief  This function returns the socket handle of "socket data received" URC
  *         and clears it
  */
socket_handle_t atcm_socket_get_hdle_urc_data_pending(atcustom_modem_context_t *p_modem_ctxt)
{
  socket_handle_t sockHandle = CS_INVALID_SOCKET_HANDLE;

  PrintAPI("enter atcm_socket_get_hdle_urc_data_pending")

  for (uint8_t i = 0U; i < CELLULAR_MAX_SOCKETS; i++)
  {
    atcustom_persistent_SOCKET_context_t *p_tmp;
    p_tmp = &p_modem_ctxt->persist.socket[i];
    if (p_tmp->socket_data_pending_urc == AT_TRUE)
    {
      sockHandle = (socket_handle_t)i;
      /* clear this URC */
      p_tmp->socket_data_pending_urc = AT_FALSE;
      break;
    }
  }

  return (sockHandle);
}

/**
  * @brief  This function returns the socket handle of "socket closed by remote" URC
  *         and clears it
  */
socket_handle_t atcm_socket_get_hdlr_urc_closed_by_remote(atcustom_modem_context_t *p_modem_ctxt)
{
  socket_handle_t sockHandle = CS_INVALID_SOCKET_HANDLE;

  PrintAPI("enter atcm_socket_get_hdlr_urc_closed_by_remote")

  for (uint8_t i = 0U; i < CELLULAR_MAX_SOCKETS; i++)
  {
    atcustom_persistent_SOCKET_context_t *p_tmp;
    p_tmp = &p_modem_ctxt->persist.socket[i];
    if (p_tmp->socket_closed_pending_urc == AT_TRUE)
    {
      sockHandle = (socket_handle_t)i;
      /* clear this URC */
      p_tmp->socket_closed_pending_urc = AT_FALSE;
      break;
    }
  }

  return (sockHandle);
}

/**
  * @brief  This function returns if there are pending "socket data received" URC
  */
at_bool_t atcm_socket_remaining_urc_data_pending(const atcustom_modem_context_t *p_modem_ctxt)
{
  PrintAPI("enter atcm_socket_remaining_urc_data_pending")
  at_bool_t remain = AT_FALSE;

  for (uint8_t i = 0U; i < CELLULAR_MAX_SOCKETS; i++)
  {
    const atcustom_persistent_SOCKET_context_t *p_tmp;
    p_tmp = &p_modem_ctxt->persist.socket[i];
    if (p_tmp->socket_data_pending_urc == AT_TRUE)
    {
      /* at least one remaining URC */
      remain = AT_TRUE;
      break;
    }
  }

  return (remain);
}

/**
  * @brief  This function returns if there are pending "socket closed by remote" URC
  */
at_bool_t atcm_socket_remaining_urc_closed_by_remote(const atcustom_modem_context_t *p_modem_ctxt)
{
  PrintAPI("enter atcm_socket_remaining_urc_closed_by_remote")

  at_bool_t remain = AT_FALSE;

  for (uint8_t i = 0U; i < CELLULAR_MAX_SOCKETS; i++)
  {
    const atcustom_persistent_SOCKET_context_t *p_tmp;
    p_tmp = &p_modem_ctxt->persist.socket[i];
    if (p_tmp->socket_closed_pending_urc == AT_TRUE)
    {
      /* at least one remaining URC */
      remain = AT_TRUE;
      break;
    }
  }

  return (remain);
}

at_bool_t atcm_socket_is_connected(const atcustom_modem_context_t *p_modem_ctxt, socket_handle_t sockHandle)
{
  at_bool_t retval = AT_FALSE;

  PrintAPI("enter atcm_socket_is_connected sockHandle=%ld", sockHandle)

  if (sockHandle != CS_INVALID_SOCKET_HANDLE)
  {
    if (p_modem_ctxt->persist.socket[sockHandle].socket_connected == AT_TRUE)
    {
      /* socket is currently connected */
      retval = AT_TRUE;
    }
  }

  return (retval);
}

at_status_t atcm_socket_set_connected(atcustom_modem_context_t *p_modem_ctxt, socket_handle_t sockHandle)
{
  at_status_t retval = ATSTATUS_OK;

  PrintAPI("enter atcm_socket_set_connected sockHandle=%ld", sockHandle)

  if (sockHandle != CS_INVALID_SOCKET_HANDLE)
  {
    p_modem_ctxt->persist.socket[sockHandle].socket_connected = AT_TRUE;
  }

  return (retval);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
