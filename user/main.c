#include "config.h"

 
 



static _KAR_STATE	 	kar_state 	= KAR_STOP;
static _KAR_STATE	 	kar_state_t = MAX_KAR_STATE;
static _KEY_EVENT  		key_event 	= MAX_KEYS_EVENT;
static uint16_t key_timer = 0;
static uint8_t 	bat_value = 0;
static uint8_t  bat_alarm_flag = 0;
static bool		rest_WDT_flag = false;
static uint8_t  get_Com[10] = {0};


void configpad(uint32_t pinstat)
{
	IOCON->PIOA_0.all  = pinstat;
	IOCON->PIOA_1.all  = pinstat;
	IOCON->PIOA_2.all  = pinstat;
	IOCON->PIOA_3.all  = pinstat;
	IOCON->PIOA_4.all  = pinstat;
	IOCON->PIOA_5.all  = pinstat;
	IOCON->PIOA_6.all  = pinstat;
	IOCON->PIOA_7.all  = pinstat;
	IOCON->PIOA_8.all  = pinstat;
	IOCON->PIOA_9.all  = pinstat;
	IOCON->PIOA_10.all = pinstat;
	IOCON->PIOA_11.all = pinstat;
	IOCON->PIOA_12.all = pinstat;
	IOCON->PIOA_13.all = pinstat;
	IOCON->PIOA_14.all = pinstat;
	IOCON->PIOA_15.all = pinstat;

	IOCON->PIOB_0.all  = pinstat;
	IOCON->PIOB_1.all  = pinstat;
	IOCON->PIOB_2.all  = pinstat;
	IOCON->PIOB_3.all  = pinstat;
	IOCON->PIOB_4.all  = pinstat;
	IOCON->PIOB_5.all  = pinstat;
	IOCON->PIOB_6.all  = pinstat;
	IOCON->PIOB_7.all  = pinstat;

	IOCON->PIOC_0.all  = pinstat;
	IOCON->PIOC_1.all  = pinstat;
	IOCON->PIOC_2.all  = pinstat;
	IOCON->PIOC_3.all  = pinstat;
	
	
	
}
/**
  *****************************************************************************
  * @Name   : C6板关机初始化状态
  *
* @Brief  : none
  *
  * @Input  : none
  *
  * @Output : none
  *
  * @Return : 
  *****************************************************************************
**/
static void kar_off(void)
{
	#if defined( DeBug )
		LOG(LOG_DEBUG," 关闭所有外设\r\n");
	#else
		
		configpad(0);
		moto_P();
		led_mode_get_t(0x01,0x06,0xff );		//关机灯效
		UART_Close(UART0);
		UART_DisableInt(UART0);
		get_gpio(IOCON_GPIOA,	PIN2,	PA2_FUNC_GPIO,	IO_Output,	IO_LOW); //POWER_ON_OFF
		get_gpio(IOCON_GPIOA,	PIN3,	PA3_FUNC_GPIO,	IO_Output,	IO_LOW); //POWER_ON_OFF
		kar_state_t = KAR_STOP;
	#endif
	
	
	
	
	


}
/**
  *****************************************************************************
  * @Name   : C6板开机初始化状态
  *
  * @Brief  : none
  *
  * @Input  : none
  *
  * @Output : none
  *
  * @Return : 
  *****************************************************************************
**/

static void kar_on(void)
{
	
	#if defined( DeBug )
		LOG(LOG_DEBUG," 已经打开所有外设\r\n");
	#else
		get_gpio(IOCON_GPIOA,	PIN2,	PA2_FUNC_TXD0,	IO_Output,	IO_LOW); //POWER_ON_OFF
		get_gpio(IOCON_GPIOA,	PIN3,	PA3_FUNC_RXD0,	IO_Output,	IO_LOW); //POWER_ON_OFF
		UART_Open(UART0, 115200, UART_NO_PARITY, UART_RX_NOT_EMPTY);    
		NVIC_EnableIRQ(UART0_IRQn);
		kar_state_t = KAR_RUN;
	#endif
	
	
}

