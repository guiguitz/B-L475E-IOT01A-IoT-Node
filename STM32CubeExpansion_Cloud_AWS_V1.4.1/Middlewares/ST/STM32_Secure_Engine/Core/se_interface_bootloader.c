/**
  ******************************************************************************
  * @file    se_interface_bootloader.c
  * @author  MCD Application Team
  * @brief   Secure Engine Interface module.
  *          This file provides set of firmware functions to manage SE Interface
  *          functionalities. These services are used by the bootloader.
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

/* Place code in a specific section*/
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".SE_IF_Code"
#elif defined(__CC_ARM)
#pragma arm section code = ".SE_IF_Code"
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"                       /* se_interface_bootloader.c is compiled in SBSFU project using main.h from this project */
#include "se_low_level.h"               /* This file is part of SE_CoreBin and adapts the Secure Engine (and its interface) to the STM32 board specificities */
#include "se_interface_common.h"
#include "se_callgate.h"
#include "se_interface_bootloader.h"
#include "se_intrinsics.h"


/*
*/
/** @addtogroup SE Secure Engine
  * @{
  */

/** @defgroup  SE_INTERFACE Secure Engine Interface
  * @{
  */

/** @defgroup  SE_INTERFACE_BOOTLOADER Secure Engine Interface for Bootloader
  * @brief Interface functions the Bootloader calls to use SE services.
  *        These APIs must remain crypto-agnostic.
  *  @note Please note that this code is compiled in the SB_SFU project (not in SE_CoreBin).
  *        Nevertheless, this file also includes "se_low_level.h" from SE_CoreBin to make sure the SE interface is in sync with the SE code.
  * @{
  *
  * "se_interface" is built and set in the context of the SB_SFU project.
  * Then some symbols are exported to the User Application for the User Application
  * to call some Secure Engine services through "se_interface"
  * (SB_SFU and the User Application are exclusive, they do not run in parallel).
  * As "se_interface" is set by SB_SFU but can be used by the User Application, it is important
  * not to introduce global variables if you do not want to create dependencies between the SB_SFU RAM mapping (SRAM1)
  * and the User Application RAM mapping.
  */

/*
 * DO NOT ADD ANY VARIABLE HERE, SEE EXPLANATIONS ABOVE
 * This file is dedicated to the bootloader so the problem should not occur.
 * Nevertheless, we keep the same rule for consistency.
 */

/** @defgroup SE_INTERFACE_BOOTLOADER_Exported_Functions Exported Functions
  * @brief These are the functions the bootloader can call to use the SE services.
  *        These functions must remain crypto-agnostic.
  * @{
  */

/**
  * @brief Secure Engine initialization function. This function initialize all the
  *        internals module of the Secure Engine.
  * @param peSE_Status Secure Engine Status.
  *        This parameter can be a value of @ref SE_Status_Structure_definition.
  * @retval SE_ErrorStatus SE_SUCCESS if successful, SE_ERROR otherwise.
  */
SE_ErrorStatus SE_Init(SE_StatusTypeDef *peSE_Status, uint32_t uSystemCoreClock)
{
  SE_ErrorStatus e_ret_status;

  /* Check if the call is coming from SFU code */
  __IS_SFU_RESERVED();

  /* Check the pointers allocation */
  if (peSE_Status == NULL)
  {
    return SE_ERROR;
  }

  /* Set the CallGate function pointer */
  SET_CALLGATE();

  /*Enter Secure Mode*/
  SE_EnterSecureMode();

  /*Secure Engine Call*/
  e_ret_status = (*SE_CallGatePtr)(SE_INIT_ID, peSE_Status, uSystemCoreClock);

  /*Exit Secure Mode*/
  SE_ExitSecureMode();


  return e_ret_status;
}

/**
  * @brief Secure Engine Startup function. This function initializes the SE Core binary.
  *        This function must be called only one time so an internal check is executed.
  * @param None.
  * @retval SE_ErrorStatus SE_SUCCESS if successful, SE_ERROR otherwise.
  */
