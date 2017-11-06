/* Includes ------------------------------------------------------------------*/

#include "c32f0xx.h"
#include "c32f0xx_timer.h"
#include "c32f0xx_sys.h"

extern uint32_t SystemCoreClock;
/*****************************************************************************
Function Name	SYS_SystemInitial
Function Definition	void SYS_SystemInitial(void)
Function Description	Initial system-configure clocl, IO pin function
Input Parameters	*ct: pointer to CT16B0/CT16B1
tickpersecond: timer count frequency, less than system clock
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void SYS_SystemInitial(void)
{
	//WDT->MOD = 0x02;
	//Configure MainClock and SystemClock(defaul MainClock=48MHz and SystemClock=24MHz)	
	if(C_CIB->RESERVED0 == 0x55aaaa55)
	{
		SYSCON->IRCCTRL.all = 0x24;//C_CIB->IRCTRIM;//晶振频偏纠下  500Hz 正常为 480Hz输出频率
	}
	else 
	{
		//SYSCON->IRCCTRL.all = 0x1d;
	}
	
	SYS_SelectMainClkSrc(IRC_CLK);	
	SYS_SetAHBClkDivider(1);
	
	return;
}
/*****************************************************************************
Function Name	SYS_SelectMainClkSrc
Function Definition	Void SYS_SelectMainClkSrc(uint8_t src)
Function Description	Select Main clcok source
Input Parameters	src:IRC_CLK
								WDT_CLK
Other are invalid
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_SelectMainClkSrc(uint8_t src)
{
	//switch main clk source
	SYSCON->MAINCLKUEN.bit.ENA = 1;					//disable main clk update
	
	//switch main clk source to Specifyed source
	if (IRC_CLK == src)
    {
        SYSCON->MAINCLKSEL.bit.SEL = 0;
    }   
    else if (WDT_CLK == src)
    {
        SYSCON->MAINCLKSEL.bit.SEL = 2;
    }
    else
    {
    
    }
		
	SYSCON->MAINCLKUEN.bit.ENA = 0; 						//enable main clk update
	
	SystemCoreClockUpdate();
	return;
}
/*****************************************************************************
Function Name	SYS_SetAHBClkDivider
Function Definition	Void SYS_SetAHBClkDivider (uint8_t div)
Function Description	Set divider, that divides the main clock to provide the system clock to the core, memories,and the peripherals.
Input Parameters	div:1~255, 0 is forbidented
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_SetAHBClkDivider (uint8_t div)
{
	if (((*SYSCON).MAINCLKSEL.bit.SEL == 0)&&(div < 2))
	{
		(*C_FMC).FLASH_RDCYC = 1;
	}
	
	(*SYSCON).SYSAHBCLKDIV.bit.DIV = div;
	SystemCoreClockUpdate();   
	return;
}
/*****************************************************************************
Function Name	SYS_SelectWDTClkSrc
Function Definition	Void SYS_SelectWDTClkSrc(uint8_t src)
Function Description	Select clock source as WDT counter clock.
Input Parameters	src:IRC_CLK
                                 WDT_CLK
Other are invalid
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_SelectWDTClkSrc(uint8_t src)
{
	if (IRC_CLK == src)
    {
        (*SYSCON).WDTOSCCTRL.bit.WDTCLKSRC = 0;
    }   
	else// (WDT_CLK == src)
    {
        (*SYSCON).WDTOSCCTRL.bit.WDTCLKSRC = 1;
    }   
		
	return;
}

/*****************************************************************************
Function Name	SYS_SelectClkOutSrc
Function Definition	SYS_SelectClkOutSrc(uint8_t src)
Function Description	Select clock source as clock output
Input Parameters	src:   IRC_CLK
                                    SYS_CLK
                                    WDT_CLK
                                    MAIN_CLK
                                    RTC_CLK
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_SelectClkOutSrc(uint8_t src)
{
	switch (src)
	{
		case IRC_CLK:
			src = 0;
    		break;
		case SYS_CLK:
			src = 1;
	    	break;
		case WDT_CLK:
			src = 2;
    		break;
		case MAIN_CLK:
			src = 3;
    		break;
		case RTC_CLK:
			src = 4;
    		break;
		default:
			return;
	}
	//switch clock
	(*SYSCON).CLKOUTUEN.bit.ENA = 1;
	(*SYSCON).CLKOUTCLKSEL.bit.SEL = src; 	//select clk out source
	(*SYSCON).CLKOUTUEN.bit.ENA = 0;
 
	return;
}

/*****************************************************************************
Function Name	SYS_SetClkOutDivider
Function Definition	SYS_SetClkOutDivider(uint8_t src)
Function Description	Select clock source as clock output
Input Parameters	div:Clock output divider, valid 1~255 
											0: clock is disable
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_SetClkOutDivider(uint8_t div)
{
	(*SYSCON).CLKOUTDIV.bit.DIV = div;
 
	return;
}

/*****************************************************************************
Function Name	SYS_EnableClkOut
Function Description	Enable clock output
Input Parameters	No
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_EnableClkOut(void)
{
	(*IOCON).PIOA_8.all = 0x235;   							//enable clock out pin 
	return;
}

/*****************************************************************************
Function Name	SYS_DisableClkOut
Function Definition	Void SYS_DisableClkOut(void)
Function Description	Disable clock output
Input Parameters	No
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_DisableClkOut(void)
{
	(*SYSCON).CLKOUTDIV.bit.DIV = 0;
	(*IOCON).PIOA_8.all = 0x230;   							//disable clock out pin 
 
	return;
}
/*****************************************************************************
Function Name	SYS_SetIOFilter
Function Definition	Void SYS_SetIOFilter(uint8_t ticks)
Function Description	Set IO filter 
Input Parameters	Ticks: 1~255, every ticks sample IO input
0, no sample
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_SetIOFilter(uint8_t ticks)
{
	(*SYSCON).IOCONFIGCLKDIV.bit.DIV = ticks;
 
	return;
}
/*****************************************************************************
Function Name	SYS_PowerDownAngIP
Function Definition	SYS_PowerDownAngIP(uint32_t angips)
Function Description	Shut down specific analog IPs power supply
Input Parameters	Angips: analog IPs bit
                    Valid bit values are PD_IRC, PD_BOD, PD_ADC, PD_RTC, PD_WDTOSC, PD_LCD, PD_TOUCH
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_PowerDownAngIP(uint32_t angips)
{
	(*SYSCON).PDRUNCFG.all |= angips;
 
	return;
}
/*****************************************************************************
Function Name	SYS_PowerUpAngIP
Function Definition	SYS_PowerUpAngIP(uint32_t angips)
Function Description	Power up specific analog IPs power supply
Input Parameters	Angips: analog IPs bit 
Valid bit values are PD_IRC, PD_BOD, PD_ADC, PD_RTC, PD_WDTOSC, PD_LCD, PD_TOUCH
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_PowerUpAngIP(uint32_t angips)
{
	(*SYSCON).PDRUNCFG.all &= ~angips;
 
	return;
}
/*****************************************************************************
Function Name	SYS_EnablePhrClk
Function Definition	void SYS_EnablePhrClk(uint32_t phrs)
Function Description	Enable specific peripherals clock
Input Parameters	phrs: peripherals clock bit 
Valid bit values are AHB_IIC, AHB_CRC, AHB_CT16B0, AHB_CT16B1, AHB_CT16B2, 
	AHB_CT16B3, AHB_SPI, AHB_UART0, AHB_UART1,AHB_ADC, AHB_WDT, AHB_IOCON, 
	AHB_PWM,AHB_LCD, AHB_RTC,AHB_TOUCH 
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_EnablePhrClk(uint32_t phrs)
{
	(*SYSCON).SYSAHBCLKCTRL.all |= phrs;
 
	return;
}
/*****************************************************************************
Function Name	SYS_DisablePhrClk
Function Definition	void SYS_DisablePhrClk(uint32_t phrs)
Function Description	Disable specific peripherals clock
Input Parameters	phrs: peripherals clock bit 
Valid bit values are AHB_IIC, AHB_CRC, AHB_CT16B0, AHB_CT16B1, AHB_CT16B2, 
AHB_CT16B3, AHB_SPI, AHB_UART0, AHB_UART1,AHB_ADC, AHB_WDT, AHB_IOCON, 
AHB_PWM,AHB_LCD, AHB_RTC,AHB_TOUCH 
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_DisablePhrClk(uint32_t phrs)
{
	(*SYSCON).SYSAHBCLKCTRL.all &= ~phrs;
 
	return;
}
/*****************************************************************************
Function Name	SYS_ResetPhr
Function Definition	void SYS_ResetPhr(uint32_t phrs)
Function Description	Reset specific peripherals 
Input Parameters	phrs: peripherals clock bit 
Valid bit values are RTS_IIC, RST_CRC, RST_CT16B0, RST_CT16B1, RST_CT16B2, 
RST_CT16B3, RST_SPI, RST_UART0, RST_UART1,RST_ADC, RST_WDT, 
RST_IOCON, RST_PWM,RST_LCD, RST_RTC,RST_TOUCH 
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_ResetPhr(uint32_t phrs)
{
	(*SYSCON).PRESETCTRL.all &= ~phrs;
	(*SYSCON).PRESETCTRL.all |= phrs;
	return;
}
/*****************************************************************************
Function Name	SYS_ResetSystem
Function Definition	void SYS_ResetSystem(void)
Function Description	Software Reset system
Input Parameters	No
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_ResetSystem(void)
{
	NVIC_SystemReset();
}
/*****************************************************************************
Function Name	SYS_BODResetSystem
Function Definition	void SYS_BODResetSystem (uint8_t bodsel)
Function Description	Enable BOD and config BOD Reset option
Input Parameters	Bodsel: 
                                      BOD_INT
                                      BOD_RESET
Other values are invalid
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_BODResetSystem (uint8_t bodsel)
{
	if (BOD_RESET == bodsel)
	{
		SYSCON->BODCTRL.bit.BODRSTEN = 1 ;
	}
	else
	{
		SYSCON->BODCTRL.bit.BODRSTEN  = 0 ;
		SYSCON->BODCTRL.bit.BODINTCLR = 1;
	}
	return;
}
/*****************************************************************************
Function Name	SYS_GetResetStatus
Function Definition	uint32_t SYS_GetResetStatus(void)
Function Description	Return reset status
Input Parameters	No
Return Value	Reset status
				Bit0 	POR 
				Bit1 	EXTRST
				Bit2 	WDT 
				Bit3 	BOD 
				Bit4 	SYSRST
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
uint32_t SYS_GetResetStatus(void)
{
	return SYSCON->SYSRESSTAT.all;
}
/*****************************************************************************
Function Name	SYS_ClearResetStatus
Function Definition	void SYS_ClearResetStatus (void)
Function Description	Clear reset status
Input Parameters	No
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_ClearResetStatus (void)
{
	SYSCON->SYSRESSTAT.all = 0xFF;
	return;
}
/*****************************************************************************
Function Name	SYS_EnterSleep
Function Definition	void SYS_EnterSleep (void)
Function Description	Enter sleep mode
Input Parameters	    No
Return Value	        No
Condition	            No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_EnterSleep (void)
{
	//Sleep
	(*PMU).PCON.bit.DPDEN = 0;			//sleep
	SCB->SCR &= ~(1<<2);			    //sleep
	
	//issue sleep command
	__WFI();	//enter sleep
	return;
}
/*****************************************************************************
Function Name	SYS_SetDeepSleepWakeupPin
Function Definition	void SYS_SetDeepSleepWakeupPin (uint16_t selio, uint16_t edge)
Function Description	Set deep sleep wake up pin
Input Parameters	Selio:GPIO_PIN0 ~ GPIO_PIN11
                    Edge: FALL_EDGE/RISE_EDGE
Return Value	No
Condition	    No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_SetDeepSleepWakeupPin (uint16_t selio, uint16_t edge)
{
	(*SYSCON).DSWAKEEN.all		= selio;		//enable the pin
	
	if(edge == FALL_EDGE)
	{
		(*SYSCON).DSWAKECTL.all &=~ selio;
	}
	else
	{
		(*SYSCON).DSWAKECTL.all	= selio;			//wake up signal
	}
	
	NVIC_EnableIRQ(WAKEUP_IRQn); 				//enable Wakeup IRQ
	
	return;
}
/*****************************************************************************
Function Name	SYS_ResetDeepSleepWakeupPin
Function Definition	void SYS_ResetDeepSleepWakeupPin(void)
Function Description	Reset all deep sleep wake pin . The wakeup pin must be reset before use.
Input Parameters	No
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_ResetDeepSleepWakeupPin(void)
{
	//(*SYSCON).DSWAKEEN.all		= 0;
	(*SYSCON).DSWAKECLR.all	= 0xFFF;			//clear int status

}
/*****************************************************************************
Function Name	SYS_EnterDeepSleep
Function Definition	void SYS_EnterDeepSleep (uint32_t deepsleepconfig, uint32_t wakeupconfig)
Function Description	Enter deep sleep mode
Input Parameters	deepsleepconfig: config PDSLEEPCFG. Avalable bit PD_BOD, PD_RTCOSC and PD_WDTOSC. 
									 If WDTOSC_PD bit is not set, the system won't wakeup except power up
			        Wakeupconfig:    config PDAWAKECFG. Avalable bit PD_IRC, PD_BOD, PD_ADC, PD_RTC, PD_WDTOSC, PD_LCD, PD_TOUCH. 
			                         If bit is set, the IP will power down after wake up from deep sleep.
Return Value	No
Condition	    No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_EnterDeepSleep(uint32_t deepsleepconfig, uint32_t wakeupconfig)
{
	uint32_t i;
	
	//if WDT osc is not powered, power osc and wait for stable
	if ((*SYSCON).PDRUNCFG.bit.WDTOSC_PD != 0)		
	{
		(*SYSCON).PDRUNCFG.bit.WDTOSC_PD = 0;
		
		//wait for a while
		while(i++<0xff);
	}
	
	//set WDT osc as back up clk for powerdown
	(*SYSCON).PDSLEEPCFG.all = deepsleepconfig;
	
	(*SYSCON).PDAWAKECFG.all = wakeupconfig;
	//switch main clk to WDT osc
	(*SYSCON).MAINCLKUEN.bit.ENA = 1;
	(*SYSCON).MAINCLKSEL.bit.SEL = 2;	//WDTOSC
	(*SYSCON).MAINCLKUEN.bit.ENA = 0;

	//Deep Sleep
	SCB->SCR |= (1<<2);								//deep sleep
	
	//issue deep sleep command
	__WFI();	//enter deep sleep
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	
	//switch back to IRC
	(*SYSCON).MAINCLKUEN.bit.ENA = 1;
	(*SYSCON).MAINCLKSEL.bit.SEL = 0;
	(*SYSCON).MAINCLKUEN.bit.ENA = 0;

	return;
}
/*****************************************************************************
Function Name	SYS_EnterPowerDown
Function Definition	void SYS_EnterPowerDown (uint32_t powerdownconfig)
Function Description	Enter sleep mode
Input Parameters	powerdownconfig : config PDSLEEPCFG. Avalable bit  PD_RTCOSC . 
	If bit is set, the IP will power down duing deep sleep.
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_EnterPowerDown(uint32_t powerdownconfig)
{	
	if (powerdownconfig & PD_RTCOSC)
    {
        (*SYSCON).PDSLEEPCFG.bit.RTCOSC_PD	= 0;
    }   
	else
    {
        (*SYSCON).PDSLEEPCFG.bit.RTCOSC_PD	= 1;
    }   
	
	(*SYSCON).PDSLEEPCFG.bit.BOD_PD     = 1;
	(*SYSCON).PDSLEEPCFG.bit.WDTOSC_PD	= 1;
	
	//Power Down
	(*PMU).PCON.bit.DPDEN = 1;
	
	SCB->SCR |= (1<<2);	//deep sleep	
	__WFI();	//enter power down
}
/*****************************************************************************
Function Name	SYS_IOConfig
Function Definition	void SYS_IOConfig(IOCON_REG *pio, uint32_t config)
Function Description	Config IO pin
Input Parameters	*pio : pointer to IOCON. IOCON->PIOA_0~ IOCON->PIOA_15, IOCON->PIOB_0~ IOCON->PIOB_15, IOCON->PIOC_7~ IOCON->PIOA_15
                             Config: IO config value
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_IOConfig(union IOCON_REG *pio, uint32_t config)
{
	pio->all = config;
	return;
}
/*****************************************************************************
Function Name	SYS_IOSetPullup
Function Definition	void SYS_IOSetPullup(IOCON_REG *pio)
Function Description	Config IO pin pull up
Input Parameters	*pio : pointer to IOCON抯 IO. IOCON->PIOA_0~ IOCON->PIOA_15, IOCON->PIOB_0~ IOCON->PIOB_15, IOCON->PIOC_7~ IOCON->PIOA_15
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_IOSetPullup(union IOCON_REG *pio)
{
	pio->bit.PDE = 0;
	pio->bit.PUE = 1;
	
	return;
}
/*****************************************************************************
Function Name	SYS_IOSetPulldown
Function Definition	void SYS_IOSetPulldown (IOCON_REG *pio)
Function Description	Config IO pin pull down
Input Parameters	*pio : pointer to IOCON抯 IO. IOCON->PIOA_0~ IOCON->PIOA_15, IOCON->PIOB_0~ IOCON->PIOB_15, IOCON->PIOC_7~ IOCON->PIOA_15
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_IOSetPulldown (union IOCON_REG *pio)
{
	pio->bit.PUE = 0;
	pio->bit.PDE = 1;
	return;
}
/*****************************************************************************
Function Name	SYS_IOSwitchtoAng
Function Definition	void SYS_IOSwitchtoAng (IOCON_REG *pio)
Function Description	Switch IO pin to analog function
Input Parameters	*pio : pointer to IOCON抯 IO. IOCON->PIOA_0~ IOCON->PIOA_15, IOCON->PIOB_0~ IOCON->PIOB_15, IOCON->PIOC_7~ IOCON->PIOA_15
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_IOSwitchtoAng (union IOCON_REG *pio)
{
	pio->bit.ADM = 0;
	return;
}
/*****************************************************************************
Function Name	SYS_IOSwitchtoDigit
Function Definition	void SYS_IOSwitchtoDigit (IOCON_REG *pio)
Function Description	Switch IO pin digital function
Input Parameters	*pio : pointer to IOCON抯 IO. IOCON->PIOA_0~ IOCON->PIOA_15, IOCON->PIOB_0~ IOCON->PIOB_15, IOCON->PIOC_7~ IOCON->PIOA_15
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_IOSwitchtoDigit (union IOCON_REG *pio)
{
	pio->bit.ADM = 1;
	return;
}
/*****************************************************************************
Function Name	SYS_IOEnableFilter
Function Definition	void SYS_IOEnableFilter (IOCON_REG *pio)
Function Description	Enable IO pin filter function
Input Parameters	*pio : pointer to IOCON抯 IO. IOCON->PIOA_0~ IOCON->PIOA_15, IOCON->PIOB_0~ IOCON->PIOB_15, IOCON->PIOC_7~ IOCON->PIOA_15
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_IOEnableFilter (union IOCON_REG *pio)
{
	pio->bit.S_MODE = 1;
	return;
}
/*****************************************************************************
Function Name	SYS_ IODisableFilter
Function Definition	void SYS_ IODisableFilter (IOCON_REG *pio)
Function Description	Disable IO pin filter function
Input Parameters	*pio : pointer to IOCON抯 IO. IOCON->PIOA_0~ IOCON->PIOA_15, IOCON->PIOB_0~ IOCON->PIOB_15, IOCON->PIOC_7~ IOCON->PIOA_15
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void SYS_IODisableFilter (union IOCON_REG *pio)
{
	pio->bit.S_MODE = 0;
	return;
}

/*****************************************************************************
Function Name	GPIO_IOCONInit
Function Definition	void  GPIO_IOCONInit(uint32_t port,uint32_t pin,GPIO_InitTypeDef config)
Function Description	Config IO
Input Parameters	port : pointer to GPIO device, IOCON_GPIOA/IOCON_GPIOB/IOCON_GPIOC
                            pin: PIN0~PIN15
                            config:  IO  Config
Return Value         None
Condition	#include sys.h
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/

void  SYS_IOCONInit(uint32_t port,uint32_t pin,GPIO_InitTypeDef config)
{
	uint8_t    i              = 0;
	uint8_t    port_offset    = 0;
	uint32_t   mask           = 0;
	uint32_t * addr           = (uint32_t *)IOCON_BASE;
	uint32_t * addr_tmp       = (uint32_t *)IOCON_BASE;

    port_offset = port/2;

	for( i=0 ; i<16 ; i++)
	{
		mask = ((uint32_t)0x00000001) << i;
		if(0 != (pin & mask))
		{
			addr_tmp   = addr + 16*port_offset;  //select port
			addr_tmp  += i ;                     //select pin
			*addr_tmp  = config.all;
		}
	}			
    
	return;	
}

