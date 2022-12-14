/**
  ******************************************************************************
  * @file    com_sockets_statistic.c
  * @author  MCD Application Team
  * @brief   This file implements Socket statistic
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
#include "com_sockets_statistic.h"

#if (COM_SOCKETS_STATISTIC == 1U)

#include <string.h>
#include <stdio.h>

#include "cmsis_os_misrac2012.h"

/* Private defines -----------------------------------------------------------*/
#if ((USE_TRACE_COM_SOCKETS == 1U) || (USE_CMD_CONSOLE == 1U))
#if (USE_PRINTF == 0U)
#include "trace_interface.h"
#define PrintSTAT(format, args...) \
  TracePrint(DBG_CHAN_COM, DBL_LVL_P0, "" format "\n\r", ## args)
#else /* USE_PRINTF == 1U */
#define PrintSTAT(format, args...)  \
  printf("" format "\n\r", ## args);
#endif /* USE_PRINTF == 0U */
#endif /* (USE_TRACE_COM_SOCKETS == 1U) || (USE_CMD_CONSOLE == 1U) */

#if (USE_DATACACHE == 1)
#include "dc_common.h"
#include "dc_time.h"
#endif /* USE_DATACACHE == 1 */

/* Private typedef -----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint16_t sock_cre_ok;
  uint16_t sock_cre_nok;
  uint16_t sock_cnt_ok;
  uint16_t sock_cnt_nok;
  uint16_t sock_snd_ok;
  uint16_t sock_snd_nok;
  uint16_t sock_rcv_ok;
  uint16_t sock_rcv_nok;
  uint16_t sock_cls_ok;
  uint16_t sock_cls_nok;
#if (USE_DATACACHE == 1)
  uint16_t nwk_up;
  uint16_t nwk_dwn;
#endif /* USE_DATACACHE == 1 */
} com_socket_statistic_t;

/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

#if 0
#if ((USE_TRACE_COM_SOCKETS == 1U) || (USE_CMD_CONSOLE == 1U))
/* Statistic socket state readable print */
static const uint8_t *com_socket_state_string[] =
{
  "Invalid",
  "Creating",
  "Created",
  "Connected",
  "Sending",
  "Receiving",
  "Closing"
};
#endif /* (USE_TRACE_COM_SOCKETS == 1U) || (USE_CMD_CONSOLE == 1U) */
#endif /* not yet supported */

/* Statistic socket variable */
static com_socket_statistic_t com_socket_statistic;

/* Private typedef -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Callback prototype */
static void com_socket_statistic_timer_cb(void const *argument);

/* Private function Definition -----------------------------------------------*/

/**
  * @brief  Called when statistic timer raised
  * @note   Request com socket statistic print
  * @param  argument - parameter passed at creation of osTimer
  * @note   Unused
  * @retval None
  */
static void com_socket_statistic_timer_cb(void const *argument)
{
  UNUSED(argument);
  com_sockets_statistic_display();
}

/* Functions Definition ------------------------------------------------------*/

/**
  * @brief  Component initialization
  * @note   must be called only one time and
  *         before using any other functions of com_*
  * @param  None
  * @retval None
  */
void com_sockets_statistic_init(void)
{
  /* Statistic display timer */
  static osTimerId com_socket_statistic_timer_handle;

  (void)memset(&com_socket_statistic, 0, sizeof(com_socket_statistic_t));
  if (COM_SOCKETS_STATISTIC_PERIOD != 0U)
  {
    osTimerDef(com_socket_statistic_timer,
               com_socket_statistic_timer_cb);
    com_socket_statistic_timer_handle = osTimerCreate(osTimer(com_socket_statistic_timer),
                                                      osTimerPeriodic, NULL);
    (void)osTimerStart(com_socket_statistic_timer_handle,
                       (uint32_t)(COM_SOCKETS_STATISTIC_PERIOD * 60000U));
  }
}

/**
  * @brief  Managed com sockets statistic update
  * @note   -
  * @param  stat - to know what the function has to do
  * @note   statistic update
  * @retval None
  */
void com_sockets_statistic_update(com_sockets_stat_update_t stat)
{
  switch (stat)
  {
#if (USE_DATACACHE == 1)
    case COM_SOCKET_STAT_NWK_UP:
    {
      com_socket_statistic.nwk_up++;
      break;
    }
    case COM_SOCKET_STAT_NWK_DWN:
    {
      com_socket_statistic.nwk_dwn++;
      break;
    }
#endif /* USE_DATACACHE == 1 */
    case COM_SOCKET_STAT_CRE_OK:
    {
      com_socket_statistic.sock_cre_ok++;
      break;
    }
    case COM_SOCKET_STAT_CRE_NOK:
    {
      com_socket_statistic.sock_cre_nok++;
      break;
    }
    case COM_SOCKET_STAT_CNT_OK:
    {
      com_socket_statistic.sock_cnt_ok++;
      break;
    }
    case COM_SOCKET_STAT_CNT_NOK:
    {
      com_socket_statistic.sock_cnt_nok++;
      break;
    }
    case COM_SOCKET_STAT_SND_OK:
    {
      com_socket_statistic.sock_snd_ok++;
      break;
    }
    case COM_SOCKET_STAT_SND_NOK:
    {
      com_socket_statistic.sock_snd_nok++;
      break;
    }
    case COM_SOCKET_STAT_RCV_OK:
    {
      com_socket_statistic.sock_rcv_ok++;
      break;
    }
    case COM_SOCKET_STAT_RCV_NOK:
    {
      com_socket_statistic.sock_rcv_nok++;
      break;
    }
    case COM_SOCKET_STAT_CLS_OK:
    {
      com_socket_statistic.sock_cls_ok++;
      break;
    }
    case COM_SOCKET_STAT_CLS_NOK:
    {
      com_socket_statistic.sock_cls_nok++;
      break;
    }
    default:
    {
      break;
    }
  }
}

/**
  * @brief  Display com sockets statistics
  * @note   Request com sockets statistics display
  * @param  None
  * @retval None
  */
void com_sockets_statistic_display(void)
{
#if 0
  socket_desc_t *socket_desc;
  socket_desc = socket_desc_list;
#endif /* not yet supported */

  /* Check that at least one socket has run */
  if (com_socket_statistic.sock_cre_ok != 0U)
  {
    PrintSTAT("*** Socket Stat Begin ***")
#if (USE_DATACACHE == 1)
    dc_time_date_rt_info_t dc_time_date_rt_info;

    (void)dc_srv_get_time_date(&dc_time_date_rt_info,
                               DC_DATE_AND_TIME);

    PrintSTAT("%02ld/%02ld/%04ld - %02ld:%02ld:%02ld",
              dc_time_date_rt_info.mday,
              dc_time_date_rt_info.month,
              dc_time_date_rt_info.year,
              dc_time_date_rt_info.hour,
              dc_time_date_rt_info.min,
              dc_time_date_rt_info.sec)

    PrintSTAT("Nwk: up:%5d dwn:%5d tot:%6d",
              com_socket_statistic.nwk_up,
              com_socket_statistic.nwk_dwn,
              (com_socket_statistic.nwk_up + com_socket_statistic.nwk_dwn))
#endif /* USE_DATACACHE == 1 */
    PrintSTAT("Cre: ok:%5d nok:%5d tot:%6d",
              com_socket_statistic.sock_cre_ok,
              com_socket_statistic.sock_cre_nok,
              (com_socket_statistic.sock_cre_ok + com_socket_statistic.sock_cre_nok))
    PrintSTAT("Con: ok:%5d nok:%5d tot:%6d",
              com_socket_statistic.sock_cnt_ok,
              com_socket_statistic.sock_cnt_nok,
              (com_socket_statistic.sock_cnt_ok + com_socket_statistic.sock_cnt_nok))
    PrintSTAT("Snd: ok:%5d nok:%5d tot:%6d",
              com_socket_statistic.sock_snd_ok,
              com_socket_statistic.sock_snd_nok,
              (com_socket_statistic.sock_snd_ok + com_socket_statistic.sock_snd_nok))
    PrintSTAT("Rcv: ok:%5d nok:%5d tot:%6d",
              com_socket_statistic.sock_rcv_ok,
              com_socket_statistic.sock_rcv_nok,
              (com_socket_statistic.sock_rcv_ok + com_socket_statistic.sock_rcv_nok))
    PrintSTAT("Cls: ok:%5d nok:%5d tot:%6d",
              com_socket_statistic.sock_cls_ok,
              com_socket_statistic.sock_cls_nok,
              (com_socket_statistic.sock_cls_ok + com_socket_statistic.sock_cls_nok))
#if 0
    while (socket_desc != NULL)
    {
      if (socket_desc->local == COM_SOCKETS_FALSE)
      {
        PrintSTAT("Sock: id:%3d-State:%s-Err:%d",
                  socket_desc->id,
                  com_socket_state_string[socket_desc->state],
                  socket_desc->error)
      }
      else
      {
        PrintSTAT("Ping: id:%3d-State:%s-Err:%d",
                  socket_desc->id,
                  com_socket_state_string[socket_desc->state],
                  socket_desc->error)
      }
      socket_desc = socket_desc->next;
    }
#endif /* not yet supported */
    PrintSTAT("*** Socket Stat End ***")
  }
}

#else /* COM_SOCKETS_STATISTIC == 0U */
/**
  * @brief  Component initialization
  * @note   must be called only one time and
  *         before using any other functions of com_*
  * @param  None
  * @retval None
  */
void com_sockets_statistic_init(void)
{
  /* Nothing to do */
}

/**
  * @brief  Managed com sockets statistic update and print
  * @note   -
  * @param  stat - to know what the function has to do
  * @note   statistic init, update or print
  * @retval None
  */
void com_sockets_statistic_update(com_sockets_stat_update_t stat)
{
  UNUSED(stat);
  /* Nothing to do */
}

/**
  * @brief  Display com sockets statistics
  * @note   Request com sockets statistics display
  * @param  None
  * @retval None
  */
void com_sockets_statistic_display(void)
{
  /* Nothing to do */
}

#endif /* COM_SOCKET_STATISTIC == 1U */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
