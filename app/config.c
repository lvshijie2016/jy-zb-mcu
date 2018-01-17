#include "config.h"


/**
  *****************************************************************************
  * @Name   : IO设置
  *
  * @Brief  : none
  *
  * @Input  : Value: gpio  -> IOCON_GPIOA | IOCON_GPIOB |IOCON_GPIOC 
										 pin   -> PIN0 ~ PIN12
										 function -> PA10_FUNC_PWM_OUT4 | PA6_FUNC_GPIO | PB6_FUNC_I2C_SCL|..........
										 I_O ->  IO_Output | IO_Input
										 HL  ->  IO_HIGH   | IO_LOW
  *
  * @Output : none
  *
  * @Return : 
  *******************************************************************************
	**/

void get_gpio(uint8_t gpio, uint16_t pin, uint8_t function,uint8_t I_O, uint8_t HL ,uint8_t edge)
{
#if defined C32F0
	GPIO_TypeDef 		*port;
	IRQn_Type 			IRQ;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	SYS_EnablePhrClk(AHB_IOCON);
	
	if(edge == PULL_UP_EN)
	{
		GPIO_InitStructure.bit.PDE    = PULL_DOWN_DISABLE;
		GPIO_InitStructure.bit.PUE    = PULL_UP_ENABLE;
	}else if(PULL_DOWN_EN == edge){
		
		GPIO_InitStructure.bit.PDE    = PULL_DOWN_ENABLE;
		GPIO_InitStructure.bit.PUE    = PULL_UP_DISABLE;
	}else{
		
		GPIO_InitStructure.bit.PDE    = PULL_DOWN_DISABLE;
		GPIO_InitStructure.bit.PUE    = PULL_UP_DISABLE;
	}
	GPIO_InitStructure.bit.CSE    = SCHMITT_ENABLE;
	GPIO_InitStructure.bit.INV    = INPUT_INVERT_DISABLE;
	GPIO_InitStructure.bit.SRM    = FAST_SLEW_RATE_MODE;
	GPIO_InitStructure.bit.ADM    = DIGIT_ENABLE;
	GPIO_InitStructure.bit.DRV    = LOW_DRIVE;
	GPIO_InitStructure.bit.OD     = OPEN_DRAIN_DISABLE;
	GPIO_InitStructure.bit.S_MODE = INPUT_FILTER_DISABLE;
	//GPIO_InitStructure.bit.IEN    = INPUT_DISABLE;
	
	switch(gpio) {
		case IOCON_GPIOA:
				port	 = GPIOA;
				IRQ  	 = GPIOA_IRQn;
				SYS_EnablePhrClk(AHB_GPIOA);
			break;
		case IOCON_GPIOB:
				port	 = GPIOB;
				IRQ 	 = GPIOB_IRQn;
				SYS_EnablePhrClk(AHB_GPIOB);
			break;
		case IOCON_GPIOC:
				port 		= GPIOC;
				IRQ 	 	= GPIOC_IRQn;
				SYS_EnablePhrClk(AHB_GPIOC);
			break;
			default:
		break;
	}
	
	GPIO_InitStructure.bit.FUNC = (function&0x0F);
	
	SYS_IOCONInit(gpio, pin, GPIO_InitStructure);
	
	
	if(function&0xF0) function = 0;  //特殊功能IO
	if(!function)
	{
		if(I_O == IO_Output)
		{
			GPIO_ConfigPinsAsOutput(port, pin);
			
			if(HL == IO_HIGH)
				port->SET.all |= pin;
				
			else if(HL == IO_LOW)
				port->CLR.all |= pin;	
			
		}
		else
		{
			NVIC_DisableIRQ(IRQ);
			GPIO_ConfigPinsAsInput(port, pin);
			
			if(HL == IO_HIGH) 
				GPIO_EnableInt(port, pin, RISE_EDGE);
			else
				GPIO_EnableInt(port, pin, FALL_EDGE);
			
			NVIC_EnableIRQ(IRQ);
		}
	}
#endif
}
	

