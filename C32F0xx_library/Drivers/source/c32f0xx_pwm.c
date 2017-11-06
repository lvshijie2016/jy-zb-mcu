/*****************************************************************************
 *   pwm.c:  Header file for C32F0xx
 *
 *   Copyright(C) 2015, 
 *   All rights reserved.
 *
 *   History
 *   2015.09.21  ver 1.01    Prelimnary version
 *
******************************************************************************/
/* Includes ------------------------------------------------------------------*/

#include "c32f0xx.h"
#include "c32f0xx_sys.h"
#include "c32f0xx_pwm.h"

extern uint32_t SystemCoreClock;

/*****************************************************************************
** Function name:		PWM_Init
**
** Descriptions:		Init PWM Module
**
** parameters:			
**                	freq: PWM Frequency
**					prescaler:1,2,4,8
**					pair: PWM_COMPLEMENT,PWM_DEPENDENT
**					align: PWM_CENTER,PWM_EDGE
**                  polarity: PWM_POSITIVE,PWM_NEGATIVE
**
** Returned value:		none
** 
*****************************************************************************/
void PWM_Init(uint32_t freq,uint8_t prescaler,uint8_t pair, uint8_t align, uint8_t polarity)
{
	//enable PWM clk
	(*SYSCON).SYSAHBCLKCTRL.bit.PWMCLK = 1;
	
	//set prescaler 
	(*PWM).CTRL.bit.PRSC = prescaler;

	//Config 0-1,2-3,4-5 pair
	(*PWM).CNFG.bit.NDEP01 = pair;//complementary
	(*PWM).CNFG.bit.NDEP23 = pair;//complementary
	(*PWM).CNFG.bit.NDEP45 = pair;//complementary

	//This write-protectable bit determines the polarity for the bottom-side  OR top-side PWMs.
	(*PWM).CNFG.bit.TOPNEG01 = 0;//positive top-side polarity
	(*PWM).CNFG.bit.BOTNEG01 = 0;
	(*PWM).CNFG.bit.TOPNEG23 = 0;//positive top-side polarity
	(*PWM).CNFG.bit.BOTNEG23 = 0;
	(*PWM).CNFG.bit.TOPNEG45 = 0;//positive top-side polarity
	(*PWM).CNFG.bit.BOTNEG45 = 0;
	
	//Cofig PMW as Center-Aligned
	(*PWM).CNFG.bit.EDG = align;//Center-Aligned

	//set modulu register, to generate freq Hz wave form
	if(align == 1)
	{
		(*PWM).CMOD.all = SystemCoreClock/((1<<prescaler) * freq);
		(*PWM).CTRL.bit.HALF = 0;
	}
	else
	{
		(*PWM).CMOD.all=(SystemCoreClock/((1<<prescaler) * freq))/2;
		(*PWM).CTRL.bit.HALF = 1;
	}
	
	//PWM compare output is driven to high when the CNTR register is great thanVAL
	//0:PWM compare output 0 is driven to high when the CNTR register is great thanVAL0
	//1:PWM compare output 0 is driven to high when the CNTR register is less thanVAL0
	(*PWM).PSCR.bit.CINV0 = 1;
	(*PWM).PSCR.bit.CINV1 = 1;
	(*PWM).PSCR.bit.CINV2 = 1;
	(*PWM).PSCR.bit.CINV3 = 1;
	(*PWM).PSCR.bit.CINV4 = 0;
	(*PWM).PSCR.bit.CINV5 = 0;
	
	//0-1,2-3,4-5 pair
	//Select the PWM value register for the PWM_0 and PWM_1 pins pair in complementary mode.
	//0:Take VAL0 register value in next PWM cycle
	//1:Take VAL1 register value in next PWM cycle
	(*PWM).CTRL.bit.IPOL0 = 1;	// VAL1
	(*PWM).CTRL.bit.IPOL1 = 1;	// VAL3
	(*PWM).CTRL.bit.IPOL2 = 1;	// VAL5
	
	//Intial phase
	(*PWM).CNTRINI.bit.INITVAL = 0;
	
	//Load Value
	(*PWM).CTRL.bit.LDOK = 1;
}

/*****************************************************************************
** Function name:		PWM_DeInit
**
** Descriptions:		De_Initial PWM module
**
** parameters:			none
**                	
**
** Returned value:		none
** 
*****************************************************************************/
void PWM_DeInit(void)
{
	//Disable PWM clk
	(*SYSCON).SYSAHBCLKCTRL.bit.PWMCLK = 0;
}

