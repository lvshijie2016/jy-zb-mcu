#include "config.h"

 
 



static _KAR_STATE	 	kar_state 	= KAR_STOP;
static _KAR_STATE	 	kar_state_t = MAX_KAR_STATE;
static _KEY_EVENT  		key_event 	= MAX_KEYS_EVENT;
static uint16_t key_timer = 0;
static uint8_t 	bat_value = 100;
static uint8_t  get_Com[10] = {0};


void LowPowerConsumptionConfig(void);




void configpad(uint32_t pinstat)
{
//	IOCON->PIOA_0.all  = pinstat;  //POWERR_KEY
	IOCON->PIOA_1.all  = pinstat;
	IOCON->PIOA_2.all  = pinstat;
	IOCON->PIOA_3.all  = pinstat;
	IOCON->PIOA_4.all  = pinstat;
	//IOCON->PIOA_5.all  = pinstat; //RTC_INT
	IOCON->PIOA_6.all  = pinstat;
	IOCON->PIOA_7.all  = pinstat;
	IOCON->PIOA_8.all  = pinstat;
	IOCON->PIOA_9.all  = pinstat;
	IOCON->PIOA_10.all = pinstat;
	IOCON->PIOA_11.all = pinstat;
	IOCON->PIOA_12.all = pinstat;
	IOCON->PIOA_13.all = pinstat;
	IOCON->PIOA_14.all = pinstat;
	
	//IOCON->PIOA_15.all = pinstat; //MCU_INT
	
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

void LowPowerConsumptionConfig(void)
{
	WDT_Disable;
	aperture_all_off();
	moto_P();
	DisablePhrClk_t();
	configpad(0);
	SYS_SetDeepSleepWakeupPin(PIN0|PIN5,FALL_EDGE);//设置唤醒引脚	
	SYS_DisablePhrClk(0xfffffff0 & (~(1<<29)));//关闭GPIOA时钟
	
	IOCON->PIOA_0.all  = PIN0|PIN5;//设置唤醒引脚上拉
	SYS_EnterDeepSleep(PD_RTCOSC | PD_BOD, 0);	
	sys_init_t();//重新初始化所有配置
	
	Information_events = get_Alarm_Int_state() ? RTC_INT_EVENTS : POWER_KEY_EVENTS;
	# if defined(DeBug)
		LOG(LOG_DEBUG," exit sleep mode\r\n");
	#endif
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
	kar_state_t =  KAR_STOP;
	kar_state   =  KAR_STOP;
	LOG(LOG_DEBUG,"power OFF...... \r\n");
	POWER_OFF;
	#if defined( DeBug )
		LOG(LOG_DEBUG," Shut down all of the peripherals \r\n");
	#endif
	LowPowerConsumptionConfig();//进入睡眠
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
	//sys_init();
	if(bat_value > (BAT_VALUE_LOW+5)|| GPIO_GetPinState(GPIOA,USB_DET))//电池电量大于10%开机
	{
		POWER_ON; //C6板电源开启
		led_mode_get_t(0x04,0xff,20 ); //开机灯效
		#if defined( DeBug )
			LOG(LOG_DEBUG,"power ON...... \r\n");
		#endif
		kar_state_t = KAR_RUN;  //开始接收串口数据
	}else 
	{
		led_mode_get_t(0x01,0x03,8 );	
		#if defined( DeBug )
			LOG(LOG_DEBUG,"Low energy...... \r\n");
		#endif
		kar_off();//进入睡眠
	}
	
	
	
	
	
	
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
					LOG(LOG_DEBUG,"LONG_PRESS--key_timer=%d \r\n",key_timer);
				#endif
				
			}
			else if(key_timer > 2000) 
			{
				key_event = RESET_PRESS; //复位
				key_timer = 0;
				Information_events	 &= 	(~POWER_KEY_EVENTS);
				#if defined( DeBug )
					LOG(LOG_DEBUG,"START_RESET..... ");
				#endif
			}
			
			
			set_soft_timer(TIMER_KEY, 1);
		}else if(!key_timer || key_timer >= 100)
		{
			key_timer = 0;
			Information_events	 &= 	(~POWER_KEY_EVENTS);
			#if defined( DeBug )
				LOG(LOG_DEBUG," Exit_Key..\r\n");
			#endif
			
		}else if(key_timer <100)
		{
			key_event = SHORT_PRESS;	//短按
			key_timer = 0;
			Information_events	 &= 	(~POWER_KEY_EVENTS);
			#if defined( DeBug )
				LOG(LOG_DEBUG," SHORT_PRESS..\r\n");
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
					
					
					kar_on();	
					key_event = MAX_KEYS_EVENT;
			
				break;
				case KAR_RUN:
						led_mode_get_t(0x01,0xff,15 );		//关机灯效
						WriteUartBuf(KAR_POWER__OFF);
						UART_Send_t(KAR_POWER_OFF_COMMAND); //发送关机指令
						
						#if defined( DeBug )
							LOG(LOG_DEBUG,"power off...... \r\n");
						#endif
						key_event = MAX_KEYS_EVENT;
				break;
				case KAR_DORMANCY:                   //触发唤醒事件  
					key_event = SHORT_PRESS;        //跳转到短按事件
					#if defined( DeBug )
						LOG(LOG_DEBUG," Wake up ...... \r\n");
					#endif
					
				break;

				default:break;
			}
			
			break;
		case SHORT_PRESS:				//短按事件  1.唤醒事件
		
			if(kar_state == KAR_DORMANCY)
			{	
				kar_state_t = KAR_RUN; //退出KAR睡眠开始接收串口数据
				KAR_DORMANCY_Enable;
				set_soft_timer(TIMER_POWER, 500);	
				key_event = KEYS_DORMANCY_STATE;  //跳转到执行唤醒事件
				
				#if defined( DeBug )
						LOG(LOG_DEBUG,"wakes up start ...... \r\n");
				#endif
			}
			break;
			
		case KEYS_DORMANCY_STATE:		//执行唤醒事件
		
			if(check_soft_timeout(TIMER_POWER))
			{
				KAR_DORMANCY_Disable;
				key_event = MAX_KEYS_EVENT;
				#if defined( DeBug )
						LOG(LOG_DEBUG,"KAR wakes up system...... \r\n");
				#endif
			}
			
			
			break;
			
			case RESET_PRESS:		
				#if defined( DeBug )
						LOG(LOG_DEBUG,"MCU reset  ...... \r\n");
				#endif
				SYS_ResetSystem();
				//kar_off();
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
	//电池电量检测
	if(Information_events&ADC_BAT_EVENTS)
	{
		bat_value = get_adc_value();
	
		Information_events &= (~ADC_BAT_EVENTS);
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
		case KAR_RUN:  //开机状态
			kar_state = KAR_RUN;
			kar_state_t = KAR_RUN;
		break;
		case KAR_STOP: //关机状态
			kar_off();
		break;
		case KAR_DORMANCY://睡眠状态
			kar_state = KAR_DORMANCY;	
			kar_state_t = KAR_DORMANCY;
			//aperture_all_off();
			led_mode_get_t(LED_MODE_APERTURE_ALL_BREATHE,0xff,50);
		break;
		case KAR_RESET:  //kar睡眠状态
			EXCEPTION(EXCEPTION_3);
			//kar_state_t = KAR_RUN;
		break;
	}
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