SE_ErrorStatus SE_Startup(void)
{
  SE_ErrorStatus e_ret_status;
  static uint8_t b_startup_already_done = 0U;
  static SE_ErrorStatus(*SE_StartupPtr)(void);            /*!< Secure Engine STARTUP  pointer function*/

  /* Check if the call is coming from SFU code*/
  __IS_SFU_RESERVED();

  /* Need to be sure to execute this initialization only one time! */
  if (b_startup_already_done == 0U)
  {
    /*Secure Core Initialization */
    SE_StartupPtr = (SE_ErrorStatus(*)(void))((uint32_t) SE_STARTUP_REGION_ROM_START + 1U);
    e_ret_status = (*SE_StartupPtr)();
    if (e_ret_status == SE_SUCCESS)
    {
      b_startup_already_done = 1U;
    }
  }
  else
  {
    /* This function has been already successfully called */
    e_ret_status = SE_SUCCESS;
  }

  return e_ret_status;
}

/**
  * @brief call by SFU to read Info Boot inside the protected area.
  * @param peSE_Status Secure Engine Status.
  *        This parameter can be a value of @ref SE_Status_Structure_definition.
  * @param  pxSE_BootInfo BootInfo area pointer that will be filled.
  * @retval SE_ErrorStatus SE_SUCCESS if successful, SE_ERROR otherwise.
  */
SE_ErrorStatus SE_INFO_ReadBootInfo(SE_StatusTypeDef *peSE_Status, SE_BootInfoTypeDef *pxSE_BootInfo)
{
  SE_ErrorStatus e_ret_status;

  /* Check if the call is coming from SFU code*/
  __IS_SFU_RESERVED();

#ifdef SFU_ISOLATE_SE_WITH_MPU
  if (0 != SE_IsUnprivileged())
  {
    uint32_t params[2] = {(uint32_t)pxSE_BootInfo, (uint32_t)NULL};
    SE_SysCall(&e_ret_status, SE_BOOT_INFO_READ_ALL_ID, peSE_Status, &params);
  }
  else
  {
#endif /* SFU_ISOLATE_SE_WITH_MPU */
    /* Set the CallGate function pointer */
    SET_CALLGATE();

    /*Enter Secure Mode*/
    SE_EnterSecureMode();

    /*Secure Engine Call*/
    e_ret_status = (*SE_CallGatePtr)(SE_BOOT_INFO_READ_ALL_ID, peSE_Status, pxSE_BootInfo);

    /*Exit Secure Mode*/
    SE_ExitSecureMode();
#ifdef SFU_ISOLATE_SE_WITH_MPU
  }
#endif /* SFU_ISOLATE_SE_WITH_MPU */
  return e_ret_status;
}

/**
  * @brief  call by SFU to write Info Boot inside the protected area.
  * @param  peSE_Status Secure Engine Status.
  * @param  pxSE_BootInfo pxSE_BootInfo BootInfo area pointer.
  * @retval SE_ErrorStatus SE_SUCCESS if successful, SE_ERROR otherwise.
  */
SE_ErrorStatus SE_INFO_WriteBootInfo(SE_StatusTypeDef *peSE_Status, SE_BootInfoTypeDef *pxSE_BootInfo)
{
  SE_ErrorStatus e_ret_status;

  /* Check if the call is coming from SFU code*/
  __IS_SFU_RESERVED();

#ifdef SFU_ISOLATE_SE_WITH_MPU
  if (0 != SE_IsUnprivileged())
  {
    uint32_t params[2] = {(uint32_t)pxSE_BootInfo, (uint32_t)NULL};
    SE_SysCall(&e_ret_status, SE_BOOT_INFO_WRITE_ALL_ID, peSE_Status, &params);
  }
  else
  {
#endif /* SFU_ISOLATE_SE_WITH_MPU */
    /* Set the CallGate function pointer */
    SET_CALLGATE();

    /*Enter Secure Mode*/
    SE_EnterSecureMode();

    /*Secure Engine Call*/
    e_ret_status = (*SE_CallGatePtr)(SE_BOOT_INFO_WRITE_ALL_ID, peSE_Status, pxSE_BootInfo);

    /*Exit Secure Mode*/
    SE_ExitSecureMode();
#ifdef SFU_ISOLATE_SE_WITH_MPU
  }
#endif /* SFU_ISOLATE_SE_WITH_MPU */
  return e_ret_status;
}

