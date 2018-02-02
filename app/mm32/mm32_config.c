#include "config.h"

static void mm32_led_gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOB, ENABLE);  

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	//PD0,PD1/OSC REMAP TO GPIO
	//GPIOD->AFRL = 0x00000011;
	
	
	
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void exit_irq_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;  		  
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能GPIOA和复用功能时钟
    
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_5;	 //PA.0     POWER KEY   PA.5      RTC INT   
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;//上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化IO
	
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_11|GPIO_Pin_12;	 //PA.11:USB_DET     PA.12: TOUCH2     
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;//上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化IO
	
    //使用外部中断方式
    SYSCFG_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0|GPIO_PinSource5|GPIO_PinSource11|GPIO_PinSource12);	//中断线0连接GPIOA.0  GPIOA.5  

    EXTI_InitStructure.EXTI_Line = EXTI_Line5|EXTI_Line11;	//设置按键所有的外部线路   
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//设外外部中断模式:EXTI线路为中断请求
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  //上升/下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	// 初始化外部中断

	EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//ÉèÖÃ°´¼üËùÓÐµÄÍâ²¿ÏßÂ·   
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //ÉÏÉý/ÏÂ½µÑØ´¥·¢
    EXTI_Init(&EXTI_InitStructure);	// ³õÊ¼»¯Íâ²¿ÖÐ¶Ï
		
    EXTI_InitStructure.EXTI_Line = EXTI_Line12;	//设置按键所有的外部线路   
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //上升/下降沿触发
    EXTI_Init(&EXTI_InitStructure);	// 初始化外部中断
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn; //使能按键所在的外部中断通道  POWER KEY
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2; //从优先级2级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
    NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn; //使能按键所在的外部中断通道  RTC_INT   
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2; //从优先级2级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
    NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

static void mm32_gpio_init_t(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);  //¿ªÆôGPIOA,GPIOBÊ±Ö
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15| GPIO_Pin_14;  //POWER C600 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOA, ENABLE);  

	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_4|GPIO_Pin_1|GPIO_Pin_6;      //VBT_MCU_INT, motor check
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*½«PA4ÅäÖÃÎªÄ£ÄâÊäÈë*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*	5V_DRV_EN	*/
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;      //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	

	/*	motor gpio init	*/
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_10 | GPIO_Pin_13;      //motor_4, motor_1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource13,GPIO_AF_7);

	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_1| GPIO_Pin_3;      //motor_2, motor_3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	mm32_led_gpio_init();	
	exit_irq_init();

}

void gpio_init(void)
{
	mm32_gpio_init_t();
}

void uni_pwm_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //¿ªÆôGPIOBÊ±ÖÓ

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);
	//SYSCFG->CFGR|=0x1<<11;
	//   GPIOA->AFRH=0x2;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9; //TIM1_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //¸´ÓÃÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	TIM_TimeBaseStructure.TIM_Period = 99; //ÉèÖÃÔÚÏÂÒ»¸ö¸üÐÂÊÂ¼þ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =0; //ÉèÖÃÓÃÀ´×÷ÎªTIMxÊ±ÖÓÆµÂÊ³ýÊýµÄÔ¤·ÖÆµÖµ  ²»·ÖÆµ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //¸ù¾ÝTIM_TimeBaseInitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯TIMxµÄÊ±¼ä»ùÊýµ¥Î»

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //±È½ÏÊä³öÊ¹ÄÜ
	TIM_OCInitStructure.TIM_Pulse = 0; //ÉèÖÃ´ý×°Èë²¶»ñ±È½Ï¼Ä´æÆ÷µÄÂö³åÖµ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //Êä³ö¼«ÐÔ:TIMÊä³ö±È½Ï¼«ÐÔ¸ß
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //¸ù¾ÝTIM_OCInitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèTIMx
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //¸ù¾ÝTIM_OCInitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèTIMx

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1Ô¤×°ÔØÊ¹ÄÜ
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1Ô¤×°ÔØÊ¹Ä

	TIM_ARRPreloadConfig(TIM1, ENABLE); //Ê¹ÄÜTIMxÔÚARRÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷

	TIM_CtrlPWMOutputs(TIM1, ENABLE); 

	TIM_Cmd(TIM1, ENABLE);  //Ê¹ÄÜTIM1


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
}