static void state_run_monitoring(void)
{
	static bool 	report_energy_flag = true;
	static uint8_t	bat_alarm_timer = 0;
	static uint16_t run_timer_state = 0;
	static uint8_t  get_sleep_timer = 0;
	static uint8_t 	get_sleep_timer_t = 6;
	
	if(check_soft_timeout(TIMER_BAT)) //状态运行检测时间50us
	{
		SYS_EnablePhrClk(AHB_ADC); 
		NVIC_EnableIRQ(ADC_IRQn);
		ADC_IssueSoftTrigger; 
		
		if(!run_timer_state) //10秒一次
		{
			run_timer_state = (20000/ENERGY_SAMPLING_TIMER);
			bat_alarm_timer ? bat_alarm_timer-- : 0;
			get_sleep_timer ? get_sleep_timer-- : 0;
			
			if(kar_state_t != KAR_RUN && kar_state_t != KAR_DORMANCY)
				get_sleep_timer_t ? get_sleep_timer_t-- : 0;
			else  get_sleep_timer_t = 6;
			
			report_energy_flag = true;	
			
		}else run_timer_state--;
	
		if(bat_value < BAT_VALUE_LOW && kar_state == KAR_RUN)  //KAR运行下 低于5%执行正常关机
		{ 
			if(!GPIO_GetPinState(GPIOA,USB_DET)){//是否在充电状态
				key_event = LONG_PRESS;			 //发出按键关机信号
				#if defined( DeBug )
					LOG(LOG_DEBUG,"energy LOW 5 \r\n");
				#endif
			}
		}else if(bat_value < (BAT_VALUE_LOW) && kar_state_t == KAR_DORMANCY) //KAR休眠下电量低5% 执行强制关机
		{
			if(!GPIO_GetPinState(GPIOA,USB_DET)){//读取是否在充电状态
			
				kar_off();
			}
		}
		else if(bat_value < (BAT_VALUE_LOW+10) && !bat_alarm_timer  &&  kar_state == KAR_RUN) //KAR f运行下电量低于10%开始报警
		{
			WriteUartBuf(0x01);
			WriteUartBuf(bat_value);
			UART_Send_t(BAT_COMMAN);  
			bat_alarm_timer = 6; //1分钟上报警报电量低电
			#if defined( DeBug )
					LOG(LOG_DEBUG,"alarm energy LOW 10 \r\n");
			#endif
		}
		else if(report_energy_flag && kar_state == KAR_RUN) //10秒上报电量 状态
		{
			WriteUartBuf(0x00);
			WriteUartBuf(bat_value);
			UART_Send_t(BAT_COMMAN); 
			report_energy_flag = false;
		}
		else if(kar_state_t == KAR_DORMANCY && !get_sleep_timer ) //KAR_睡眠时间设置  30分 后关机
		{
			get_sleep_timer = (6*30);  //30分钟后关机
			kar_off();	
		}
		else if(kar_state_t != KAR_RUN && kar_state_t != KAR_DORMANCY && !get_sleep_timer_t)
		{
			get_sleep_timer_t = 6; //60秒未开机进入睡眠
			kar_off();
			
		}
	}

}



