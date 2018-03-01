#include "config.h"

#if defined MM32F031K6
extern void SystemInit (void);
#endif

_KAR_STATE	 	kar_state 	= KAR_STOP;
_KAR_STATE	 	kar_state_t = MAX_KAR_STATE;
static _KEY_EVENT  		key_event 	= MAX_KEYS_EVENT;

_KAR_STATE *r_kar_state = (_KAR_STATE*)0x20000ff8;
_KAR_STATE *r_kar_state_t = (_KAR_STATE*)0x20000ff4;

static uint16_t key_timer = 0;
static uint8_t 	bat_value = 100;
static uint8_t  bat_last_value = 50;
static uint8_t  get_Com[10] = {0};
static uint16_t sleep_off_timer = SLEEP_DEFAULT_OFF_TIMER; //睡眠关机时间
static uint8_t FIRMWARE_VERSION[3]= {3,0,11};

extern _GetLedComData_t GetLedComData_t;
static void dly1us(uint32_t dlytime) {while(dlytime--);}

uint8_t first_get_value = 1;

static void get_adc_value(void)
{
	uint8_t						vaild[3];			// 0: bat adc value, 1-2: motor adc value
	uint16_t					adc_value[3];
	double						adValue;
	static  bool				bat_flag = true;
	static	 _ADC_typedef		adc_typedef;
	static uint16_t				moto_R_state_flag;				
	static uint16_t				moto_L_state_flag;

	memset(vaild,0,sizeof(vaild));
	memset(adc_value,0,sizeof(adc_value));

	adc_value_get(vaild,adc_value);

	if(check_soft_timeout(TIMER_BAT)){ //状态运行检测时间50us
		if(vaild[0]){
			adValue = adc_value[0]*1000*3.3/4095.0;
			adValue = 1730 > adValue ? 0 : ((adValue-1730)/370*100);
			adc_typedef.bat_buffer[adc_typedef.head] = (uint8_t)adValue;
	
			adc_typedef.data = adc_typedef.data + adc_typedef.bat_buffer[adc_typedef.head];
			adc_typedef.head = (adc_typedef.head+1) == BAT_VALUE_BUFFER ? 0: adc_typedef.head+1;
			adc_typedef.data = adc_typedef.data - adc_typedef.bat_buffer[adc_typedef.head];
			
			if(bat_flag) {
				bat_value = (adc_typedef.data-adc_typedef.bat_buffer[adc_typedef.head])/adc_typedef.head;
				bat_flag  = (adc_typedef.head+1) == BAT_VALUE_BUFFER ? false : true;
			}else bat_value = (adc_typedef.data-adc_typedef.bat_buffer[adc_typedef.head])/BAT_VALUE_BUFFER;
			
				if (bat_value >100)
				bat_value = 100;
		}
		set_soft_timer(TIMER_BAT,ENERGY_SAMPLING_TIMER); 

		//MOTO 右电机电流检测 1s超过1200ma电流为堵转状态
		if(vaild[1])
		{
			adValue = adc_value[1]*1000*3.3/4095.0;
			
			moto_R_state_flag = (uint16_t)adValue;
		
		}

		//MOTO 左电机电流检测 1s超过1200ma电流为堵转状态
		if(vaild[2])
		{
			
			adValue =adc_value[2]*1000*3.3/4095.0;
			moto_L_state_flag = (uint16_t)adValue;
			
		}
		//传入电机驱动
		get_moto_current_state(moto_R_state_flag,moto_L_state_flag,bat_value);
		
	}
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
void kar_off(void)
{
	kar_state_t =  KAR_STOP;
	kar_state   =  KAR_STOP;
	*r_kar_state_t =  KAR_STOP;
	*r_kar_state   =  KAR_STOP;
	dly1us(50000);
	
	dly1us(50000);
	DRV_Disable;//USB截止输出
	
	dly1us(50000);
	
	POWER_OFF;					// 关闭C600电源
	aperture_all_off();			// 关闭灯光
	moto_P();					// 关闭电机
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
	uint8_t i;

	
	#if defined( DeBug )
		LOG(LOG_DEBUG,"Energy state (BAT = %d)\r\n",bat_value);	
	#endif
	
	if(bat_value >= (BAT_VALUE_LOW+5)|| USB_DET_VALUE)//电池电量大于10%开机
	{
		POWER_ON; //C6板电源开启
		led_mode_get_t(0x04,0xff,20 ); //开机灯效
		#if defined( DeBug )
			LOG(LOG_DEBUG,"state start power ON...... \r\n");
			
		#endif
		
		#if defined( V50_DeBug )
			LOG(LOG_DEBUG,"state start power ON...... \r\n");
			
		#endif
		kar_state_t = KAR_RUN;  //开始接收串口数据
		*r_kar_state_t = KAR_RUN;
	}else 
	{
		led_mode_get_t(0x01,0x03,8 );	
		#if defined( DeBug )
			LOG(LOG_DEBUG,"Low energy,state exit power ON ...... \r\n");
		#endif
		
		#if defined( V50_DeBug )
			LOG(LOG_DEBUG,"Low energy,state exit power ON ...... \r\n");
		#endif
		i = 0x1F;
		while(i--){
			
			dly1us(100000);
			led_mode_get_tt(LED_MODE_APERTURE_ALL_BLINK,0xff,10);
		}
		
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
		timer_delay_ms(1);
		if(!POWER_KEY_VALUE)
		{
			key_timer++;
			if(key_timer == 30) 	
			{
				key_event = LONG_PRESS; //长按
				key_timer = 0;
				Information_events	 &= 	(~POWER_KEY_EVENTS);
				#if defined( DeBug )
					LOG(LOG_DEBUG,"LONG_PRESS event ....\r\n");
				#endif
				
				#if defined( V50_DeBug )
					LOG(LOG_DEBUG,"LONG_PRESS event ....\r\n");
				#endif
				
			}
			#if 0
			else if(key_timer > 1200) 
			{
				key_event = RESET_PRESS; //复位
				key_timer = 0;
				Information_events	 &= 	(~POWER_KEY_EVENTS);
				#if defined( DeBug )
					LOG(LOG_DEBUG,"MCU START_RESET event ..... ");
				#endif
			}
			#endif
			
			
			set_soft_timer(TIMER_KEY, 1);
		}else if(!key_timer || key_timer >= 30)
		{
			key_timer = 0;
			Information_events	 &= 	(~POWER_KEY_EVENTS);
		}else if(key_timer >3&&key_timer <30)
		{
			key_event = SHORT_PRESS;	//短按
			key_timer = 0;
			Information_events	 &= 	(~POWER_KEY_EVENTS);
			#if defined( DeBug )
				LOG(LOG_DEBUG," SHORT_PRESS event ..\r\n");
			#endif
			
			#if defined( V50_DeBug )
				LOG(LOG_DEBUG," SHORT_PRESS event ..\r\n");
			#endif
		}
		else
		{
			key_timer = 0;
			Information_events	 &= 	(~POWER_KEY_EVENTS);
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
					kar_state = KAR_STARTING;
					kar_on();	
					key_event = MAX_KEYS_EVENT;
				break;
				case KAR_RUN:
						led_mode_get_t(0x01,0xff,15 );		//关机灯效
						WriteUartBuf(KAR_POWER__OFF);
						UART_Send_t(KAR_POWER_OFF_COMMAND); //发送关机指令
						kar_state = KAR_STOPING;
				
						moto_P();
				
						#if defined( DeBug )
							LOG(LOG_DEBUG,"send kar power off...... \r\n");
						#endif
				
						#if defined( V50_DeBug )
							LOG(LOG_DEBUG,"send kar power off...... \r\n");
						#endif
						key_event = MAX_KEYS_EVENT;
				break;
				case KAR_DORMANCY:                   //触发唤醒事件  
					key_event = SHORT_PRESS;        //跳转到短按事件
					#if defined( DeBug )
						LOG(LOG_DEBUG," state KAR_DORMANCY,Triggering SHORT_PRESS event ...... \r\n");
					#endif
					
					#if defined( V50_DeBug )
						LOG(LOG_DEBUG," state KAR_DORMANCY,Triggering SHORT_PRESS event ...... \r\n");
					#endif
				break;
				case KAR_STARTING:
				case KAR_STOPING:
					key_event = MAX_KEYS_EVENT;
					kar_off();
				break;

				default:break;
			}
			
			break;
		case SHORT_PRESS:				//短按事件  1.唤醒事件
		
			if(kar_state == KAR_DORMANCY)
			{	
				kar_state_t = KAR_RUN; //退出KAR睡眠开始接收串口数据
				*r_kar_state_t = KAR_RUN;
				KAR_DORMANCY_Enable;
				set_soft_timer(TIMER_POWER, 500);	
				key_event = KEYS_DORMANCY_STATE;  //跳转到执行唤醒事件
				
				#if defined( DeBug )
						LOG(LOG_DEBUG,"wakes up start ...... \r\n");
				#endif
				
				#if defined( V50_DeBug )
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
				
				#if defined( V50_DeBug )
						LOG(LOG_DEBUG,"KAR wakes up system...... \r\n");
				#endif
			}
			
			
			break;
			
			case RESET_PRESS:		
				#if defined( DeBug )
						LOG(LOG_DEBUG,"MCU reset  ...... \r\n");
				#endif
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
		#if defined( DeBug )
				LOG(LOG_DEBUG,"DRV_EVENTS \r\n");
		#endif
		if(all_event_flag.DRV)
		{
			DRV_Disable;//USB截止输出
			
			all_event_flag.DRV = false;
			WriteUartBuf(0x00);  
			UART_Send_t(USB_OUT_COMMAN);
			
		}
		else
		{
			DRV_Enable;//USB输出
			
			all_event_flag.DRV = true;
			WriteUartBuf(0x01);
			UART_Send_t(USB_OUT_COMMAN);
				
		}
		
		Information_events	 &= 	(~DRV_EVENTS);
	}
	//时钟中断事件处理
	if(Information_events&RTC_INT_EVENTS)
	{
		#if defined( DeBug )
			LOG(LOG_DEBUG,"RTC_INT_EVENTS\n");
		#endif
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
		
		
	}
	if(Information_events&USB_DET_EVENTS)  //USB_DET事件
	{
		#if defined( DeBug )
			LOG(LOG_DEBUG,"USB_DET_EVENTS \r\n");
		#endif
		
		#if defined( V50_DeBug )
			LOG(LOG_DEBUG,"USB_DET_EVENTS \r\n");
		#endif
		
		if(kar_state == KAR_RUN && USB_DET_VALUE)
		{
			WriteUartBuf(0x02);
			WriteUartBuf(bat_value);
			UART_Send_t(BAT_COMMAN);
		}
		Information_events &= (~USB_DET_EVENTS);
	
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
		case KAR_RUN:  //开机状态
			kar_state = KAR_RUN;
			kar_state_t = KAR_RUN;
			*r_kar_state = KAR_RUN;
			*r_kar_state_t = KAR_RUN;
			#if defined( DeBug )
				LOG(LOG_DEBUG,"kar_state&kar_state_t =  KAR_RUN");
			#endif
		break;
		case KAR_STOP: //关机状态
			kar_off();
		break;
		case KAR_DORMANCY://睡眠状态
			kar_state = KAR_DORMANCY;	
			kar_state_t = KAR_DORMANCY;
			*r_kar_state = KAR_DORMANCY;	
			*r_kar_state_t = KAR_DORMANCY;
			aperture_all_off(); //关闭灯光
			//led_mode_get_t(LED_MODE_APERTURE_ALL_BREATHE,0xff,50);
			
			#if defined( DeBug )
				LOG(LOG_DEBUG,"kar_state&kar_state_t =  KAR_DORMANCY");
			#endif
			
		break;
		case KAR_RESET:  //kar睡眠状态
			#if defined( DeBug )
				LOG(LOG_DEBUG,"KAR_RESET  EVENTS ......");
			#endif
			
			#if defined C32F0
			EXCEPTION(EXCEPTION_3);
			#endif
			//kar_state_t = KAR_RUN;
		break;
		
	}

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
	
	static  uint8_t		bat_alarm_timer = 0;
	static  uint8_t		bat_alarm_timer_t = 3;
	static  uint16_t  	get_sleep_timer = 6*SLEEP_DEFAULT_OFF_TIMER;
	static  uint8_t 	get_sleep_timer_t = 6;
	
	
	if(check_soft_timeout(TIMER_STATE_RUN))
	{
		set_soft_timer(TIMER_STATE_RUN,10000);
		if(kar_state == KAR_RUN) //10秒上报电量 状态
		{
			#if defined( DeBug )
				LOG(LOG_DEBUG,"report_energy  10s 1 time bat_value = %d\r\n",bat_value);
			#endif
			if(bat_value < (BAT_VALUE_LOW+10)) //KAR f运行下电量低于15%开始报警
			{
				if(!bat_alarm_timer)
				{
					WriteUartBuf(0x01);
					WriteUartBuf(bat_value);
					UART_Send_t(BAT_COMMAN);  
					bat_alarm_timer = 6; //1分钟上报警报电量低电
					#if defined( DeBug )
						LOG(LOG_DEBUG,"alarm energy LOW 10 bat_value = %d\r\n",bat_value);
					#endif
				}else bat_alarm_timer--;
			}else {
				bat_alarm_timer = 6;
				bat_alarm_timer_t = 3;
			}
			
			if(bat_value < BAT_VALUE_LOW && !USB_DET_VALUE )  //KAR运行下 低于5%执行正常关机
			{ 
			
				if(!bat_alarm_timer_t)
				{
					key_event = LONG_PRESS;			 //发出按键关机信号
				#if defined( DeBug )
					LOG(LOG_DEBUG,"energy LOW 5 get -> POWER_OFF bat_value = %d\r\n",bat_value);
				#endif
					
				#if defined( V50_DeBug )
					LOG(LOG_DEBUG,"energy LOW 5 get -> POWER_OFF bat_value = %d\r\n",bat_value);
				#endif
					bat_alarm_timer_t = 3;
				}else bat_alarm_timer_t--;
				
			}else{
				WriteUartBuf(0x00);
				WriteUartBuf(bat_value);
				UART_Send_t(BAT_COMMAN); 
			}
			
			get_sleep_timer_t = 6; //60秒未开机进入睡眠
			get_sleep_timer = (6*sleep_off_timer);  //默认60分钟后关机
		}
		else if(kar_state_t == KAR_DORMANCY ) //KAR_睡眠时间设置  60分 后关机
		{
			#if defined( DeBug )
				LOG(LOG_DEBUG,"get_sleep_timer = %d\r\n",(get_sleep_timer*10));
			#endif
			
			#if defined( V50_DeBug )
				LOG(LOG_DEBUG,"get_sleep_timer = %d\r\n",(get_sleep_timer*10));
			#endif
			
			if(!get_sleep_timer)
			{
				#if defined( DeBug )
					LOG(LOG_DEBUG,"kar_state_t = KAR_DORMANCY  MCU 30 minutes get kar_off()\r\n");
				#endif
				get_sleep_timer = (6*sleep_off_timer);  //默认60分钟后关机
				kar_off();
			}else get_sleep_timer--;
			
			if(bat_value < BAT_VALUE_LOW) //KAR休眠下电量低5% 执行强制关机
			{
					if(!USB_DET_VALUE)
					{
						//读取是否在充电状态
						#if defined( DeBug )
							LOG(LOG_DEBUG,"KAR_DORMANCY energy LOW 5 get kar_off() %d\r\n" , bat_value);
						#endif
						
						#if defined( V50_DeBug )
							LOG(LOG_DEBUG,"KAR_DORMANCY energy LOW 5 get kar_off() %d\r\n" , bat_value);
						#endif
						kar_off();
				}
			}
		}else if(kar_state_t != KAR_RUN){
			
			
			#if defined( DeBug )
				LOG(LOG_DEBUG,"get_sleep_timer_t = %d\r\n",(get_sleep_timer_t*10));
			#endif
			
			if(!get_sleep_timer_t){
				#if defined( DeBug )
					LOG(LOG_DEBUG,"kar_state_t = KAR_STOP  MCU 1 minutes get kar_off()\r\n");
				#endif
				get_sleep_timer_t = 6; //60秒未开机进入睡眠
				kar_off();
			}else get_sleep_timer_t--;
		}
	}	
}

static unsigned char *iap_version_addr = (unsigned char*)0x8007c00;

static void kar_connect(void)
{
	uint8_t i;
	uart0_get_cmd(get_Com);
	if(kar_state_t == KAR_RUN  || get_Com[0] ==  KAR_RUN_STATE)
	{
		switch(get_Com[0]) {

			case HANDSHAKE_COMMAND:
				WriteUartBuf(POISON_VERSION);
				for (i=0;i<3;i++)
				{
					WriteUartBuf(FIRMWARE_VERSION[i]);
				}
				for (i=0;i<3;i++)
				{
					WriteUartBuf(iap_version_addr[i]);
				}

				UART_Send_t(HANDSHAKE_COMMAND);

#if defined( DeBug )
				LOG(LOG_DEBUG,"FIRMWARE_VERSION= V %d . %d. %d\r\n",FIRMWARE_VERSION[0],FIRMWARE_VERSION[1],FIRMWARE_VERSION[2]);
#endif

#if defined( V50_DeBug )
				LOG(LOG_DEBUG,"FIRMWARE_VERSION= V %d . %d. %d\r\n",FIRMWARE_VERSION[0],FIRMWARE_VERSION[1],FIRMWARE_VERSION[2]);
#endif

				break;
			case KAR_RUN_STATE:
#if defined( DeBug )
				LOG(LOG_DEBUG,"KAR_RUN_STATE\r\n");
#endif
				get_kar_run_state(get_Com);


				break;
			case HEARTBEAT_COMMAND:

#if defined( DeBug )
				LOG(LOG_DEBUG,"HEARTBEAT_COMMAND\r\n");
#endif

				break;
			case LIGHT_COMMAND:
				led_mode_get(get_Com);
#if defined( DeBug )
				LOG(LOG_DEBUG,"LED_state = %d\r\n",get_Com[GET_LED_NUM]);
#endif

				break;
			case ALARM_COMMAND:
#if defined( DeBug )
				LOG(LOG_DEBUG,"Set_Alarm_Clock(get_Com) \r\n");
#endif
				Set_Alarm_Clock(get_Com);

				break;
			case MOTO_COMMAND:
#if defined( DeBug )
				LOG(LOG_DEBUG,"GetMotoCom(get_Com)\r\n");
#endif
				GetMotoCom(get_Com);
				break;
			case TIMER_DATA_COMMAND:
#if defined( DeBug )
				LOG(LOG_DEBUG,"Set_date_timer(get_Com)\r\n");
#endif
				Set_date_timer(get_Com);

				break;

			case GET_TIMER_COMMAND:
#if defined( DeBug )
				LOG(LOG_DEBUG,"Get_date_timer()\r\n");
#endif
				Get_date_timer();
				break;
			case BAT_COMMAN:
#if defined( DeBug )
				LOG(LOG_DEBUG,"BAT_COMMAN\r\n");
#endif
				WriteUartBuf(0x00);
				WriteUartBuf(bat_value);
				UART_Send_t(BAT_COMMAN);
				break;
			case SLEEP_OFF_TIMER_SEY_COMMAN:  //kar睡眠关机时间设置
#if defined( DeBug )
				LOG(LOG_DEBUG,"Set Sleep off timer ......");
#endif
				sleep_off_timer  = get_Com[1];
				WriteUartBuf(0x01);
				UART_Send_t(SLEEP_OFF_TIMER_SEY_COMMAN);
				break;
			case SYSTEM_UPDATE_COMMAN:      //升级命令
#if defined( DeBug )
				LOG(LOG_DEBUG,"receive SYSTEM_UPDATE_COMMAN\r\n");
#endif
				WriteUartBuf(0x00);
				UART_Send_t(0xF0);
				update_flash_flag_set();
				dly1us(1000000);
				NVIC_SystemReset();
				break;
			default:break;
		}

	}
	memset(get_Com,0,sizeof(get_Com));
}


int main(void)
{
	app_iap_init();
	
	uni_sys_init();	// 硬件初始化
	
	sys_start();	// 判断启动条件，区别软复位启动，升级后启动和掉电启动
	
	while(1)
	{
		get_adc_value();
		kar_connect();
		Handler_event();
		power_OFF_ON();
		led_run_task();
		moto_run_task();
		//exceotion_management();
		state_run_monitoring();
		uni_wdt_reload();

		dly1us(100000);
	}
}