/**
  * @brief call by SFU to lock part of Secure Engine services
  * @param  peSE_Status Secure Engine Status.
  * @retval SE_ErrorStatus SE_SUCCESS if successful, SE_ERROR otherwise.
  */
SE_ErrorStatus SE_LockRestrictServices(SE_StatusTypeDef *pSE_Status)
{
  SE_ErrorStatus e_ret_status;

  /* Check if the call is coming from SFU code */
  __IS_SFU_RESERVED();
#ifdef SFU_ISOLATE_SE_WITH_MPU
  if (0 != SE_IsUnprivileged())
  {
    SE_SysCall(&e_ret_status, SE_LOCK_RESTRICT_SERVICES, pSE_Status, NULL);
  }
  else
  {
#endif /* SFU_ISOLATE_SE_WITH_MPU */
    /* Set the CallGate function pointer */
    SET_CALLGATE();

    SE_EnterSecureMode();
    e_ret_status = (*SE_CallGatePtr)(SE_LOCK_RESTRICT_SERVICES, pSE_Status);
    SE_ExitSecureMode();
#ifdef SFU_ISOLATE_SE_WITH_MPU
  }
#endif /* SFU_ISOLATE_SE_WITH_MPU */
  return e_ret_status;
}

/**
  * @brief call by SFU to read flash protected area
  * @param  peSE_Status: Secure Engine Status.
  * @param  pDestination: pointer to output buffer.
  * @param  pSource: pointer to the flash area to read.
  * @param  Length: number of bytes to read from flash
  * @retval SE_ErrorStatus SE_SUCCESS if successful, SE_ERROR otherwise.
  */
SE_ErrorStatus SE_SFU_IMG_Read(SE_StatusTypeDef *pSE_Status, void *pDestination, const void *pSource, uint32_t Length)
{
  SE_ErrorStatus e_ret_status;

  /* Check if the call is coming from SFU code*/
  __IS_SFU_RESERVED();
#ifdef SFU_ISOLATE_SE_WITH_MPU
  if (0 != SE_IsUnprivileged())
  {

    uint32_t params[3] = {(uint32_t)pDestination, (uint32_t)pSource, (uint32_t)Length};
    SE_SysCall(&e_ret_status, SE_IMG_READ, pSE_Status, &params);
  }
  else
  {
#endif /* SFU_ISOLATE_SE_WITH_MPU */
    /* Set the CallGate function pointer */
    SET_CALLGATE();

    /*Enter Secure Mode*/
    SE_EnterSecureMode();

    /*Secure Engine Call*/
    e_ret_status = (*SE_CallGatePtr)(SE_IMG_READ, pSE_Status, pDestination, pSource, Length);;

    /*Exit Secure Mode*/
    SE_ExitSecureMode();
#ifdef SFU_ISOLATE_SE_WITH_MPU
  }
#endif /* SFU_ISOLATE_SE_WITH_MPU */
  return e_ret_status;
}

/**
  * @brief call by SFU to write a flash protected area
  * @param  peSE_Status: Secure Engine Status.
  * @param  pDestination: pointer to flash area to write.
  * @param  pSource: pointer to input buffer.
  * @param  Length: number of bytes to write to flash
  * @retval SE_ErrorStatus SE_SUCCESS if successful, SE_ERROR otherwise.
  */