/*****************************************************************************
** Function name:		PWM_SetRelaodFreq
**
** Descriptions:		set PWM reload frequency
**
** parameters:			
**                	  
**									everytimes: how much time to reload
**
** Returned value:		none
** 
*****************************************************************************/
void PWM_SetRelaodFreq(uint8_t everytimes)
{
	(*PWM).CTRL.bit.LDFQ = everytimes;
}

/*****************************************************************************
** Function name:		PWM_SetRelaodFreq
**
** Descriptions:		Set PWM Duty
** parameters:			
**                	channelindex: PWM0~5 
**									dutyval: 0-100
**
** Returned value:		none
** 
*****************************************************************************/
void PWM_SetDuty(uint8_t channelindex, uint16_t dutyval)
{
	uint8_t i;
	uint32_t channelenable;
	
	for (i=0;i<6;i++)
	{
		channelenable = channelindex & (1<<i);
		if (channelenable != 0)
		{
			switch (i)
			{
				case 0:
					(*PWM).VAL0.all = ((*PWM).CMOD.all*dutyval)/100;
					break;
				case 1:
					(*PWM).VAL1.all = ((*PWM).CMOD.all*dutyval)/100;
					break;
				case 2:
					(*PWM).VAL2.all = ((*PWM).CMOD.all*dutyval)/100;
					break;
				case 3:
					(*PWM).VAL3.all = ((*PWM).CMOD.all*dutyval)/100;
					break;
				case 4:
					(*PWM).VAL4.all = ((*PWM).CMOD.all*dutyval)/100;
					break;
				case 5:
					(*PWM).VAL5.all = ((*PWM).CMOD.all*dutyval)/100;                
					break;
				default:
					break;
			}
		}
	}
	
	(*PWM).CTRL.bit.LDOK = 1;
}

/*****************************************************************************
** Function name:		PWM_SetRiseDeadZone
**
** Descriptions:		odd channel on rising edge
**
** parameters:			
**                	
**									rdtimeticks: deadtime us
**
** Returned value:		none
** 
*****************************************************************************/
void PWM_SetRiseDeadZone(uint16_t rdtimeticks)
{
	(*PWM).DTIM0.all = 20*rdtimeticks;
}


/*****************************************************************************
** Function name:		PWM_SetFallDeadZone
**
** Descriptions:		odd channel on falling edge
**
** parameters:			
**                	
**									rdtimeticks: deadtime us
**
** Returned value:		none
** 
*****************************************************************************/
void PWM_SetFallDeadZone(uint16_t rdtimeticks)
{
	(*PWM).DTIM1.all = 20*rdtimeticks;
}

/*****************************************************************************
** Function name:		PWM_EnableFaultProtect
**
** Descriptions:		Set PWM fault type and  interrupt
**
** parameters:			
**			faults: 		PWMFAULT, ADCFAULT, SWFAULT
**			cleartype:	PWM_MANUAL, PWM_AUTOMATIC
**			faultint:		DISINT, ENINT
**
** Returned value:		none
** 
*****************************************************************************/
void PWM_EnableFaultProtect(uint8_t faults, uint8_t cleartype, uint8_t faultint)
{
	switch(faults)
	{
		case 0:
			(*PWM).FCTRL.bit.FMODE0 = cleartype;
			(*PWM).FCTRL.bit.FIE0 = faultint;
			break;
		case 1:
			(*PWM).FCTRL.bit.FMODE1 = cleartype;
			(*PWM).FCTRL.bit.FIE1 = faultint;
			break;
		case 2:
			(*PWM).FCTRL.bit.FMODE2 = cleartype;
			(*PWM).FCTRL.bit.FIE2 = faultint;
			break;
		default:
			break;
	}
}

/*****************************************************************************
** Function name:		PWM_ClearFault
**
** Descriptions:		Clear PWM fault signal 
**
** parameters:			
**                	
**										none
**
** Returned value:		none
** 
*****************************************************************************/
void PWM_ClearFault(void)
{
	(*PWM).FLTACK.bit.FTACK0 = 1;
	(*PWM).FLTACK.bit.FTACK1 = 1;
	(*PWM).FLTACK.bit.FTACK2 = 1;
}

