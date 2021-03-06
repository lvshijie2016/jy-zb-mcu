/**************************************************************************//**
* @file    HAL_device.h
* @brief   CMSIS Cortex-M Peripheral Access Layer for MindMotion
*          microcontroller devices
*
*          This is a convenience header file for defining the part number on the
*          build command line, instead of specifying the part specific header file.
*
* Example: Add "-DMM32X031" to your build options, to define part
*          Add "#include "HAL_device.h" to your source files
*
*
* @version 1.6.0
*
*
*****************************************************************************/

#ifndef __HAL_device_H
#define __HAL_device_H


#define MM32F031x4_x6
#if defined(MM32F031x4_x6)
#include "MM32F031x4_x6.h"

#else
#error "HAL_device.h: PART NUMBER undefined"
#endif
#endif /* __HAL_device_H */
/*-------------------------(C) COPYRIGHT 2017 MindMotion ----------------------*/