SE_ErrorStatus SE_SFU_IMG_Write(SE_StatusTypeDef *pSE_Status, void *pDestination, const void *pSource, uint32_t Length)
{
  SE_ErrorStatus e_ret_status;

  /* Check if the call is coming from SFU code*/
  __IS_SFU_RESERVED();

#ifdef SFU_ISOLATE_SE_WITH_MPU
  if (0 != SE_IsUnprivileged())
  {
    uint32_t params[3] = {(uint32_t)pDestination, (uint32_t)pSource, (uint32_t)Length};
    SE_SysCall(&e_ret_status, SE_IMG_WRITE, pSE_Status, &params);
  }
  else
  {
#endif /* SFU_ISOLATE_SE_WITH_MPU */
    /* Set the CallGate function pointer */
    SET_CALLGATE();

    SE_EnterSecureMode();
    e_ret_status = (*SE_CallGatePtr)(SE_IMG_WRITE, pSE_Status, pDestination, pSource, Length);
    SE_ExitSecureMode();
#ifdef SFU_ISOLATE_SE_WITH_MPU
  }
#endif /* SFU_ISOLATE_SE_WITH_MPU */
  return e_ret_status;
}

/**
  * @brief Secure Engine Encrypt Init function.
  *        It is a wrapper of Encrypt_Init function included in the protected area.
  * @param peSE_Status Secure Engine Status.
  *        This parameter can be a value of @ref SE_Status_Structure_definition.
  * @param pxSE_Metadata Metadata that will be used to fill the Crypto Init structure.
  * @retval SE_ErrorStatus SE_SUCCESS if successful, SE_ERROR otherwise.
  */
SE_ErrorStatus SE_Encrypt_Init(SE_StatusTypeDef *peSE_Status, SE_FwRawHeaderTypeDef *pxSE_Metadata)
{

  SE_ErrorStatus e_ret_status;

  /* Check if the call is coming from SFU code*/
  __IS_SFU_RESERVED();

  /* Check the pointers allocation */
  if ((peSE_Status == NULL) || (pxSE_Metadata == NULL))
  {
    return SE_ERROR;
  }
#ifdef SFU_ISOLATE_SE_WITH_MPU
  if (0 != SE_IsUnprivileged())
  {
    uint32_t params[1] = {(uint32_t)pxSE_Metadata};
    SE_SysCall(&e_ret_status, SE_CRYPTO_LL_ENCRYPT_INIT_ID, peSE_Status, &params);
  }
  else
  {
#endif /* SFU_ISOLATE_SE_WITH_MPU */
    /* Set the CallGate function pointer */
    SET_CALLGATE();

    /*Enter Secure Mode*/
    SE_EnterSecureMode();

    /*Secure Engine Call*/
    e_ret_status = (*SE_CallGatePtr)(SE_CRYPTO_LL_ENCRYPT_INIT_ID, peSE_Status, pxSE_Metadata);

    /*Exit Secure Mode*/
    SE_ExitSecureMode();
#ifdef SFU_ISOLATE_SE_WITH_MPU
  }
#endif /* SFU_ISOLATE_SE_WITH_MPU */
  return e_ret_status;
}

/**
  * @brief Secure Engine Encrypt Append function.
  *        It is a wrapper of Encrypt_Append function included in the protected area.
  * @param peSE_Status Secure Engine Status.
  *        This parameter can be a value of @ref SE_Status_Structure_definition.
  * @param pInputBuffer pointer to Input Buffer.
  * @param uInputSize Input Size (bytes).
  * @param pOutputBuffer pointer to Output Buffer.
  * @param puOutputSize pointer to Output Size (bytes).
  * @retval SE_ErrorStatus SE_SUCCESS if successful, SE_ERROR otherwise.
  */
SE_ErrorStatus SE_Encrypt_Append(SE_StatusTypeDef *peSE_Status, const uint8_t *pInputBuffer, int32_t InputSize,    uint8_t *pOutputBuffer, int32_t *pOutputSize)
{

  SE_ErrorStatus e_ret_status;

  /* Check if the call is coming from SFU code*/
  __IS_SFU_RESERVED();

#ifdef SFU_ISOLATE_SE_WITH_MPU
  if (0 != SE_IsUnprivileged())
  {
    uint32_t params[4] = {(uint32_t)pInputBuffer, (uint32_t)InputSize, (uint32_t)pOutputBuffer, (uint32_t)pOutputSize};
    SE_SysCall(&e_ret_status, SE_CRYPTO_LL_ENCRYPT_APPEND_ID, peSE_Status, &params);
  }
  else
  {
#endif /* SFU_ISOLATE_SE_WITH_MPU */
    /* Set the CallGate function pointer */
    SET_CALLGATE();

    /*Enter Secure Mode*/
    SE_EnterSecureMode();

    /*Secure Engine Call*/
    e_ret_status = (*SE_CallGatePtr)(SE_CRYPTO_LL_ENCRYPT_APPEND_ID, peSE_Status, pInputBuffer, InputSize, pOutputBuffer, pOutputSize);

    /*Exit Secure Mode*/
    SE_ExitSecureMode();
#ifdef SFU_ISOLATE_SE_WITH_MPU
  }
#endif /* SFU_ISOLATE_SE_WITH_MPU */
  return e_ret_status;
}