void get_adc_gpio(uint8_t gpio, uint16_t pin, uint8_t function, uint8_t edge)	
{
	
#if defined C32F0	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	switch(gpio) {
		case IOCON_GPIOA:
				SYS_EnablePhrClk(AHB_GPIOA);
			break;
		case IOCON_GPIOB:
				SYS_EnablePhrClk(AHB_GPIOB);
			break;
		case IOCON_GPIOC:
				SYS_EnablePhrClk(AHB_GPIOC);
			break;
			default:
		break;
	}
	
	
	
	
	if(edge == PULL_UP_EN)
	{
		GPIO_InitStructure.bit.PDE    = PULL_DOWN_DISABLE;
		GPIO_InitStructure.bit.PUE    = PULL_UP_ENABLE;
	}else if(PULL_DOWN_EN == edge){
		
		GPIO_InitStructure.bit.PDE    = PULL_DOWN_ENABLE;
		GPIO_InitStructure.bit.PUE    = PULL_UP_DISABLE;
	}else{
		
		GPIO_InitStructure.bit.PDE    = PULL_DOWN_DISABLE;
		GPIO_InitStructure.bit.PUE    = PULL_UP_DISABLE;
	}
	
	GPIO_InitStructure.bit.CSE    = SCHMITT_ENABLE;
	GPIO_InitStructure.bit.INV    = INPUT_INVERT_DISABLE;
	GPIO_InitStructure.bit.SRM    = FAST_SLEW_RATE_MODE;
	GPIO_InitStructure.bit.ADM    = DIGIT_ENABLE;
	GPIO_InitStructure.bit.DRV    = LOW_DRIVE;
	GPIO_InitStructure.bit.OD     = OPEN_DRAIN_DISABLE;
	GPIO_InitStructure.bit.S_MODE = INPUT_FILTER_DISABLE;
	GPIO_InitStructure.bit.FUNC  = function;
	SYS_IOCONInit(gpio, pin, GPIO_InitStructure);
	
	#endif

}	
	

void gpio_init_t(void)
{
#if defined C32F0	
	NVIC_SetPriority(GPIOA_IRQn,1);
	/***************TOUCH2_DRV****************************/
	get_gpio(IOCON_GPIOA,	PIN12,	PA12_FUNC_GPIO,	IO_Input,	IO_HIGH, PULL_UP_EN);  //->MOTO ADC 1
	
	/***************5V_DRV_EN****************************/
	get_gpio(IOCON_GPIOC, PIN3, (SPECIAL|PC3_FUNC_GPIO), IO_Output,	IO_LOW, PULL_UP_EN); //5V_DRV_EN
	
	/***************MCU_INT_KAR_PA14****************************/
	get_gpio(IOCON_GPIOA,	PIN14, (SPECIAL|PA14_FUNC_GPIO), IO_Output, IO_LOW, PULL_UP_EN);
	
	/**************POWER_KEY_PA0****************************/
	get_gpio(IOCON_GPIOA,	PIN0,	PA0_FUNC_GPIO,	IO_Input, IO_LOW, PULL_UP_EN);  
	
	/**************MCU_POWER_PA15****************************/
	get_gpio(IOCON_GPIOA,	PIN15,	PA15_FUNC_GPIO,	IO_Output, IO_DEFAULT, PULL_UP_EN); //KAR_POWER_ON_OFF  
	
	/***************RTC_INT_PA5****************************/
	get_gpio(IOCON_GPIOA,	PIN5,	PA5_FUNC_GPIO,	IO_Input, IO_LOW, PULL_UP_EN);  //RTC_INT_PB5
	//RTC _ IIC IO
	get_adc_gpio(IOCON_GPIOB,PIN6,PB6_FUNC_I2C_SCL,PULL_UP_EN);
	get_adc_gpio(IOCON_GPIOB,PIN7,PB7_FUNC_I2C_SDA,PULL_UP_EN);
	
	
/***************************LED******************************/	
	//GPIO1
	get_gpio(IOCON_GPIOB, PIN5, PB5_FUNC_GPIO,	IO_Output,	IO_LOW, PULL_UP_EN);
	//GPIO2
	get_gpio(IOCON_GPIOC, PIN0, PC0_FUNC_GPIO,	IO_Output,	IO_LOW, PULL_UP_EN); 
	
	//GPIO3
	get_gpio(IOCON_GPIOC, PIN1, PC1_FUNC_GPIO,	IO_Output,	IO_LOW, PULL_UP_EN);
	//GPIO4
	get_gpio(IOCON_GPIOB, PIN4, PB0_FUNC_GPIO,	IO_Output,	IO_LOW, PULL_UP_EN);
	
/***************************USB_DET GPIO******************************/
	get_gpio(IOCON_GPIOA, PIN11, PA11_FUNC_GPIO, IO_Input,	IO_HIGH, DISABLE_ALL_PULL);	

	/***************************MOTO******************************/
  //PWM
	get_gpio(IOCON_GPIOA, PIN10, PA10_FUNC_GPIO, IO_Output, IO_LOW, PULL_UP_EN);
	get_gpio(IOCON_GPIOB, PIN1, PB1_FUNC_GPIO, IO_Output, IO_LOW, PULL_UP_EN);
	
	//PH PL
	get_gpio(IOCON_GPIOA, PIN13, (SPECIAL|PA13_FUNC_GPIO), IO_Output, IO_LOW, PULL_UP_EN);
	get_gpio(IOCON_GPIOB, PIN3,	PB3_FUNC_GPIO, IO_Output, IO_LOW, PULL_UP_EN);
	
	//MOTO_ADC
	get_adc_gpio(IOCON_GPIOA, PIN1, PA1_FUNC_ADC_IN1,DISABLE_ALL_PULL);
	get_adc_gpio(IOCON_GPIOA, PIN6, PA6_FUNC_ADC_IN6,PULL_DOWN_EN);
	
/***************************VBAT_ADC******************************/
	get_adc_gpio(IOCON_GPIOA,	PIN4,	PA4_FUNC_ADC_IN4, DISABLE_ALL_PULL);
#elif defined MM32F031K6

    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);  //开启GPIOA,GPIOB时�
	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15| GPIO_Pin_14;  //POWER C600 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  
    
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_4;      //VBT_MCU_INT
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*将PA4配置为模拟输入*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		
	