/**
  *****************************************************************************
  * @Name   : 按键信息
  *
  * @Brief  : none
  *
  * @Input  : none
  *
  * @Output : none
  *
  * @Return : 
  *****************************************************************************
**/

static void power_key_event(void)
{
	if(check_soft_timeout(TIMER_KEY))
	{	
		timer_delay_ms(4);
		if(!GPIO_GetPinState(GPIOA,PIN0))
		{
			key_timer++;
			if(key_timer == 100) 	
			{
				key_event = LONG_PRESS; //长按
				#if defined( DeBug )
					LOG(LOG_DEBUG,"按键长按 key_timer=%d \r\n",key_timer);
				#endif
				
			}
			else if(key_timer > 2000) 
			{
				key_event = RESET_PRESS; //复位
				key_timer = 0;
				Information_events	 &= 	(~POWER_KEY_EVENTS);
				//#if defined( DeBug )
					LOG(LOG_DEBUG,"开始复位...... ");
				//#endif
			}
			
			
			set_soft_timer(TIMER_KEY, 1);
		}else if(!key_timer || key_timer >= 100)
		{
			key_timer = 0;
			Information_events	 &= 	(~POWER_KEY_EVENTS);
			#if defined( DeBug )
				LOG(LOG_DEBUG," 退出按键事件\r\n");
			#endif
			
		}else if(key_timer <100)
		{
			key_event = SHORT_PRESS;	//短按
			key_timer = 0;
			Information_events	 &= 	(~POWER_KEY_EVENTS);
			#if defined( DeBug )
				LOG(LOG_DEBUG," 按键短按\r\n");
			#endif
		}
	}
}

/**
  *****************************************************************************
  * @Name   : 开关机事件
  *
  * @Brief  : none
  *
  * @Input  : none
  *
  * @Output : none
  *
  * @Return : 
  *****************************************************************************
**/

static void power_OFF_ON(void)
{
	switch(key_event) {
		
		case LONG_PRESS:		//长按事件
			switch(kar_state) {
				case KAR_STOP:
					if(bat_value > BAT_VALUE_LOW|| GPIO_GetPinState(GPIOA,PIN13))
					{
						kar_on();
						//kar_state = KAR_RUN;
						POWER_ON;
						kar_state_t = KAR_RUN;     		//记录开机事件
						led_mode_get_t(0x04,0xff,20 ); //开机灯效
						
						#if defined( DeBug )
							LOG(LOG_DEBUG,"开机中...... \r\n");
						#endif
						
					}else 
					{
						led_mode_get_t(0x01,0x03,8 );		//关机灯效
						#if defined( DeBug )
							LOG(LOG_DEBUG,"电量低...... \r\n");
						#endif
					}
						
					key_event = MAX_KEYS_EVENT;
			
				break;
				case KAR_RUN:
					//if(kar_state_t == MAX_KAR_STATE)
					//{
						led_mode_get_t(0x01,0xff,15 );		//关机灯效
				
						WriteUartBuf(KAR_POWER__OFF);
						UART_Send_t(KAR_POWER_OFF_COMMAND); //发送关机指令
						
						
						#if defined( DeBug )
							LOG(LOG_DEBUG,"关机中...... \r\n");
						#endif
					//}
					key_event = MAX_KEYS_EVENT;
				break;
				case KAR_DORMANCY:                   //触发唤醒事件  
				
					key_event = SHORT_PRESS;        //跳转到短按事件
					
					#if defined( DeBug )
						LOG(LOG_DEBUG,"正在唤醒...... \r\n");
					#endif
					
				break;

				default:break;
			}
			
			break;
		case SHORT_PRESS:				//短按事件  1.唤醒事件
			if(kar_state == KAR_DORMANCY)
			{
				KAR_DORMANCY_Enable;
				set_soft_timer(TIMER_POWER, 500);	
				key_event = KEYS_DORMANCY_STATE;  //跳转到执行唤醒事件
			}
			
			break;
			
		case KEYS_DORMANCY_STATE:		//执行唤醒事件
			if(check_soft_timeout(TIMER_POWER))
			{
				KAR_DORMANCY_Disable;
				key_event = MAX_KEYS_EVENT;
				kar_state_t = KAR_DORMANCY;
			}
			break;
			
			case RESET_PRESS:		
				#if defined( DeBug )
						LOG(LOG_DEBUG,"5秒后启动复位...... \r\n");
				#endif
				rest_WDT_flag = true;
				key_event = MAX_KEYS_EVENT;
				
			break;

		default:break;
	}
}