/**
  * @brief Secure Engine Encrypt Finish function.
  *        It is a wrapper of Encrypt_Finish function included in the protected area.
  * @param peSE_Status Secure Engine Status.
  *        This parameter can be a value of @ref SE_Status_Structure_definition.
  * @param pOutputBuffer pointer to Output Buffer.
  * @param puOutputSize pointer to Output Size (bytes).
  * @retval SE_ErrorStatus SE_SUCCESS if successful, SE_ERROR otherwise.
  */
SE_ErrorStatus SE_Encrypt_Finish(SE_StatusTypeDef *peSE_Status, uint8_t *pOutputBuffer, int32_t *pOutputSize)
{

  SE_ErrorStatus e_ret_status;

  /* Check if the call is coming from SFU code*/
  __IS_SFU_RESERVED();

#ifdef SFU_ISOLATE_SE_WITH_MPU
  if (0 != SE_IsUnprivileged())
  {
    uint32_t params[2] = {(uint32_t)pOutputBuffer, (uint32_t)pOutputSize};
    SE_SysCall(&e_ret_status, SE_CRYPTO_LL_ENCRYPT_FINISH_ID, peSE_Status, &params);
  }
  else
  {
#endif /* SFU_ISOLATE_SE_WITH_MPU */
    /* Set the CallGate function pointer */
    SET_CALLGATE();

    /*Enter Secure Mode*/
    SE_EnterSecureMode();

    /*Secure Engine Call*/
    e_ret_status = (*SE_CallGatePtr)(SE_CRYPTO_LL_ENCRYPT_FINISH_ID, peSE_Status, pOutputBuffer, pOutputSize);

    /*Exit Secure Mode*/
    SE_ExitSecureMode();
#ifdef SFU_ISOLATE_SE_WITH_MPU
  }
#endif /* SFU_ISOLATE_SE_WITH_MPU */
  return e_ret_status;
}

/**
  * @brief Secure Engine Decrypt Init function.
  *        It is a wrapper of Decrypt_Init function included in the protected area.
  * @param peSE_Status Secure Engine Status.
  *        This parameter can be a value of @ref SE_Status_Structure_definition.
  * @param pxSE_Metadata Metadata that will be used to fill the Crypto Init structure.
  * @retval SE_ErrorStatus SE_SUCCESS if successful, SE_ERROR otherwise.
  */
SE_ErrorStatus SE_Decrypt_Init(SE_StatusTypeDef *peSE_Status, SE_FwRawHeaderTypeDef *pxSE_Metadata)
{

  SE_ErrorStatus e_ret_status;

  /* Check if the call is coming from SFU code*/
  __IS_SFU_RESERVED();

#ifdef SFU_ISOLATE_SE_WITH_MPU
  if (0 != SE_IsUnprivileged())
  {
    uint32_t params[1] = {(uint32_t)pxSE_Metadata};
    SE_SysCall(&e_ret_status, SE_CRYPTO_LL_DECRYPT_INIT_ID, peSE_Status, &params);
  }
  else
  {
#endif /* SFU_ISOLATE_SE_WITH_MPU */
    /* Set the CallGate function pointer */
    SET_CALLGATE();

    /*Enter Secure Mode*/
    SE_EnterSecureMode();

    /*Secure Engine Call*/
    e_ret_status = (*SE_CallGatePtr)(SE_CRYPTO_LL_DECRYPT_INIT_ID, peSE_Status, pxSE_Metadata);

    /*Exit Secure Mode*/
    SE_ExitSecureMode();
#ifdef SFU_ISOLATE_SE_WITH_MPU
  }
#endif /* SFU_ISOLATE_SE_WITH_MPU */
  return e_ret_status;
}