static void kar_connect(void)
{
	
	uart0_get_cmd(get_Com);
	if(kar_state_t == KAR_RUN )
	{
		switch(get_Com[0]) {

			case HANDSHAKE_COMMAND:
				WriteUartBuf(POISON_VERSION);
				WriteUartBuf(FIRMWARE_VERSION);
				UART_Send_t(HANDSHAKE_COMMAND);
				
				#if defined( DeBug )
					LOG(LOG_DEBUG,"FIRMWARE_VERSION= V %d . %d\r\n",FIRMWARE_VERSION/10,FIRMWARE_VERSION%10);
				#endif

			break;
			case KAR_RUN_STATE:
			
				get_kar_run_state(get_Com);
				
				
			break;
			case HEARTBEAT_COMMAND:

		
			break;
			case LIGHT_COMMAND:
				led_mode_get(get_Com);
				#if defined( DeBug )
					LOG(LOG_DEBUG,"LED_state = %d\r\n",get_Com[GET_LED_NUM]);
				#endif

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

//static void dly1us(int dlytime) {while(dlytime--);}

int main(void)
{
	sys_init();
	kar_off();//进入睡眠
	//moto_D();
	while(1)
	{
		//ADC_IssueSoftTrigger;
		//dly1us(500000);
		//LOG(LOG_DEBUG,"MTOT_ADC=%d\r\n",get_adc_moto());
		if(1)
		{
			kar_connect();
			Handler_event();
			power_OFF_ON();
			led_run_task();
		//	if(bat_value > 20) 
				moto_run_task();
		//	else moto_P();
			exceotion_management();
			state_run_monitoring();
		}
			WDT_Feed();	
	}
}