#endif	
}

static void pwm_init_t(void)
{
#if defined C32F0
	PWM_Init(10, PWMDIV_1, PWM_DEPENDENT, PWM_EDGE, PWM_POSITIVE);
	PWM_SetRiseDeadZone(1);
	PWM_SetFallDeadZone(1);
	PWM_EnableFaultProtect(PWMFAULT, PWM_AUTOMATIC, DISINT);
	PWM_Start();
	
#elif defined MM32F031K6
	
	  GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //开启GPIOB时钟
    
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);
    //SYSCFG->CFGR|=0x1<<11;
 //   GPIOA->AFRH=0x2;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9; //TIM1_CH1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
    TIM_TimeBaseStructure.TIM_Period = 99; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
    TIM_TimeBaseStructure.TIM_Prescaler =0; //设置用来作为TIMx时钟频率除数的预分频值  不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
    
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1预装载使能
		TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1预装载使�
    
    TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
		
		TIM_CtrlPWMOutputs(TIM1, ENABLE); 
    
    TIM_Cmd(TIM1, ENABLE);  //使能TIM1
	
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOA, ENABLE);  
    
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //TIM3_CH3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM3_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    TIM_TimeBaseStructure.TIM_Period = 99; 
    TIM_TimeBaseStructure.TIM_Prescaler =0; 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
    
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
    TIM_OCInitStructure.TIM_Pulse = 0; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); 
    TIM_ARRPreloadConfig(TIM3, ENABLE); 
    TIM_Cmd(TIM3, ENABLE); 	
#endif
}


static void adc_init_t(void)
{ 
#if defined C32F0
	SYS_EnablePhrClk(AHB_ADC);
	ADC_DeInit();
	ADC_Init(10000UL);
	ADC_EnableChannels( ADC_CHN1_ENABLE);
	ADC_EnableChannels( ADC_CHN4_ENABLE);
	ADC_EnableChannels( ADC_CHN6_ENABLE);
	ADC_SelectTriggerSource(TRIGGERMODE,ADC_START_BY_SOFTWAER,ADC_TRIGGER_RISE_EDGE);
	ADC_SetupChannels(AD1, ADC_DR1);
	ADC_SetupChannels(AD6, ADC_DR6);
	ADC_SetupChannels(AD4, ADC_DR4);
//	ADC_EnableConversionInt(ADC_CHN1_ENABLE);
//	ADC_EnableConversionInt(ADC_CHN4_ENABLE);
//	ADC_EnableConversionInt(ADC_CHN6_ENABLE);
	ADC_WaitAdcReady();
	ADC_IssueSoftTrigger;
#elif defined MM32F031K6
	
	    ADC_InitTypeDef  ADC_InitStructure;
    
 // GPIO_Configuration();
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    /* Initialize the ADC_PRESCARE values */
    ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;   //48M / 16= 3M
    /* Initialize the ADC_Mode member */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Single;
    /* Initialize the ADC_ContinuousConvMode member */
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    /* Initialize the ADC_DataAlign member */
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    /* Initialize the ADC_ExternalTrigConv member */
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//ADC1 通道1，
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    /*屏蔽所有通道*/
    ADC_RegularChannelConfig(ADC1, DISABLE_ALL_CHANNEL , 0, 0); 
    /*使能选中通道,后面参数保留*/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_13_5Cycles); 
    
    ADC_Cmd(ADC1, ENABLE); 
#endif

}