/**
  * @brief Secure Engine Decrypt Append function.
  *        It is a wrapper of Decrypt_Append function included in the protected area.
  * @param peSE_Status Secure Engine Status.
  *        This parameter can be a value of @ref SE_Status_Structure_definition.
  * @param pInputBuffer pointer to Input Buffer.
  * @param uInputSize Input Size (bytes).
  * @param pOutputBuffe pointer to Output Buffer.
  * @param puOutputSize pointer to Output Size (bytes).
  * @retval SE_ErrorStatus SE_SUCCESS if successful, SE_ERROR otherwise.
  */
SE_ErrorStatus SE_Decrypt_Append(SE_StatusTypeDef *peSE_Status, const uint8_t *pInputBuffer, int32_t InputSize, uint8_t *pOutputBuffer, int32_t *pOutputSize)
{

  SE_ErrorStatus e_ret_status;

  /* Check if the call is coming from SFU code*/
  __IS_SFU_RESERVED();

#ifdef SFU_ISOLATE_SE_WITH_MPU
  if (0 != SE_IsUnprivileged())
  {
    uint32_t params[4] = {(uint32_t)pInputBuffer, (uint32_t)InputSize, (uint32_t)pOutputBuffer, (uint32_t)pOutputSize};
    SE_SysCall(&e_ret_status, SE_CRYPTO_LL_DECRYPT_APPEND_ID, peSE_Status, &params);
  }
  else
  {
#endif /* SFU_ISOLATE_SE_WITH_MPU */
    /* Set the CallGate function pointer */
    SET_CALLGATE();

    /*Enter Secure Mode*/
    SE_EnterSecureMode();

    /*Secure Engine Call*/
    e_ret_status = (*SE_CallGatePtr)(SE_CRYPTO_LL_DECRYPT_APPEND_ID, peSE_Status, pInputBuffer, InputSize, pOutputBuffer, pOutputSize);

    /*Exit Secure Mode*/
    SE_ExitSecureMode();
#ifdef SFU_ISOLATE_SE_WITH_MPU
  }
#endif /* SFU_ISOLATE_SE_WITH_MPU */
  return e_ret_status;
}

/**
  * @brief Secure Engine Decrypt Finish function.
  *        It is a wrapper of Decrypt_Finish function included in the protected area.
  * @param peSE_Status Secure Engine Status.
  *        This parameter can be a value of @ref SE_Status_Structure_definition.
  * @param pOutputBuffer pointer to Output Buffer.
  * @param puOutputSize pointer to Output Size (bytes).
  * @retval SE_ErrorStatus SE_SUCCESS if successful, SE_ERROR otherwise.
  */
SE_ErrorStatus SE_Decrypt_Finish(SE_StatusTypeDef *peSE_Status, uint8_t *pOutputBuffer, int32_t *pOutputSize)
{

  SE_ErrorStatus e_ret_status;

  /* Check if the call is coming from SFU code*/
  __IS_SFU_RESERVED();

#ifdef SFU_ISOLATE_SE_WITH_MPU
  if (0 != SE_IsUnprivileged())
  {
    uint32_t params[2] = {(uint32_t)pOutputBuffer, (uint32_t)pOutputSize};
    SE_SysCall(&e_ret_status, SE_CRYPTO_LL_DECRYPT_FINISH_ID, peSE_Status, &params);
  }
  else
  {
#endif /* SFU_ISOLATE_SE_WITH_MPU */
    /* Set the CallGate function pointer */
    SET_CALLGATE();

    /*Enter Secure Mode*/
    SE_EnterSecureMode();

    /*Secure Engine Call*/
    e_ret_status = (*SE_CallGatePtr)(SE_CRYPTO_LL_DECRYPT_FINISH_ID, peSE_Status, pOutputBuffer, pOutputSize);

    /*Exit Secure Mode*/
    SE_ExitSecureMode();
#ifdef SFU_ISOLATE_SE_WITH_MPU
  }
#endif /* SFU_ISOLATE_SE_WITH_MPU */
  return e_ret_status;
}

