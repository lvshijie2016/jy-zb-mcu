/**************************************************************************//**
 * $Id: system_c32f0xx.h,v   $
 *
 * @file     system_c32f0xx.h
 * @brief    CMSIS Cortex-M0 Device Peripheral Access Layer Header File
 *           for the C32F0xx Device Series
 * @version  1.5
 * @date     2012/09/22 08:52:47 jpan Exp
 * @author   MCU Team
 *
 * @note
 * Copyright (C) 2011 Ltd.. All rights reserved.
 *
 * @par
 ******************************************************************************/

#ifndef __SYSTEM_C32F0xx_H
#define __SYSTEM_C32F0xx_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */
extern uint32_t MainClock;           /*!< Main Clock Frequency (Main Clock) */

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);

/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemCoreClock with current core Clock
 *         retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_C32F0xx_H */