/**
  *****************************************************************************
  * @Name   : 外设中断事件处理
  *
  * @Brief  : none
  *
  * @Input  : none
  *
  * @Output : none
  *
  * @Return : 
  *****************************************************************************
**/

static void Handler_event(void)
{
	//DRV事件处理
	if(Information_events&DRV_EVENTS)
	{
		if(!all_event_flag.DRV)
		{
			DRV_Disable;
			all_event_flag.DRV = true;
		}
		else
		{
			DRV_Enable;
			all_event_flag.DRV = false;
		}
		Information_events	 &= 	(~DRV_EVENTS);
		#if defined( DeBug )
				LOG(LOG_DEBUG,"DRV_EVENTS =%d \r\n",all_event_flag.DRV);
		#endif
	}
	
	
	
	//电机1过流检测
	if(Information_events&MOTO_DET1_EVENTS)
	{
		
		moto_P();
		Information_events	 &= 	(~MOTO_DET1_EVENTS);
		#if defined( DeBug )
			LOG(LOG_DEBUG,"MOTO_DET1_EVENTS moto_P();\r\n");
		#endif
	}
	//电机2过流检测
	if(Information_events&MOTO_DET2_EVENTS)
	{
		Information_events	 &= 	(~MOTO_DET2_EVENTS);
		moto_P();
		
		#if defined( DeBug )
			LOG(LOG_DEBUG,"MOTO_DET2_EVENTS moto_P();\r\n");
		#endif
	}
	
	//时钟中断事件处理
	if(Information_events&RTC_INT_EVENTS)
	{
		
		switch(kar_state) {
		
			case KAR_STOP:
				
				key_event = LONG_PRESS;
			break;
			case KAR_RUN:
				
				
			break;
			case KAR_DORMANCY:
			
				key_event = SHORT_PRESS;
				
			break;

			default:break;
		
		}
		Information_events &= (~RTC_INT_EVENTS);
		
		#if defined( DeBug )
			LOG(LOG_DEBUG,"RTC_INT_EVENTS =%d\r\n",key_event);
		#endif
	}
	
	if(Information_events&USB_DET_EVENTS)  //USB_DET事件
	{
		if(kar_state == KAR_RUN)
		{
			WriteUartBuf(0x02);
			WriteUartBuf(bat_value);
			UART_Send_t(BAT_COMMAN);
		}
		
		Information_events &= (~USB_DET_EVENTS);
		
		#if defined( DeBug )
			LOG(LOG_DEBUG,"USB_DET_EVENTS \r\n");
		#endif
	}
	//按键处理
	if(Information_events&POWER_KEY_EVENTS)
	{

		power_key_event();
		//heartbeat_flag = 0;
		
	}

	
}

/**
  *****************************************************************************
  * @Name   : 获取运行模式
  *
  * @Brief  : none
  *
  * @Input  : none
  *
  * @Output : none
  *
  * @Return : 
  *****************************************************************************
**/