void uni_adc_init(void)
{
	ADC_InitTypeDef  ADC_InitStructure;
 // GPIO_Configuration();
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    /* Initialize the ADC_PRESCARE values */
    ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;   //48M / 16= 3M
    /* Initialize the ADC_Mode member */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Single_Period;
    /* Initialize the ADC_ContinuousConvMode member */
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    /* Initialize the ADC_DataAlign member */
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    /* Initialize the ADC_ExternalTrigConv member */
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//ADC1 Í¨µÀ1£¬
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    /*ÆÁ±ÎËùÓÐÍ¨µÀ*/
    ADC_RegularChannelConfig(ADC1, DISABLE_ALL_CHANNEL , 0, 0); 
    /*Ê¹ÄÜÑ¡ÖÐÍ¨µÀ,ºóÃæ²ÎÊý±£Áô*/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_13_5Cycles); 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_13_5Cycles); 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_13_5Cycles); 
    
    ADC_Cmd(ADC1, ENABLE); 
}


static void UART2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	
	
#ifdef Debug	
    
    
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  
    
    
    NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;		
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
    NVIC_Init(&NVIC_InitStructure);	
    
    
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1);
    
    UART_InitStructure.UART_BaudRate = bound;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;	
    
    UART_Init(UART1, &UART_InitStructure);      
    UART_ITConfig(UART1, UART_IT_RXIEN|UART_IT_TXIEN, ENABLE);
    UART_Cmd(UART1, ENABLE);                     
    
    //UART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //UART1_RX	  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
#endif


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART2, ENABLE);	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  
    

    NVIC_InitStructure.NVIC_IRQChannel = UART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;		
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
    NVIC_Init(&NVIC_InitStructure);	
    
  
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_1);
    
    UART_InitStructure.UART_BaudRate = 115200;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;	
    
    UART_Init(UART2, &UART_InitStructure); 
    UART_ITConfig(UART2, UART_IT_RXIEN, ENABLE);
    UART_Cmd(UART2, ENABLE);                    
    
    //UART2_TX   GPIOA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void _uart_init(void)
{
	UART2_Init();
}

void clock_init(void)
{
}

void uni_wdg_init(void)
{
	Write_Iwdg_ON(IWDG_Prescaler_32,0x7ff);
}

static void Tim2_Init(u16 Prescaler,u16 Period)
{
    TIM_TimeBaseInitTypeDef TIM_StructInit;
    NVIC_InitTypeDef NVIC_StructInit;
    
    /*使能TIM1时钟,默认时钟源为PCLK1(PCLK1未分频时不倍频,否则由PCLK1倍频输出),可选其它时钟源*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
       
    TIM_StructInit.TIM_Period=Period;                                                                      //ARR寄存器值
    TIM_StructInit.TIM_Prescaler=Prescaler;                                                                //预分频值
    /*数字滤波器采样频率,不影响定时器时钟*/
    TIM_StructInit.TIM_ClockDivision=TIM_CKD_DIV1;                                                         //采样分频值
    TIM_StructInit.TIM_CounterMode=TIM_CounterMode_Up;                                                     //计数模式
    TIM_StructInit.TIM_RepetitionCounter=0;
    
    TIM_TimeBaseInit(TIM2, &TIM_StructInit);
    
    /* 配置定时器3中断通道及优先级 */
    NVIC_StructInit.NVIC_IRQChannel=TIM2_IRQn;
    NVIC_StructInit.NVIC_IRQChannelPriority=3;
    NVIC_StructInit.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_StructInit);
    
    /*更新定时器时会产生更新时间,清除标志位*/
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    /*允许定时器2更新中断*/
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
   //TIM_Cmd(TIM2, ENABLE);	
}

static void Tim16_Init(void)
{
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
}

void uni_timer_init(void)
{
	Tim16_Init();		// used for software interrupt.
	Tim2_Init(47,99);	// 100us one cycle, used for motor PWM signal.
}

