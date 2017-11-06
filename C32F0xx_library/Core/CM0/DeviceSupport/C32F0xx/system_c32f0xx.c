/**************************************************************************//**
 * $Id: system_c32f0xx.c,
 *
 * @file     system_c32f0xx.c
 * @brief    CMSIS Cortex-M0 Device Peripheral Access Layer Source File
 *           for the C32F0xx Device Series
 * @version  v 1.0 
 * @date     2014/04/04 
 * @author   MCU Team
 *
 * @note
 * Copyright (C) 2010 Technology Ltd. All rights reserved.
 *
 * @par
 ******************************************************************************/

#include <stdint.h>

#include "c32f0xx.h"
#include "c32f0xx_sys.h"



/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/

#define __WDT_OSC_CLK           (32000UL)        /* Internal WDT oscillator frequency */
#define __IRC_OSC_CLK_30MHZ     (30000000UL)     /* Internal RC oscillator frequency (sample*/
#define __IRC_OSC_CLK_40MHZ     (40000000UL)     /* Internal RC oscillator frequency (sample*/
#define __IRC_OSC_CLK_48MHZ     (48000000UL)     /* Internal RC oscillator frequency (sample*/


/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = __IRC_OSC_CLK_48MHZ;  /*!< System Clock Frequency  */
uint32_t MainClock       = __IRC_OSC_CLK_48MHZ;  /*!< Main Clock Frequency    */


/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)         /* Get Core Clock Frequency      */
{
#if 0	
    //Get MainClock source
    switch (SYSCON->MAINCLKSEL.all & 0x3)
    {
        case 0: //0x0 IRC oscillator

            if ((SYSCON->IRCCTRL.all & 0x20)==0x20)
            {
                MainClock = __IRC_OSC_CLK_48MHZ;
            }                
            else
            {
                MainClock = __IRC_OSC_CLK_40MHZ;
            }                
            break;
        case 1: 
            break;
        case 2: //0x2 WDT oscillator
            MainClock = __WDT_OSC_CLK;
            break;
        case 3: 
            break;
    }
#endif
    SystemCoreClock = MainClock / SYSCON->SYSAHBCLKDIV.all;  /* Update SystemCoreClock */

    return;
}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void)
{
	//your code here
}