/*****************************************************************************
** Function name:		PWM_Start
**
** Descriptions:		Enable PWM output
**
** parameters:			
**                	
**										none
**
** Returned value:		none
** 
*****************************************************************************/
void PWM_Start(void)
{
	// Enable PWM0
	(*PWM).CTRL.bit.PWMEN = 1;
	
	// UnMask PWM0~5
	(*PWM).CCTRL.bit.MSK0 = 0;
	(*PWM).CCTRL.bit.MSK1 = 0;
	(*PWM).CCTRL.bit.MSK2 = 0;
	(*PWM).CCTRL.bit.MSK3 = 0;
	(*PWM).CCTRL.bit.MSK4 = 0;
	(*PWM).CCTRL.bit.MSK5 = 0;

	
	// Reload signal output enable control
	(*PWM).CTRL.bit.CH0OUTEN = 1;
	(*PWM).CTRL.bit.CH1OUTEN = 1;
	(*PWM).CTRL.bit.CH2OUTEN = 1;
	(*PWM).CTRL.bit.CH3OUTEN = 1;
	(*PWM).CTRL.bit.CH4OUTEN = 1;
	(*PWM).CTRL.bit.CH5OUTEN = 1;
	
	//enable PWM0_0~5 pwm pad output
	(*PWM).ICCTRL.bit.PAD_EN = 1;
	
	//Load value
	(*PWM).CTRL.bit.LDOK = 1;
}

/*****************************************************************************
** Function name:		PWM_Stop
**
** Descriptions:		Disable PWM output
**
** parameters:			
**                	
**										none
**
** Returned value:		none
** 
*****************************************************************************/
void PWM_Stop(void)
{
	// Disable PWM0
	(*PWM).CTRL.bit.PWMEN = 0;
	
	// Mask PWM0~5
	(*PWM).CCTRL.bit.MSK0 = 1;
	(*PWM).CCTRL.bit.MSK1 = 1;
	(*PWM).CCTRL.bit.MSK2 = 1;
	(*PWM).CCTRL.bit.MSK3 = 1;
	(*PWM).CCTRL.bit.MSK4 = 1;
	(*PWM).CCTRL.bit.MSK5 = 1;

	
	// Reload signal output enable control
	(*PWM).CTRL.bit.CH0OUTEN = 0;
	(*PWM).CTRL.bit.CH1OUTEN = 0;
	(*PWM).CTRL.bit.CH2OUTEN = 0;
	(*PWM).CTRL.bit.CH3OUTEN = 0;
	(*PWM).CTRL.bit.CH4OUTEN = 0;
	(*PWM).CTRL.bit.CH5OUTEN = 0;
	
	//Disable PWM0_0~5 pwm pad output
	(*PWM).ICCTRL.bit.PAD_EN = 0;
	
	//Load value
	(*PWM).CTRL.bit.LDOK = 1;
}

/*****************************************************************************
** Function name:		PWM_Stop
**
** Descriptions:		Enable PWM Relaod Interrupt
**
** parameters:			
**                	
**										inttype: PWM_FULLINT, PWM_HALFINT
**
** Returned value:		none
** 
*****************************************************************************/
void	PWM_EnableInt(uint8_t inttype)
{
	(*PWM).CTRL.bit.PWMRIE = 1;
	(*PWM).CTRL.bit.HALF = inttype;
}

/*****************************************************************************
** Function name:		PWM_ADCTrigger
**
** Descriptions:		PWM signal trigger ADC
**
** parameters:			
**                	channel:PWM0~5
**
** Returned value:		none
** 
*****************************************************************************/
void PWM_ADCTrigger(uint8_t channel)
{
	uint8_t i;
	
	for(i=0;i<6;i++)
	{
		if(channel&(1<<i))
		{
			switch(i)
			{
				case 0:
					(*PWM).CTRL.bit.CH0OUTEN = 1;
					break;
				case 1:
					(*PWM).CTRL.bit.CH1OUTEN = 1;	
					break;
				case 2:
					(*PWM).CTRL.bit.CH2OUTEN = 1;
					break;
				case 3:
					(*PWM).CTRL.bit.CH3OUTEN = 1;
					break;
				case 4:
					(*PWM).CTRL.bit.CH4OUTEN = 1;
					break;
				case 5:
					(*PWM).CTRL.bit.CH5OUTEN = 1;
					break;
				default:
					break;
			}
		}
	}	
}

/******************************************************************************
**                            End Of File
******************************************************************************/