/**
  * @brief Secure Engine Firmware Authentication Init function.
  *        It is a wrapper of AuthenticateFW_Init function included in the Firewall.
  * @param peSE_Status Secure Engine Status.
  *        This parameter can be a value of @ref SE_Status_Structure_definition.
  * @param pxSE_Metadata Metadata that will be used to fill the Crypto Init structure.
  * @retval SE_ErrorStatus SE_SUCCESS if successful, SE_ERROR otherwise.
  */
SE_ErrorStatus SE_AuthenticateFW_Init(SE_StatusTypeDef *peSE_Status, SE_FwRawHeaderTypeDef *pxSE_Metadata)
{

  SE_ErrorStatus e_ret_status;

  /* Check if the call is coming from SFU code*/
  __IS_SFU_RESERVED();

#ifdef SFU_ISOLATE_SE_WITH_MPU
  if (0 != SE_IsUnprivileged())
  {
    uint32_t params[1] = {(uint32_t)pxSE_Metadata};
    SE_SysCall(&e_ret_status, SE_CRYPTO_LL_AUTHENTICATE_FW_INIT_ID, peSE_Status, &params);
  }
  else
  {
#endif /* SFU_ISOLATE_SE_WITH_MPU */
    /* Set the CallGate function pointer */
    SET_CALLGATE();

    /*Enter Secure Mode*/
    SE_EnterSecureMode();

    /*Secure Engine Call*/
    e_ret_status = (*SE_CallGatePtr)(SE_CRYPTO_LL_AUTHENTICATE_FW_INIT_ID, peSE_Status, pxSE_Metadata);

    /*Exit Secure Mode*/
    SE_ExitSecureMode();
#ifdef SFU_ISOLATE_SE_WITH_MPU
  }
#endif /* SFU_ISOLATE_SE_WITH_MPU */

  return e_ret_status;
}

/**
  * @brief Secure Engine Firmware Authentication Append function.
  *        It is a wrapper of AuthenticateFW_Append function included in the Firewall.
  * @param peSE_Status Secure Engine Status.
  *        This parameter can be a value of @ref SE_Status_Structure_definition.
  * @param pInputBuffer pointer to Input Buffer.
  * @param uInputSize Input Size (bytes).
  * @param pOutputBuffer pointer to Output Buffer.
  * @param puOutputSize pointer to Output Size (bytes).
  * @retval SE_ErrorStatus SE_SUCCESS if successful, SE_ERROR otherwise.
  */
SE_ErrorStatus SE_AuthenticateFW_Append(SE_StatusTypeDef *peSE_Status, const uint8_t *pInputBuffer, int32_t InputSize,    uint8_t *pOutputBuffer, int32_t *pOutputSize)
{

  SE_ErrorStatus e_ret_status;

  /* Check if the call is coming from SFU code*/
  __IS_SFU_RESERVED();

#ifdef SFU_ISOLATE_SE_WITH_MPU
  if (0 != SE_IsUnprivileged())
  {
    uint32_t params[4] = {(uint32_t)pInputBuffer, (uint32_t)InputSize, (uint32_t)pOutputBuffer, (uint32_t)pOutputSize};
    SE_SysCall(&e_ret_status, SE_CRYPTO_LL_AUTHENTICATE_FW_APPEND_ID, peSE_Status, &params);
  }
  else
  {
#endif /* SFU_ISOLATE_SE_WITH_MPU */
    /* Set the CallGate function pointer */
    SET_CALLGATE();

    /*Enter Secure Mode*/
    SE_EnterSecureMode();

    /*Secure Engine Call*/
    e_ret_status = (*SE_CallGatePtr)(SE_CRYPTO_LL_AUTHENTICATE_FW_APPEND_ID, peSE_Status, pInputBuffer, InputSize, pOutputBuffer, pOutputSize);

    /*Exit Secure Mode*/
    SE_ExitSecureMode();
#ifdef SFU_ISOLATE_SE_WITH_MPU
  }
#endif /* SFU_ISOLATE_SE_WITH_MPU */

  return e_ret_status;
}