static void get_kar_run_state(uint8_t *Com)
{
	
	switch(Com[1]) {
		case KAR_RUN:
			kar_state = KAR_RUN;
		break;
		case KAR_STOP:
			kar_state = KAR_STOP;
			kar_off();
			POWER_OFF;
		break;
		case KAR_DORMANCY:
			kar_state = KAR_DORMANCY;	
		break;
		case KAR_RESET:
			EXCEPTION(EXCEPTION_3);
			kar_state_t = KAR_RUN;
		break;
	}
	
	kar_state_t	 =  MAX_KAR_STATE;
	#if defined( DeBug )
	LOG(LOG_DEBUG,"exit  kar_state_t  ->  =%d \r\n",kar_state_t);
	LOG(LOG_DEBUG," kar_state ->  =%d \r\n",kar_state);
	#endif
}




/**
  *****************************************************************************
  * @Name   : BAT状态监听
  *
  * @Brief  : none
  *
  * @Input  : none
  *
  * @Output : none
  *
  * @Return : 
  *****************************************************************************
**/

static void bat_monitoring(void)
{
	
	if(check_soft_timeout(TIMER_BAT)) 
	{
		bat_value = get_adc_value(0);
		
		if(bat_value	<	(BAT_VALUE_LOW-4) && kar_state == KAR_RUN)  //低于3.4V执行关机
		{ 
			if(!GPIO_GetPinState(GPIOA,PIN13)){//读取是否在充电状态
				key_event = LONG_PRESS;						 //发出按键信号
				#if defined( DeBug )
					LOG(LOG_DEBUG,"电量低于%5开始关机\r\n");
				#endif
			}
				
		}
		else if(bat_value < (BAT_VALUE_LOW+5) && !bat_alarm_flag  &&  kar_state == KAR_RUN) //低于10%开始报警
		{
			WriteUartBuf(0x01);
			WriteUartBuf(bat_value);
			UART_Send_t(BAT_COMMAN);  
			bat_alarm_flag = 60/5;
		}
		
		(bat_alarm_flag)? bat_alarm_flag--	:	0;
		
		set_soft_timer(TIMER_BAT,5000); //5秒读取一次电量值
		#if defined( DeBug )
			LOG(LOG_DEBUG,"BAT_value=%d \r\n",bat_value);
		#endif
		
		
		
	}

}



static void kar_connect(void)
{
	
	uart0_get_cmd(get_Com);
	if(kar_state_t == KAR_RUN)
	{
		switch(get_Com[0]) {

			case HANDSHAKE_COMMAND:
				WriteUartBuf(POISON_VERSION);
				WriteUartBuf(FIRMWARE_VERSION);
				UART_Send_t(HANDSHAKE_COMMAND);
				
				#if defined( DeBug )
					LOG(LOG_DEBUG,"FIRMWARE_VERSION=%d.%d\r\n",FIRMWARE_VERSION/10,FIRMWARE_VERSION%10);
				#endif

			break;
			case KAR_RUN_STATE:
			
				get_kar_run_state(get_Com);
				
				
			break;
			case HEARTBEAT_COMMAND:

			
			break;
			case LIGHT_COMMAND:
				led_mode_get(get_Com);

			
			break;
			
			case ALARM_COMMAND:
				Set_Alarm_Clock(get_Com);

			break;
			case MOTO_COMMAND:
				GetMotoCom(get_Com);
			break;
			case TIMER_DATA_COMMAND:
				Set_date_timer(get_Com);
			
			break;
			
			case GET_TIMER_COMMAND:
				Get_date_timer();
			break;
			case BAT_COMMAN:
				WriteUartBuf(0x00);
				WriteUartBuf(bat_value);
				UART_Send_t(BAT_COMMAN);
			break;
			default:break;
		}
	}
	memset(get_Com,0,sizeof(get_Com));
}

int main(void)
{

	wdt_init_t(5);
	sys_init();
	bat_monitoring();
	kar_on();
	while(1)
	{
		
		kar_connect();
		bat_monitoring();
		Handler_event();
		power_OFF_ON();
		led_run_task();
		moto_run_task();	
		exceotion_management();
		if(!rest_WDT_flag)		WDT_Feed();	
	}
}