static void timer0_init_t(void)
{
#if defined C32F0
	CT16B0_Init(TMR0, 100000UL);
	CT16B0_ResetTimerCounter(TMR0);
	/* ---------- Set Timer return to zero when it matches MR value ----------*/
	CT16B0_ConfigMatch(TMR0, 100, CT_MATCH_RESET_COUNTER);
	CT16B0_EnableInt(TMR0);
	NVIC_SetPriority(CT16B0_IRQn,2);
	NVIC_EnableIRQ(CT16B0_IRQn);
	CT16B0_START;
	
#elif defined MM32F031K6
	TIM_TimeBaseInitTypeDef TIM_StructInit;
  NVIC_InitTypeDef NVIC_StructInit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
	
	//1ms for 1 tick
	TIM_StructInit.TIM_Period=100;                                                                      //ARR寄存器值
	TIM_StructInit.TIM_Prescaler=SystemCoreClock/100000;                                                //预分频值
 
	
	TIM_StructInit.TIM_ClockDivision=TIM_CKD_DIV1;                                                      //采样分频值
	TIM_StructInit.TIM_CounterMode=TIM_CounterMode_Up;                                                     //计数模式
	TIM_StructInit.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(TIM16, &TIM_StructInit);
	
	TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM16, ENABLE);	
	
	/*更新定时器时会产生更新时间,清除标志位*/
	TIM_ClearFlag(TIM16, TIM_FLAG_Update);
	
	
	NVIC_StructInit.NVIC_IRQChannel=TIM16_IRQn;
	NVIC_StructInit.NVIC_IRQChannelCmd=ENABLE;
	NVIC_StructInit.NVIC_IRQChannelPriority = 0x02;	
	
	NVIC_Init(&NVIC_StructInit);
		
#endif
}


static void timer1_init_t_PWM(void)
{
#if defined C32F0
	CT16B0_Init(TMR1, 1000000UL);
	CT16B0_ResetTimerCounter(TMR1);
	/* ---------- Set Timer return to zero when it matches MR value ----------*/
	CT16B0_ConfigMatch(TMR1, 100, CT_MATCH_RESET_COUNTER);
	CT16B0_EnableInt(TMR1);
	NVIC_SetPriority(CT16B1_IRQn,3);
	NVIC_EnableIRQ(CT16B1_IRQn);
	CT16B1_STOP;
#endif
}



void wdt_init_t(uint8_t timer)
{
#if defined C32F0
	SYS_EnablePhrClk(AHB_WDT);
	SYS_SelectWDTClkSrc(WDT_CLK);//select watch dog clock 32k
	/* ------------------- Set WachDog Window size ---------------------------*/
   WDT_SetWindowValue(0,0xffffff);
	WDT_SetIntervalinMiliSec(timer*1000);// ~= 5 second		
	WDT_Enable_RESET;
	WDT->MOD.bit.WDLOCKEN = 0;
	WDT_Enable;
	WDT->MOD.bit.WDEN  = 1;
	WDT_Feed();	
#endif
}




static void UART1_Init(void)
{
#if defined C32F0
	#if defined( DeBug )
		SYS_EnablePhrClk(AHB_UART1);  
		get_adc_gpio(IOCON_GPIOA,PIN14,PA14_FUNC_TXD1,PULL_UP_EN);
		UART_Open(UART1, 115200, UART_NO_PARITY, UART_RX_NOT_EMPTY);
	#endif
	
	#if defined( V50_DeBug )
		SYS_EnablePhrClk(AHB_UART1);  
		get_adc_gpio(IOCON_GPIOA,PIN14,PA14_FUNC_TXD1,PULL_UP_EN);
		UART_Open(UART1, 115200, UART_NO_PARITY, UART_RX_NOT_EMPTY);
	#endif
#endif
}


void exit_irq_init()
{
	#if defined MM32F031K6
	
	  GPIO_InitTypeDef GPIO_InitStructure;  		  
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能GPIOA和复用功能时钟
    
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_5;	 //PA.0     POWER KEY   PA.5      RTC INT   
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;//上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化IO
	
	
    //使用外部中断方式
    SYSCFG_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0|GPIO_PinSource5);	//中断线0连接GPIOA.0  GPIOA.5  

    EXTI_InitStructure.EXTI_Line = EXTI_Line0|EXTI_Line5;	//设置按键所有的外部线路   
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//设外外部中断模式:EXTI线路为中断请求
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  //上升/下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	// 初始化外部中断
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn; //使能按键所在的外部中断通道  POWER KEY
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2; //从优先级2级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
    NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn; //使能按键所在的外部中断通道  RTC_INT   
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2; //从优先级2级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
    NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	#endif
	
}



