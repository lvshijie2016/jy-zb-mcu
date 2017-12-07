/*****************************************************************************
 *   wdt.h:  Header file for C32F0
 *
 *   Copyright(C) 2015,
 *   All rights reserved.
 *
 *   History
 *   2014.02.21  ver 1.01    Prelimnary version
 *
******************************************************************************/
/* Includes ------------------------------------------------------------------*/

#include "c32f0xx.h"
#include "c32f0xx_sys.h"
#include "c32f0xx_wdt.h"
extern uint32_t MainClock;
/*****************************************************************************
Function Name	WDT_Init
Function Definition	void WDT_Init (void)
Function Description	Initial WDT module
Input Parameters	No 
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void WDT_Init (void)
{
    //enable WDT PCLK
    SYSCON->SYSAHBCLKCTRL.bit.WDTCLK=1;

    return;
}
/*****************************************************************************
Function Name	WDT_Deinit
Function Definition	void WDT_Deinit (void)
Function Description	De-Initial WDT module
Input Parameters	No 
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void WDT_Deinit(void)
{
    //disable WDT PCLK
    SYSCON->SYSAHBCLKCTRL.bit.WDTCLK=0;	
    return;
}
/*****************************************************************************
Function Name	WDT_SetWindowValue
Function Definition	void WDT_SetWindowValue (uint32_t intticks,uint32_t windowticks)
Function Description	Set WDT two windows value. 
Input Parameters	intticks: if WDT counter decrease to intticks value, the interrupt is generated
windowticks: if WDT counter decrease to windowticks value, no more feed valid. 
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void WDT_SetWindowValue (uint32_t intticks,uint32_t windowticks)
{
    //WDT->WARNINT.bit.WARNINT = intticks;
	//WDT->WINDOW.bit.WDWINDOW = windowticks;
	  
#if 1
	WDT->WARNINT.bit.WARNINT = intticks & 0x3FF;// only bit0~9 useful
	
	windowticks &= 0x00FFFFFF;// only bit0~23 useful
	if(windowticks < 0x100)//The minimum value is 0x100
	{
		windowticks = 0x100;
	}
	WDT->WINDOW.bit.WDWINDOW = windowticks;
#endif	
	return;
}

/*****************************************************************************
Function Name	WDT_Feed
Function Definition	void WDT_Feed (void)
Function Description	Watchdog feed
Input Parameters	No
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void WDT_Feed (void)
{
    WDT->FEED.bit.WDFEED  = 0xAA;
    WDT->FEED.bit.WDFEED  = 0x55;
    return;	
}
/*****************************************************************************
Function Name	WDT_SetIntervalinTick
Function Definition	void WDT_SetIntervalinTick (uint32_t ticks)
Function Description	Setup WDT time out interval
Input Parameters	ticks: time interval in WDT tick
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void WDT_SetIntervalinTick(uint32_t ticks)
{
    uint32_t dlytime = 500;
    WDT->TC.bit.WDTC = ticks;
    while(dlytime--);    
    return;
}
/*****************************************************************************
Function Name	WDT_SetIntervalinMiliSec
Function Definition	void WDT_SetIntervalinMiliSec(uint32_t ms)
Function Description	Setup WDT time out interval
Input Parameters	ms: time interval in mili second
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void WDT_SetIntervalinMiliSec(uint32_t ms)
{
    uint32_t ticks   = 0;
    uint32_t dlytime = 500;
    if (SYSCON->WDTOSCCTRL.bit.WDTCLKSRC)
    {
        ticks = 32000;
    }
    else
    {
        ticks = MainClock;
    }
    	
    //convert mili second to ticks
    ticks = (ms * ticks / 1000) >> 2;
	
    //set WDT timeout interval
    WDT->TC.bit.WDTC = ticks;
    
    while(dlytime--);

    return;
}
/*****************************************************************************
Function Name	WDT_EnableResetSystem
Function Definition	void WDT_EnableResetSystem(void)
Function Description	Enable system reset after WDT timeout occurs
Input Parameters	No
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void WDT_EnableResetSystem(void)
{
    WDT->MOD.bit.WDRESET = 0;
    return;
}
/*****************************************************************************
Function Name	WDT_ClearlntFlag
Function Definition	void WDT_ClearIntFlag(void)
Function Description	Clear WDT interrupt flag
Input Parameters	No
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void WDT_ClearIntFlag(void)
{
    //clr time out flag
    WDT->MOD.bit.WDTOF=0;
    //clr window int flag
    WDT->MOD.bit.WDINT=1;
}
/*****************************************************************************
Function Name	WDT_ClearlntFlag
Function Definition	uint32_t WDT_GetCounterValue (void)
Function Description	Return WDT counter value
Input Parameters	No
Return Value	WDT counter value
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
uint32_t WDT_GetCounterValue (void)
{
    return WDT->TV.all;
}
/*****************************************************************************
Function Name	WDT_EnableLock
Function Definition	void WDT_EnableLock (void)
Function Description	Lock WDT Setting. Once WDT is locked, you can not do any changing to WDT, 
including enable/disable timer, changing of WDT timer out interval, and reset system setting when time out.
Input Parameters	No
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void WDT_EnableLock (void)
{
    WDT->MOD.bit.WDLOCKCLK=1;
    WDT->MOD.bit.WDLOCKDP=1;
    WDT->MOD.bit.WDLOCKEN=1;

    return;
}
 
