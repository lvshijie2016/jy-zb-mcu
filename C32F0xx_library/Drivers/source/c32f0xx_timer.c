/*   Copyright(C) 20012, 
 *   All rights reserved.
 *
 *   History
 *   2012.07.21  ver 1.01    Prelimnary version
 *
******************************************************************************/
/* Includes ------------------------------------------------------------------*/

#include "c32f0xx.h"

#include "c32f0xx_timer.h"
#include "c32f0xx_sys.h"

extern uint32_t SystemCoreClock;
/*****************************************************************************
Function Name	CT16B0_Init
Function Definition	void CT16B0_Init(CT16B0_TypeDef *ct, uint32_t tickpersecond)
Function Description	Initial basic timer
Input Parameters	*ct: pointer to CT16B0/CT16B1
tickpersecond: timer count frequency, less than system clock
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B0_Init(CT16B0_TypeDef *ct, uint32_t tickpersecond)
{

	if (ct == CT16B0)
	{
		SYSCON->SYSAHBCLKCTRL.bit.CT16B0CLK=1;
		SYSCON->PRESETCTRL.bit.CT16B0_RST_N=0;
		SYSCON->PRESETCTRL.bit.CT16B0_RST_N=1;
	}
	else if (ct == CT16B1)
	{
		SYSCON->SYSAHBCLKCTRL.bit.CT16B1CLK=1;
		SYSCON->PRESETCTRL.bit.CT16B1_RST_N=0;
		SYSCON->PRESETCTRL.bit.CT16B1_RST_N=1;
	}	else return;
	
	if (tickpersecond > SystemCoreClock)
    {
        tickpersecond = SystemCoreClock;
    }   
			
	//ct->PR.bit.PCVAL = SystemCoreClock / tickpersecond - 1;	
    ct->PR.bit.PCVAL = SystemCoreClock / tickpersecond;	
    ct->PR.bit.PCVAL = ct->PR.bit.PCVAL;
	return;
}
/*****************************************************************************
Function Name	CT16B0_ConfigMatch
Function Definition	void CT16B0_ConfigMatch(CT16B0_TypeDef *ct, uint16_t ticks, uint8_t action)
Function Description	Setup Match
Input Parameters	*ct: pointer to CT16B0/CT16B1
ticks: match value
action: combination of following action when counter reach match value
	CT_MATCH_TRIGGER_INT 0x1
	CT_MATCH_RESET_COUNTER 0x2
	CT_MATCH_STOP_COUNTER 0x4
	CT_MATCH_DO_NOTHING 0x0
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B0_ConfigMatch(CT16B0_TypeDef *ct, uint16_t ticks, uint8_t action)
{
	ct->MR0.bit.MATCH = ticks - 1;
	ct->MCR.all=action;
	return;
}
/*****************************************************************************
Function Name	CT16B0_SetTimerCounter
Function Definition	void CT16B0_SetTimerCounter(CT16B0_TypeDef *ct, uint16_t tick)
Function Description	*ct: pointer to CT16B0/CT16B1
tick: set current value
Input Parameters	No
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B0_SetTimerCounter(CT16B0_TypeDef *ct, uint16_t tick)
{
	ct->PC.bit.PC=tick;
	return;
}
/*****************************************************************************
Function Name	CT16B0_ResetTimerCounter
Function Definition	void CT16B0_ResetTimerCounter(CT16B0_TypeDef *ct)
Function Description	Reset Timer Counter value
Input Parameters	*ct: pointer to CT16B0/CT16B1
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B0_ResetTimerCounter(CT16B0_TypeDef *ct)
{
	ct->PC.bit.PC=0;
	return;
}
/*****************************************************************************
Function Name	CT16B0_GetTimerCounter
Function Definition	uint16_t CT16B0_GetTimerCounter(CT16B0_TypeDef *ct)
Function Description	Get current timer counter value
Input Parameters	*ct: pointer to CT16B0/CT16B1
Return Value	Timer Counter value
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
uint16_t CT16B0_GetTimerCounter(CT16B0_TypeDef *ct)
{
	return ct->PC.bit.PC;
}
/*****************************************************************************
Function Name	CT16B0_ClearIntFlag
Function Definition	void CT16B0_ClearIntFlag(CT16B0_TypeDef *ct)
Function Description	Clear Match event interrupt
Input Parameters	*ct: pointer to CT16B0/CT16B1
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B0_ClearIntFlag(CT16B0_TypeDef *ct)
{
	ct->IR.bit.MR0INT=1;
	return;
}
/*****************************************************************************
Function Name	CT16B0_ClearIntFlag
Function Definition	void CT16B0_ClearIntFlag(CT16B0_TypeDef *ct)
Function Description	Clear Match event interrupt
Input Parameters	*ct: pointer to CT16B0/CT16B1
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B0_EnableInt(CT16B0_TypeDef *ct)
{
	ct->MCR.bit.MR0I=1;
	return;
}
/*****************************************************************************
Function Name	CT16B0_DisableInt
Function Definition	void CT16B0_DisableInt(CT16B0_TypeDef *ct)
Function Description	Disable Match event interrupt
Input Parameters	*ct: pointer to CT16B0/CT16B1
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B0_DisableInt(CT16B0_TypeDef *ct)
{
	ct->MCR.bit.MR0I=0;
	return;	
}
/*****************************************************************************
Function Name	CT16B2_Init
Function Definition	void CT16B2_Init(CT16B2_TypeDef *ct, uint32_t tickpersecond)
Function Description	Initial timer
Input Parameters	*ct: pointer to CT16B2/CT16B3
tickpersecond: timer count frequency, less than system clock
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B2_Init(CT16B2_TypeDef *ct, uint32_t tickpersecond)
{
	if (ct == CT16B2)
	{
		SYSCON->SYSAHBCLKCTRL.bit.CT16B2CLK=1;
		SYSCON->PRESETCTRL.bit.CT16B2_RST_N=0;
		SYSCON->PRESETCTRL.bit.CT16B2_RST_N=1;
	}
	else if (ct == CT16B3)
	{
		SYSCON->SYSAHBCLKCTRL.bit.CT16B3CLK=1;
		SYSCON->PRESETCTRL.bit.CT16B3_RST_N=0;
		SYSCON->PRESETCTRL.bit.CT16B3_RST_N=1;
	}	else return;
	
	if (tickpersecond>SystemCoreClock)
	{
		tickpersecond = SystemCoreClock;
	}
			
	//ct->PR.bit.PCVAL = SystemCoreClock / tickpersecond - 1;
    ct->PR.bit.PCVAL = SystemCoreClock / tickpersecond;

	return;
}
/*****************************************************************************
Function Name	CT16B2_SetCTType
Function Definition	void CT16B2_SetCTType(CT16B2_TypeDef *ct, uint8_t mode)
Function Description	Initial timer/counter mode
Input Parameters	*ct: pointer to CT16B2/CT16B3
mode: counter/timer mode
			000 	Timer Mode, every PCLK
			001 	Counter Mode, Primary select signal rise edge
			010 	Counter Mode, Primary select signal fall edge 
			011 	Edge Count Mode  
			100	Quadrature mode
			101	Trigger count mode.
			110	Signed count mode.
			111	Gate count mode.
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B2_SetCTType(CT16B2_TypeDef *ct, uint8_t mode)
{
	ct->CTCR.bit.CTM=mode;
	return;
}
/*****************************************************************************
Function Name	CT16B2_SelectTimerClearSignal
Function Definition	void CT16B2_SelectTimerClearSignal(CT16B2_TypeDef *ct, uint8_t capsignal, uint8_t edge)
Function Description	Select capture event to clear timer
Input Parameters	*ct: pointer to CT16B2/CT16B3
capsignal: capture signal
					0, CAP0
					1, CAP1
					2, CAP2
					4, CAP3
					5, Other Timer CAP0
					6, Other Timer CAP1
					7, Other Timer CAP2
					8, Other Timer CAP3
Edge: RISE_EDGE/FALL_EDGE
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B2_SelectTimerClearSignal(CT16B2_TypeDef *ct, uint8_t capsignal, uint8_t edge)
{
	//set counter clear singal
	ct->CTCR.bit.SELCC=capsignal<<1;
	if (edge==FALL_EDGE)
			ct->CTCR.bit.SELCC++;
	//enable counter clear action
	ct->CTCR.bit.ENCC=1;
	return;
}	
/*****************************************************************************
Function Name	CT16B2_SelectPriSrc
Function Definition	void CT16B2_SelectPriSrc(CT16B2_TypeDef *ct, uint8_t clksrc)
Function Description	Initial basic timer
Input Parameters	*ct: pointer to CT16B2/CT16B3
clksrc: clock source
				0000	CAP0 pin
				0001	CAP1 pin
				0010	CAP2 pin
				0011	CAP3 pin
				0100	PWM Event
				0101	CT16B2_MAT0./ CT16B3_MAT0
				0110	Reserved
				0111	Reserved.
				1xxx	Prescale counter
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B2_SelectPriSrc(CT16B2_TypeDef *ct, uint8_t clksrc)
{
	ct->CTCR.bit.PRISEL=clksrc;
	return;
}
/*****************************************************************************
Function Name	CT16B2_SelectPriSrc
Function Definition	void CT16B2_SelectSecSrc (CT16B2_TypeDef *ct, uint8_t clksrc, uint8_t polarity)
Function Description	Initial basic timer
Input Parameters	*ct: pointer to CT16B2/CT16B3
clksrc: clock source
				0000	CAP0 pin
				0001	CAP1 pin
				0010	CAP2 pin
				0011	CAP3 pin
				0100	PWM Event
				0101	CT16B2_MAT0./ CT16B3_MAT0
				0110	Reserved
				0111	Reserved.
				1xxx	Prescale counter
polarity: inverse clock source

Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B2_SelectSecSrc (CT16B2_TypeDef *ct, uint8_t clksrc, uint8_t polarity)
{
	ct->CTCR.bit.SECSEL=clksrc;
	ct->CTCR.bit.IPS=polarity;
	return;
}
/*****************************************************************************
Function Name	CT16B2_ConfigMatch0
Function Definition	void CT16B2_ConfigMatch0(CT16B2_TypeDef *ct, uint16_t ticks, uint8_t internalaction, uint8_t externalaction)
Function Description	Setup timer match 0 and internal action when match even occurs
Input Parameters	*ct: pointer to CT16B2/CT16B3
ticks: match 0 value
internalaction: combination of following action when counter reach match value
               CT_MATCH_TRIGGER_INT 0x1
               CT_MATCH_RESET_COUNTER 0x2
               CT_MATCH_STOP_COUNTER 0x4
               CT_MATCH_DO_NOTHING 0x0 
externalaction: one of following action when counter reach match value
               CT_MATCH_OUT_DO_NOTHING 0x0
               CT_MATCH_OUT_RESET 0x1
               CT_MATCH_OUT_SET	 0x2
               CT_MATCH_OUT_TRIGGER_PLUS	 0x3

Return Value	No
Condition	No
Function called	-

Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B2_ConfigMatch0(CT16B2_TypeDef *ct, uint16_t ticks, uint8_t internalaction, uint8_t externalaction)
{
	ct->MR0.bit.MATCH=ticks-1;
	
	ct->MCR.all=(ct->MCR.all&(~0x7))|(internalaction&0x7);
	
	ct->EMR.bit.EMC0=externalaction;
	ct->EMR.bit.EM0=1;	
	return;
}
/*****************************************************************************
Function Name	CT16B2_ConfigMatch1
Function Definition	void CT16B2_ ConfigMatch1(CT16B2_TypeDef *ct, uint16_t ticks, uint8_t internalaction, uint8_t externalaction)
Function Description	Setup timer match 1 and internal action and CT16B2_MAT1/ CT16B2_MAT1 output external action when match even occurs
Input Parameters	*ct: pointer to CT16B2/CT16B3
ticks: match 1 value
internalaction: combination of following action when counter reach match value
			CT_MATCH_TRIGGER_INT 0x1
			CT_MATCH_RESET_COUNTER 0x2
			CT_MATCH_STOP_COUNTER 0x4
			CT_MATCH_DO_NOTHING 0x0 
externalaction: one of following action when counter reach match value
			CT_MATCH_OUT_DO_NOTHING 0x0
			CT_MATCH_OUT_RESET 0x1
			CT_MATCH_OUT_SET	 0x2
			CT_MATCH_OUT_TRIGGER_PLUS	 0x3
Return Value	No
Condition	No
Function called	-


Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B2_ConfigMatch1(CT16B2_TypeDef *ct, uint16_t ticks, uint8_t internalaction, uint8_t externalaction)
{
	ct->MR1.bit.MATCH=ticks;
	ct->MCR.all=(ct->MCR.all&(~(0x7<<3)))|((internalaction&0x7)<<3);

	ct->EMR.bit.EMC1=externalaction;
	ct->EMR.bit.EM1=1;	
	return;
}	
/*****************************************************************************
Function Name	CT16B2_ConfigMatch2
Function Definition	void CT16B2_ConfigMatch2(CT16B2_TypeDef *ct, uint16_t ticks, uint8_t internalaction, uint8_t externalaction)
Function Description	Setup timer match 2 and internal action and CT16B2_MAT2/ CT16B2_MAT2 output external action when match even occurs
Input Parameters	*ct: pointer to CT16B2/CT16B3
ticks: match 2 value
internalaction: combination of following action when counter reach match value
		CT_MATCH_TRIGGER_INT 0x1
		CT_MATCH_RESET_COUNTER 0x2
		CT_MATCH_STOP_COUNTER 0x4
		CT_MATCH_DO_NOTHING 0x0 
externalaction: one of following action when counter reach match value
		CT_MATCH_OUT_DO_NOTHING 0x0
		CT_MATCH_OUT_RESET 0x1
		CT_MATCH_OUT_SET	 0x2
		CT_MATCH_OUT_TRIGGER_PLUS	 0x3
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B2_ConfigMatch2(CT16B2_TypeDef *ct, uint16_t ticks, uint8_t internalaction, uint8_t externalaction)
{
	ct->MR2.bit.MATCH=ticks;
	ct->MCR.all=(ct->MCR.all&(~(0x7<<6)))|((internalaction&0x7)<<6);
	
	ct->EMR.bit.EMC2=externalaction;
	ct->EMR.bit.EM2=1;		
	return;
}
/*****************************************************************************
Function Name	CT16B2_ConfigMatch3
Function Definition	void CT16B2_ConfigMatch3(CT16B2_TypeDef *ct, uint16_t ticks, uint8_t internalaction, uint8_t externalaction)
Function Description	Setup timer match 3 and internal action and CT16B2_MAT3/ CT16B2_MAT3 output external action when match even occurs
Input Parameters	*ct: pointer to CT16B2/CT16B3
ticks: match 3 value
internalaction: combination of following action when counter reach match value
		CT_MATCH_TRIGGER_INT 0x1
		CT_MATCH_RESET_COUNTER 0x2
		CT_MATCH_STOP_COUNTER 0x4
		CT_MATCH_DO_NOTHING 0x0 
externalaction: one of following action when counter reach match value
		CT_MATCH_OUT_DO_NOTHING 0x0
		CT_MATCH_OUT_RESET 0x1
		CT_MATCH_OUT_SET	 0x2
		CT_MATCH_OUT_TRIGGER_PLUS	 0x3
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B2_ConfigMatch3(CT16B2_TypeDef *ct, uint16_t ticks, uint8_t internalaction, uint8_t externalaction)
{
	ct->MR1.bit.MATCH=ticks;
	ct->MCR.all=(ct->MCR.all&(~(0x7<<9)))|((internalaction&0x7)<<9);
	
	ct->EMR.bit.EMC3=externalaction;
	ct->EMR.bit.EM3=1;			
	return;
}

/*****************************************************************************
Function Name	CT16B2_ConfigCapture0
Function Definition	void CT16B2_ConfigCapture0 (CT16B2_TypeDef *ct, uint8_t edge, uint8_t intreq)
Function Description	Set up Capture 0 
Input Parameters	*ct: pointer to CT16B2/CT16B3
edge: CAP0 signal edge, RISE_EDGE/FALL_EDGE/BOTH_EDGE. Deifned in sys.h
intreq: interrupt request
			CT_CAPTURE_DO_NOTHING
			CT_CAPTURE_TRIGGER_INT
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B2_ConfigCapture0(CT16B2_TypeDef *ct, uint8_t edge, uint8_t intreq)
{
	switch(edge)
	{
		case RISE_EDGE:
				ct->CCR.bit.CAP0RE=1;
				ct->CCR.bit.CAP0FE=0;
			break;
		case FALL_EDGE:
				ct->CCR.bit.CAP0RE=0;
				ct->CCR.bit.CAP0FE=1;
			break;
		case BOTH_EDGE:
				ct->CCR.bit.CAP0RE=1;
				ct->CCR.bit.CAP0FE=1;
			break;
		default:
			break;
	}
	if (intreq==CT_CAPTURE_TRIGGER_INT)
		ct->CCR.bit.CAP0I=1;
	else
		ct->CCR.bit.CAP0I=0;
	return;
}
/*****************************************************************************
Function Name	CT16B2_ConfigCapture1
Function Definition	void CT16B2_ConfigCapture1 (CT16B2_TypeDef *ct, uint8_t edge, uint8_t intreq)
Function Description	Set up Capture 1 
Input Parameters	*ct: pointer to CT16B2/CT16B3
edge: CAP0 signal edge, RISE_EDGE/FALL_EDGE/BOTH_EDGE. Deifned in sys.h
intreq: interrupt request
			CT_CAPTURE_DO_NOTHING
			CT_CAPTURE_TRIGGER_INT
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B2_ConfigCapture1(CT16B2_TypeDef *ct, uint8_t edge, uint8_t intreq)
{
	switch(edge)
	{
		case RISE_EDGE:
				ct->CCR.bit.CAP1RE=1;
				ct->CCR.bit.CAP1FE=0;
			break;
		case FALL_EDGE:
				ct->CCR.bit.CAP1RE=0;
				ct->CCR.bit.CAP1FE=1;
			break;
		case BOTH_EDGE:
				ct->CCR.bit.CAP1RE=1;
				ct->CCR.bit.CAP1FE=1;
			break;
		default:
			break;
	}
	if (intreq==CT_CAPTURE_TRIGGER_INT)
		ct->CCR.bit.CAP1I=1;
	else
		ct->CCR.bit.CAP1I=0;
	return;
}
/*****************************************************************************
Function Name	CT16B2_ConfigCapture2
Function Definition	void CT16B2_ConfigCapture2 (CT16B2_TypeDef *ct, uint8_t edge, uint8_t intreq)
Function Description	Set up Capture 2 
Input Parameters	*ct: pointer to CT16B2/CT16B3
edge: CAP0 signal edge, RISE_EDGE/FALL_EDGE/BOTH_EDGE. Deifned in sys.h
intreq: interrupt request
			CT_CAPTURE_DO_NOTHING
			CT_CAPTURE_TRIGGER_INT
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B2_ConfigCapture2(CT16B2_TypeDef *ct, uint8_t edge, uint8_t intreq)
{
	switch(edge)
	{
		case RISE_EDGE:
				ct->CCR.bit.CAP2RE=1;
				ct->CCR.bit.CAP2FE=0;
			break;
		case FALL_EDGE:
				ct->CCR.bit.CAP2RE=0;
				ct->CCR.bit.CAP2FE=1;
			break;
		case BOTH_EDGE:
				ct->CCR.bit.CAP2RE=1;
				ct->CCR.bit.CAP2FE=1;
			break;
		default:
			break;
	}
	if (intreq==CT_CAPTURE_TRIGGER_INT)
		ct->CCR.bit.CAP2I=1;
	else
		ct->CCR.bit.CAP2I=0;
	return;
}
/*****************************************************************************
Function Name	CT16B2_ConfigCapture3
Function Definition	void CT16B2_ConfigCapture3 CT16B2_TypeDef *ct, uint8_t edge, uint8_t intreq)
Function Description	Set up Capture 3 
Input Parameters	*ct: pointer to CT16B2/CT16B3
edge: CAP0 signal edge, RISE_EDGE/FALL_EDGE/BOTH_EDGE. Deifned in sys.h
intreq: interrupt request
			CT_CAPTURE_DO_NOTHING
			CT_CAPTURE_TRIGGER_INT
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B2_ConfigCapture3(CT16B2_TypeDef *ct, uint8_t edge, uint8_t intreq)
{
	switch(edge)
	{
		case RISE_EDGE:
				ct->CCR.bit.CAP3RE=1;
				ct->CCR.bit.CAP3FE=0;
			break;
		case FALL_EDGE:
				ct->CCR.bit.CAP3RE=0;
				ct->CCR.bit.CAP3FE=1;
			break;
		case BOTH_EDGE:
				ct->CCR.bit.CAP3RE=1;
				ct->CCR.bit.CAP3FE=1;
			break;
		default:
			break;
	}
	if (intreq==CT_CAPTURE_TRIGGER_INT)
		ct->CCR.bit.CAP3I=1;
	else
		ct->CCR.bit.CAP3I=0;
	return;
}
/*****************************************************************************
Function Name	CT16B2_GetCapture0Value
Function Definition	void CT16B2_GetCapture0Value(CT16B2_TypeDef *ct)
Function Description	Return timer Capture 0 register value
Input Parameters	*ct: pointer to CT16B2/CT16B3
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
uint16_t CT16B2_GetCapture0Value (CT16B2_TypeDef *ct)
{
	return ct->CR0.bit.CAP;
}
/*****************************************************************************
Function Name	CT16B2_GetCapture1Value
Function Definition	void CT16B2_GetCapture1Value(CT16B2_TypeDef *ct)
Function Description	Return timer Capture 1 register value
Input Parameters	*ct: pointer to CT16B2/CT16B3
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
uint16_t CT16B2_GetCapture1Value (CT16B2_TypeDef *ct)
{
	return ct->CR1.bit.CAP;
}
/*****************************************************************************
Function Name	CT16B2_GetCapture2Value
Function Definition	void CT16B2_GetCapture2Value(CT16B2_TypeDef *ct)
Function Description	Return timer Capture 2 register value
Input Parameters	*ct: pointer to CT16B2/CT16B3
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
uint16_t CT16B2_GetCapture2Value (CT16B2_TypeDef *ct)
{
	return ct->CR2.bit.CAP;
}
/*****************************************************************************
Function Name	CT16B2_GetCapture3Value
Function Definition	void CT16B2_GetCapture3Value(CT16B2_TypeDef *ct)
Function Description	Return timer Capture 3 register value
Input Parameters	*ct: pointer to CT16B2/CT16B3
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
uint16_t CT16B2_GetCapture3Value (CT16B2_TypeDef *ct)
{
	return ct->CR3.bit.CAP;
}
/*****************************************************************************
Function Name	CT16B2_SetTimerCounter
Function Definition	void CT16B2_SetTimerCounter(CT16B2_TypeDef *ct, uint16_t ticks)
Function Description	Set timer counter current value
Input Parameters	*ct: pointer to CT16B2/CT16B3
tick: value to be set to current counter
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B2_SetTimerCounter(CT16B2_TypeDef *ct, uint16_t ticks)
{
	ct->TC.bit.TC=ticks;
	return;
}
/*****************************************************************************
Function Name	CT16B2_ResetTimerCounter
Function Definition	void CT16B2_ResetTimerCounter(CT16B2_TypeDef *ct)
Function Description	Reset timer counter current value
Input Parameters	*ct: pointer to CT16B2/CT16B3
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B2_ResetTimerCounter(CT16B2_TypeDef *ct)
{
	ct->TCR.bit.CRST=1;
	ct->TCR.bit.CRST=0;
	return;
}
/*****************************************************************************
Function Name	CT16B2_GetTimerCounter
Function Definition	uint16_tCT16B2_GetTimerCounter (CT16B2_TypeDef *ct)
Function Description	Get timer counter current value
Input Parameters	*ct: pointer to CT16B2/CT16B3
Return Value	Current counter value
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
uint16_t CT16B2_GetTimerCounter (CT16B2_TypeDef *ct)
{
	return ct->TC.bit.TC;
}
/*****************************************************************************
Function Name	CT16B2_ClearIntFlag
Function Definition	void CT16B2_ClearIntFlag(CT16B2_TypeDef *ct)
Function Description	Clear interrupt
Input Parameters	*ct: pointer to CT16B0/CT16B1
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B2_ClearIntFlag(CT16B2_TypeDef *ct)
{
	ct->IR.all = 0xFF;
	return;
}
/*****************************************************************************
Function Name	CT16B2_EnableInt
Function Definition	void CT16B2_EnableInt(CT16B2_TypeDef *ct, uint8_t intbit)
Function Description	Enable event interrupt
Input Parameters	*ct: pointer to CT16B2/CT16B3
intbit: bit0~7 related match0~match3,capture0~capture3
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B2_EnableInt(CT16B2_TypeDef *ct, uint8_t intbit)
{
}
/*****************************************************************************
Function Name	CT16B2_DisableInt
Function Definition	void CT16B2_DisableInt(CT16B2_TypeDef *ct, uint8_t intbit)
Function Description	Disable event interrupt
Input Parameters	*ct: pointer to CT16B2/CT16B3
intbit: bit0~7 related match0~match3,capture0~capture3
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void CT16B2_DisableInt(CT16B2_TypeDef *ct, uint8_t intbit)
{
}