void sys_init(void)
{
#if defined C32F0	
	SYS_SystemInitial();
	wdt_init_t(2);
	gpio_init_t();
	//get_gpio(IOCON_GPIOA,	PIN15,	PA15_FUNC_GPIO,	IO_Output, IO_DEFAULT, PULL_UP_EN); //KAR_POWER_ON_OFF  
	UART0_Init();
	UART1_Init();
	pwm_init_t();
	adc_init_t();
	#if defined( DeBug )
		LOG(LOG_DEBUG,"ADC Open  Successful.. \r\n");
	#endif
	timer0_init_t();
	timer1_init_t_PWM();
	IIC_Init(1,400,0xA0);
	
	#if defined( DeBug )
		LOG(LOG_DEBUG,"IIC Open  Successful.. \r\n");
	#endif
	RTC_Start();
	clear_all_event();
	led_mode_get_t(0x06,0xff,30 );
	moto_P();
#elif defined MM32F031K6
	gpio_init_t();
	timer0_init_t();
	pwm_init_t();
	exit_irq_init();
	UART2_Init();
	adc_init_t();
	IIC_Init(1,400,0xA0);
	RTC_Start();
	//UART0_Init();
	#if defined MM32F031K8
		UART1_Init();
	#endif
	
#endif
}

void sys_init_t(void)
{
#if defined C32F0
	SYS_SystemInitial();
	wdt_init_t(2);
	gpio_init_t();
	//get_gpio(IOCON_GPIOA,	PIN15,	PA15_FUNC_GPIO,	IO_Output, IO_DEFAULT, PULL_UP_EN); //KAR_POWER_ON_OFF 
	UART0_Init();
	UART1_Init();
	pwm_init_t();
	adc_init_t();
	timer0_init_t();
	timer1_init_t_PWM();
	IIC_Init(1,400,0xA0);
	//RTC_Start();
	//while(Rtc_Check());
	clear_all_event();
	led_mode_get_t(0x06,0xff,30 );
	moto_P();
	#if defined( DeBug )
		LOG(LOG_DEBUG,"mcu wakeup sysinit! .......... \r\n");
	#endif
#endif
}

void DisablePhrClk_t(void)
{
#if defined C32F0
	//IIC
	get_gpio(IOCON_GPIOB,PIN6,PB6_FUNC_GPIO,IO_Output,IO_HIGH,PULL_UP_EN);
	get_gpio(IOCON_GPIOB,PIN7,PB7_FUNC_GPIO,IO_Output,IO_HIGH,PULL_UP_EN);
	//LED
	get_gpio(IOCON_GPIOB,PIN0,PB0_FUNC_GPIO,IO_Output,IO_LOW,PULL_DOWN_EN);
	get_gpio(IOCON_GPIOA,PIN7,PA7_FUNC_GPIO,IO_Output,IO_LOW, PULL_DOWN_EN);
	get_gpio(IOCON_GPIOA,PIN9,PA9_FUNC_GPIO,IO_Output,IO_LOW, PULL_DOWN_EN);
	get_gpio(IOCON_GPIOA,PIN10,PA10_FUNC_GPIO,IO_Output,IO_LOW, PULL_DOWN_EN);
	//ADC
	get_gpio(IOCON_GPIOA,PIN1,PA1_FUNC_GPIO,IO_Output,IO_LOW, PULL_DOWN_EN);
	get_gpio(IOCON_GPIOA,PIN6,PA6_FUNC_GPIO,IO_Output,IO_LOW, PULL_DOWN_EN);
	get_gpio(IOCON_GPIOA,PIN4,PA4_FUNC_GPIO,IO_Output,IO_LOW, PULL_DOWN_EN);
	
	//USB_DET
	get_gpio(IOCON_GPIOA,PIN11,PA11_FUNC_GPIO,IO_Output,IO_LOW,PULL_DOWN_EN);
	
	//uart0
	get_gpio(IOCON_GPIOA,PIN2,PA2_FUNC_GPIO,IO_Output,IO_LOW, PULL_DOWN_EN);
	get_gpio(IOCON_GPIOA,PIN3,PA3_FUNC_GPIO,IO_Output,IO_LOW, PULL_DOWN_EN);
	
//	NVIC_DisableIRQ(GPIOA_IRQn);
//	SYS_DisablePhrClk(AHB_GPIOA);
	SYS_DisablePhrClk(AHB_GPIOC);
	SYS_DisablePhrClk(AHB_GPIOB);
	SYS_DisablePhrClk(AHB_WDT);
	SYS_DisablePhrClk(AHB_IOCON);
	SYS_DisablePhrClk(AHB_UART0);
	SYS_DisablePhrClk(AHB_ADC);
#endif
}