/**
  * @brief Secure Engine Firmware Authentication Finish function.
  *        It is a wrapper of AuthenticateFW_Finish function included in the Firewall.
  * @param peSE_Status Secure Engine Status.
  *        This parameter can be a value of @ref SE_Status_Structure_definition.
  * @param pOutputBuffer pointer to Output Buffer.
  * @param puOutputSize pointer to Output Size (bytes).
  * @retval SE_ErrorStatus SE_SUCCESS if successful, SE_ERROR otherwise.
  */
SE_ErrorStatus SE_AuthenticateFW_Finish(SE_StatusTypeDef *peSE_Status, uint8_t *pOutputBuffer, int32_t *pOutputSize)
{

  SE_ErrorStatus e_ret_status;

  /* Check if the call is coming from SFU code*/
  __IS_SFU_RESERVED();

#ifdef SFU_ISOLATE_SE_WITH_MPU
  if (0 != SE_IsUnprivileged())
  {
    uint32_t params[2] = {(uint32_t)pOutputBuffer, (uint32_t)pOutputSize};
    SE_SysCall(&e_ret_status, SE_CRYPTO_LL_AUTHENTICATE_FW_FINISH_ID, peSE_Status, &params);
  }
  else
  {
#endif /* SFU_ISOLATE_SE_WITH_MPU */
    /* Set the CallGate function pointer */
    SET_CALLGATE();

    /*Enter Secure Mode*/
    SE_EnterSecureMode();

    /*Secure Engine Call*/
    e_ret_status = (*SE_CallGatePtr)(SE_CRYPTO_LL_AUTHENTICATE_FW_FINISH_ID, peSE_Status, pOutputBuffer, pOutputSize);

    /*Exit Secure Mode*/
    SE_ExitSecureMode();
#ifdef SFU_ISOLATE_SE_WITH_MPU
  }
#endif /* SFU_ISOLATE_SE_WITH_MPU */

  return e_ret_status;
}


/**
  * @brief Secure Engine Verify Raw Fw Header Tag.
  *        It verifies the signature of a raw header file.
  * @param peSE_Status Secure Engine Status.
  *        This parameter can be a value of @ref SE_Status_Structure_definition.
  * @param pFwRawHeader pointer to RawHeader Buffer.
  * @retval SE_ErrorStatus SE_SUCCESS if successful, SE_ERROR otherwise.
  */
SE_ErrorStatus SE_VerifyFwRawHeaderTag(SE_StatusTypeDef *peSE_Status, SE_FwRawHeaderTypeDef *pxFwRawHeader)
{
  SE_ErrorStatus e_ret_status;

  /* Check if the call is coming from SFU code*/
  __IS_SFU_RESERVED();

#ifdef SFU_ISOLATE_SE_WITH_MPU
  if (0 != SE_IsUnprivileged())
  {
    uint32_t params[1] = {(uint32_t)pxFwRawHeader};
    SE_SysCall(&e_ret_status, SE_CRYPTO_HL_AUTHENTICATE_METADATA, peSE_Status, &params);
  }
  else
  {
#endif /* SFU_ISOLATE_SE_WITH_MPU */
    /* Set the CallGate function pointer */
    SET_CALLGATE();

    /*Enter Secure Mode*/
    SE_EnterSecureMode();

    e_ret_status = (*SE_CallGatePtr)(SE_CRYPTO_HL_AUTHENTICATE_METADATA, peSE_Status, pxFwRawHeader);

    /*Exit Secure Mode*/
    SE_ExitSecureMode();
#ifdef SFU_ISOLATE_SE_WITH_MPU
  }
#endif /* SFU_ISOLATE_SE_WITH_MPU */

  return e_ret_status;

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


/* Stop placing data in specified section*/
#if defined(__ICCARM__)
#pragma default_function_attributes =
#elif defined(__CC_ARM)
#pragma arm section code
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
