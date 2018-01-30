#include "config.h"

#if defined MM32F031K6
extern void SystemInit (void);
#endif

static _KAR_STATE	 	kar_state 	= KAR_STOP;
static _KAR_STATE	 	kar_state_t = MAX_KAR_STATE;
static _KEY_EVENT  		key_event 	= MAX_KEYS_EVENT;

static uint16_t key_timer = 0;
static uint8_t 	bat_value = 100;
static uint8_t  bat_last_value = 50;
static uint8_t  get_Com[10] = {0};
static uint16_t sleep_off_timer = SLEEP_DEFAULT_OFF_TIMER; //˯�߹ػ�ʱ��
static uint8_t FIRMWARE_VERSION[3]= {3,0,7};

extern _GetLedComData_t GetLedComData_t;
static void LowPowerConsumptionConfig(void);
static void dly1us(uint32_t dlytime) {while(dlytime--);}

uint8_t first_get_value = 1;

u16 ADC1_SingleChannel_Get(uint8_t ADC_Channel_x)
{ 
    u16 puiADData;
    /*ADCR�Ĵ�����ADSTλʹ�ܣ��������ת��*/
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==0);
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    puiADData=ADC1->ADDATA&0xfff;
    return puiADData;
}

void ADC1_SoftStart(void)
{
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==0);
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
}

static void get_adc_value(void)
{	
	static	 _ADC_typedef		adc_typedef;
	double  		 			adValue;
	uint32_t         			adValue_t;
	#if defined C32F0
		static  bool				bat_flag = true;
	#elif defined MM32F031K6
		static  bool				bat_flag = TRUE;
	#endif
	static uint16_t				moto_R_state_flag;				
	static uint16_t				moto_L_state_flag;

	#if defined C32F0
	if(check_soft_timeout(TIMER_BAT)) //״̬���м��ʱ��50us
	{	
		SYS_EnablePhrClk(AHB_ADC); 
		
		adValue_t = 0;
		adValue_t = (*ADC).DR[4].all;
		if(0x80000000 == (adValue_t & 0x80000000))
		{
			adValue = adValue_t*1000000*3.3/4095.0/1000;
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
			
//			if (first_get_value) {
//				first_get_value = 0;
//				bat_last_value = bat_value;
//			}
//			else {
//				if (GPIO_GetPinState(GPIOA,USB_DET)) {                                 //�������µ���ֵ����
//					if (bat_last_value > bat_value) {
//						bat_value = bat_last_value;
//					} else if ((bat_value - bat_last_value) > BAT_MIN_STEP) {
//							bat_value = bat_last_value;
//					} else {
//							bat_last_value = bat_value;
//					}
//				} else {
//						if (bat_last_value < bat_value) {
//							bat_value = bat_last_value;
//						} else if ((bat_last_value - bat_value) > BAT_MIN_STEP) {
//								bat_value = bat_last_value;
//						} else {
//								bat_last_value = bat_value;
//						}
//				}
//			}
		}
		
		set_soft_timer(TIMER_BAT,ENERGY_SAMPLING_TIMER); 
		//MOTO �ҵ��������� 1s����1200ma����Ϊ��ת״̬
		adValue_t = 0;
		adValue_t = (*ADC).DR[1].all;
		if(0x80000000 == (adValue_t & 0x80000000))
		{
			adValue = adValue_t*1000000*3.3/4095.0/1000;
			
			moto_R_state_flag = (uint16_t)adValue;
		
			#if defined( DeBug )
				//LOG(LOG_DEBUG," moto_R_state_flag = %d -> %d\r\n", (uint32_t)adValue,moto_R_state_flag);
			#endif
		}
		adValue_t = 0;
		adValue_t = (*ADC).DR[6].all;
		if(0x80000000 == (adValue_t & 0x80000000))
		{
			//MOTO ����������� 1s����1200ma����Ϊ��ת״̬
			
			adValue = adValue_t*1000000*3.3/4095.0/1000;
			moto_L_state_flag = (uint16_t)adValue;
			
			#if defined( DeBug )
				//LOG(LOG_DEBUG," moto_L_state_flag = %d -> %d\r\n", (uint32_t)adValue,moto_L_state_flag);
			#endif
		}
		//����������
		ADC_IssueSoftTrigger;
		get_moto_current_state(moto_R_state_flag,moto_L_state_flag,bat_value);
		
	}
	#elif defined MM32F031K6
	
	if(check_soft_timeout(TIMER_BAT)) //״̬���м��ʱ��50us
	{	 

		ADC1_SoftStart();
		adValue_t = 0;
		adValue_t = ADC1->ADDR4 & 0xfff;
		adValue = adValue_t*1000000*3.3/4095.0/1000;
		adValue = 1730 > adValue ? 0 : ((adValue-1730)/370*100);

		adc_typedef.bat_buffer[adc_typedef.head] = (uint8_t)adValue;

		adc_typedef.data = adc_typedef.data + adc_typedef.bat_buffer[adc_typedef.head];
		adc_typedef.head = (adc_typedef.head+1) == BAT_VALUE_BUFFER ? 0: adc_typedef.head+1;
		adc_typedef.data = adc_typedef.data - adc_typedef.bat_buffer[adc_typedef.head];

		if(bat_flag) {
			bat_value = (adc_typedef.data-adc_typedef.bat_buffer[adc_typedef.head])/adc_typedef.head;
			bat_flag  = (adc_typedef.head+1) == BAT_VALUE_BUFFER ? FALSE : TRUE;
		}else bat_value = (adc_typedef.data-adc_typedef.bat_buffer[adc_typedef.head])/BAT_VALUE_BUFFER;

		if (bat_value >100)
			bat_value = 100;

		if (first_get_value) {
			first_get_value = 0;
			bat_last_value = bat_value;
		}
		else {
			#if defined C32F0
			if (GPIO_GetPinState(GPIOA,USB_DET)) {                                 //�������µ���ֵ����
			#elif defined MM32F031K6
			if ( GPIO_ReadInputDataBit(GPIOA,USB_DET)){
			#endif
			
				if (bat_last_value > bat_value) {
					bat_value = bat_last_value;
				} else if ((bat_value - bat_last_value) > BAT_MIN_STEP) {
					bat_value = bat_last_value;
				} else {
					bat_last_value = bat_value;
				}
			} else {
				if (bat_last_value < bat_value) {
					bat_value = bat_last_value;
				} else if ((bat_last_value - bat_value) > BAT_MIN_STEP) {
					bat_value = bat_last_value;
				} else {
					bat_last_value = bat_value;
				}
			}
		}

		set_soft_timer(TIMER_BAT,ENERGY_SAMPLING_TIMER); 
		//MOTO �ҵ��������� 1s����1200ma����Ϊ��ת״̬
		adValue_t = 0;
		adValue_t = ADC1->ADDR1 & 0xfff;
		{
			adValue = adValue_t*1000000*3.3/4095.0/1000;

			moto_R_state_flag = (uint16_t)adValue;

#if defined( DeBug )
			//LOG(LOG_DEBUG," moto_R_state_flag = %d -> %d\r\n", (uint32_t)adValue,moto_R_state_flag);
#endif
		}
		adValue_t = 0;
		adValue_t = ADC1->ADDR6 & 0xfff;
		{
			//MOTO ����������� 1s����1200ma����Ϊ��ת״̬

			adValue = adValue_t*1000000*3.3/4095.0/1000;
			moto_L_state_flag = (uint16_t)adValue;

#if defined( DeBug )
			//LOG(LOG_DEBUG," moto_L_state_flag = %d -> %d\r\n", (uint32_t)adValue,moto_L_state_flag);
#endif
		}
		//����������
		get_moto_current_state(moto_R_state_flag,moto_L_state_flag,bat_value);

	}
	
	#endif
}
void configpad(uint32_t pinstat)
{
	#ifdef C32F0
	//IOCON->PIOA_0.all  = pinstat;  //POWERR_KEY
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
	//IOCON->PIOA_12.all = pinstat;
	IOCON->PIOA_13.all = pinstat;
	IOCON->PIOA_14.all = pinstat;
	
	if(GPIO_GetPinState(GPIOA,PIN15))
		IOCON->PIOA_15.all = 1; //MCU_INT
	else 
		IOCON->PIOA_15.all = 0; //MCU_INT
	
	IOCON->PIOB_0.all  = pinstat;
	IOCON->PIOB_1.all  = pinstat;
	IOCON->PIOB_2.all  = pinstat;
	IOCON->PIOB_3.all  = pinstat;
	IOCON->PIOB_4.all  = pinstat;
	IOCON->PIOB_5.all  = pinstat;
	IOCON->PIOB_6.all  = 1;
	IOCON->PIOB_7.all  = 1;

	IOCON->PIOC_0.all  = pinstat;
	IOCON->PIOC_1.all  = pinstat;
	IOCON->PIOC_2.all  = pinstat;
	IOCON->PIOC_3.all  = pinstat;
	
	#endif
}

#if defined MM32F031K6
void Sys_Stop(void)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1,DISABLE);	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
  //  PWR_WakeUpPinCmd(ENABLE);  //ʹ�ܻ��ѹܽŹ���
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI); 

    


    PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);	  //����ͣ����STOP��ģʽ 
    
}
#endif

void LowPowerConsumptionConfig(void)
{
	#if defined C32F0
	GPIO_InitTypeDef GPIO_InitStructure;
	
	#ifdef USING_RESET
//	GPIO_InitStructure.bit.FUNC = PC2_FUNC_NRST;
		GPIO_InitStructure.bit.FUNC = PC2_FUNC_GPIO;
	#endif
	
	WDT_Disable;
	aperture_all_off();
	moto_P();
//	configpad(0);
	DisablePhrClk_t();
	SYS_SetDeepSleepWakeupPin(PIN0|PIN5,FALL_EDGE);//���û�������	
	#if defined( DeBug )
		LOG(LOG_DEBUG," get sleep mode \r\n");
	#endif
	
	#if defined( V50_DeBug )
		LOG(LOG_DEBUG," get sleep mode \r\n");
	#endif
	SYS_DisablePhrClk(0xfffffff0 & (~(1<<29)));//�ر�GPIOAʱ��
	IOCON->PIOA_0.all  = PIN0|PIN5;//|PIN5;//���û�����������
	dly1us(50000);
	SYS_EnterDeepSleep(PD_RTCOSC | PD_BOD, 0);	
	sys_init_t();//���³�ʼ����������
	
	#ifdef USING_RESET
		GPIO_InitStructure.bit.FUNC = PC2_FUNC_NRST;
	//GPIO_InitStructure.bit.FUNC = PC2_FUNC_GPIO;
	#endif
	
	Information_events = get_Alarm_Int_state() ? RTC_INT_EVENTS : POWER_KEY_EVENTS;
	# if defined(DeBug)
		LOG(LOG_DEBUG," exit sleep mode...  ->%d \r\n",Information_events);
	#endif
	
	# if defined(V50_DeBug)
		LOG(LOG_DEBUG," exit sleep mode...  ->%d \r\n",Information_events);
	#endif
	
	#elif defined MM32F031K6
	

    RCC_LSICmd(DISABLE);
	Sys_Stop();    //����ͣ��ģʽ
		 
	SystemInit();  //���Ѻ����³�ʼ��һ��ʱ��
	Write_Iwdg_ON(IWDG_Prescaler_32,0x7ff);
	UART2_Init();
	
	
	#endif
}


/**
  *****************************************************************************
  * @Name   : C6��ػ���ʼ��״̬
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
	dly1us(50000);
	
	#if defined( DeBug )
		LOG(LOG_DEBUG,"kar_state&kar_state_t =  KAR_STOP");
	#endif
	
	#if defined( V50_DeBug )
		LOG(LOG_DEBUG,"kar_state&kar_state_t =  KAR_STOP");
	#endif
	dly1us(50000);
	DRV_Disable;//USB��ֹ���
	
	#if defined( DeBug )
		LOG(LOG_DEBUG,"power OFF...... \r\n");
	#endif
	
	#if defined( V50_DeBug )
		LOG(LOG_DEBUG,"power OFF...... \r\n");
	#endif
	dly1us(50000);
	
	POWER_OFF;
	aperture_all_off();
	moto_P();
	LowPowerConsumptionConfig();//����˯��
}
/**
  *****************************************************************************
  * @Name   : C6�忪����ʼ��״̬
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
	
	#if defined C32F0
		if(bat_value >= (BAT_VALUE_LOW+5)|| GPIO_GetPinState(GPIOA,USB_DET))//��ص�������10%����
	#elif defined MM32F031K6
		if(bat_value >= (BAT_VALUE_LOW+5)|| GPIO_ReadInputDataBit(GPIOA,USB_DET))//��ص�������10%����
	#endif
	{
		POWER_ON; //C6���Դ����
		led_mode_get_t(0x04,0xff,20 ); //������Ч
		#if defined( DeBug )
			LOG(LOG_DEBUG,"state start power ON...... \r\n");
			
		#endif
		
		#if defined( V50_DeBug )
			LOG(LOG_DEBUG,"state start power ON...... \r\n");
			
		#endif
		kar_state_t = KAR_RUN;  //��ʼ���մ�������
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
		
		kar_off();//����˯��
	}
	
	
	
	
	
	
}

/**
  *****************************************************************************
  * @Name   : ������Ϣ
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
		#if defined C32F0
			if(!GPIO_GetPinState(GPIOA,PIN0))
		#elif defined MM32F031K6
			if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
		#endif
		{
			key_timer++;
			if(key_timer == 30) 	
			{
				key_event = LONG_PRESS; //����
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
				key_event = RESET_PRESS; //��λ
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
			key_event = SHORT_PRESS;	//�̰�
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
  * @Name   : ���ػ��¼�
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
		
		case LONG_PRESS:		//�����¼�
			switch(kar_state) {
				case KAR_STOP:
					kar_state = KAR_STARTING;
					kar_on();	
					key_event = MAX_KEYS_EVENT;
				break;
				case KAR_RUN:
						led_mode_get_t(0x01,0xff,15 );		//�ػ���Ч
						WriteUartBuf(KAR_POWER__OFF);
						UART_Send_t(KAR_POWER_OFF_COMMAND); //���͹ػ�ָ��
						kar_state = KAR_STOPING;
				
						#if defined C32F0
							moto_P();
						#endif
				
						#if defined( DeBug )
							LOG(LOG_DEBUG,"send kar power off...... \r\n");
						#endif
				
						#if defined( V50_DeBug )
							LOG(LOG_DEBUG,"send kar power off...... \r\n");
						#endif
						key_event = MAX_KEYS_EVENT;
				break;
				case KAR_DORMANCY:                   //���������¼�  
					key_event = SHORT_PRESS;        //��ת���̰��¼�
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
		case SHORT_PRESS:				//�̰��¼�  1.�����¼�
		
			if(kar_state == KAR_DORMANCY)
			{	
				kar_state_t = KAR_RUN; //�˳�KAR˯�߿�ʼ���մ�������
				KAR_DORMANCY_Enable;
				set_soft_timer(TIMER_POWER, 500);	
				key_event = KEYS_DORMANCY_STATE;  //��ת��ִ�л����¼�
				
				#if defined( DeBug )
						LOG(LOG_DEBUG,"wakes up start ...... \r\n");
				#endif
				
				#if defined( V50_DeBug )
						LOG(LOG_DEBUG,"wakes up start ...... \r\n");
				#endif
			}
			break;
			
		case KEYS_DORMANCY_STATE:		//ִ�л����¼�
		
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
				#if defined C32F0
					SYS_ResetSystem();
				#endif
				//kar_off();
			break;

		default:break;
	}
}


/**
  *****************************************************************************
  * @Name   : �����ж��¼�����
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

	//DRV�¼�����
	if(Information_events&DRV_EVENTS)
	{
		#if defined( DeBug )
				LOG(LOG_DEBUG,"DRV_EVENTS \r\n");
		#endif
		if(all_event_flag.DRV)
		{
			DRV_Disable;//USB��ֹ���
			
			#if defined C32F0
				all_event_flag.DRV = false;
			#elif defined MM32F031K6
				all_event_flag.DRV = FALSE;
			#endif
			WriteUartBuf(0x00);  
			UART_Send_t(USB_OUT_COMMAN);
			
		}
		else
		{
			DRV_Enable;//USB���
			
			#if defined C32F0
				all_event_flag.DRV = true;
			#elif defined MM32F031K6
				all_event_flag.DRV = TRUE;
			#endif
			WriteUartBuf(0x01);
			UART_Send_t(USB_OUT_COMMAN);
				
		}
		
		Information_events	 &= 	(~DRV_EVENTS);
	}
	//ʱ���ж��¼�����
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
	if(Information_events&USB_DET_EVENTS)  //USB_DET�¼�
	{
		#if defined( DeBug )
			LOG(LOG_DEBUG,"USB_DET_EVENTS \r\n");
		#endif
		
		#if defined( V50_DeBug )
			LOG(LOG_DEBUG,"USB_DET_EVENTS \r\n");
		#endif
		
		#if defined C32F0
			if(kar_state == KAR_RUN && GPIO_GetPinState(GPIOA,USB_DET))
		#elif defined MM32F031K6
			if(kar_state == KAR_RUN && GPIO_ReadInputDataBit(GPIOA,USB_DET))
		#endif
		{
			WriteUartBuf(0x02);
			WriteUartBuf(bat_value);
			UART_Send_t(BAT_COMMAN);
		}
		Information_events &= (~USB_DET_EVENTS);
	
	}
	//��������
	if(Information_events&POWER_KEY_EVENTS)
	{
		power_key_event();
		//heartbeat_flag = 0;
	}
	
}

/**
  *****************************************************************************
  * @Name   : ��ȡ����ģʽ
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
		case KAR_RUN:  //����״̬
			kar_state = KAR_RUN;
			kar_state_t = KAR_RUN;
			#if defined( DeBug )
				LOG(LOG_DEBUG,"kar_state&kar_state_t =  KAR_RUN");
			#endif
		break;
		case KAR_STOP: //�ػ�״̬
			kar_off();
		break;
		case KAR_DORMANCY://˯��״̬
			kar_state = KAR_DORMANCY;	
			kar_state_t = KAR_DORMANCY;
			aperture_all_off(); //�رյƹ�
			//led_mode_get_t(LED_MODE_APERTURE_ALL_BREATHE,0xff,50);
			
			#if defined( DeBug )
				LOG(LOG_DEBUG,"kar_state&kar_state_t =  KAR_DORMANCY");
			#endif
			
		break;
		case KAR_RESET:  //kar˯��״̬
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
  * @Name   : BAT״̬����
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
		if(kar_state == KAR_RUN) //10���ϱ����� ״̬
		{
			#if defined( DeBug )
				LOG(LOG_DEBUG,"report_energy  10s 1 time bat_value = %d\r\n",bat_value);
			#endif
			if(bat_value < (BAT_VALUE_LOW+10)) //KAR f�����µ�������15%��ʼ����
			{
				if(!bat_alarm_timer)
				{
					WriteUartBuf(0x01);
					WriteUartBuf(bat_value);
					UART_Send_t(BAT_COMMAN);  
					bat_alarm_timer = 6; //1�����ϱ����������͵�
					#if defined( DeBug )
						LOG(LOG_DEBUG,"alarm energy LOW 10 bat_value = %d\r\n",bat_value);
					#endif
				}else bat_alarm_timer--;
			}else {
				bat_alarm_timer = 6;
				bat_alarm_timer_t = 3;
			}
			
			#if defined C32F0
				if(bat_value < BAT_VALUE_LOW && !GPIO_GetPinState(GPIOA,USB_DET) )  //KAR������ ����5%ִ�������ػ�
			#elif defined MM32F031K6
				if(bat_value < BAT_VALUE_LOW && !GPIO_ReadInputDataBit(GPIOA,USB_DET) )  //KAR������ ����5%ִ�������ػ�
			#endif
			{ 
			
				if(!bat_alarm_timer_t)
				{
					key_event = LONG_PRESS;			 //���������ػ��ź�
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
			
			get_sleep_timer_t = 6; //60��δ��������˯��
			get_sleep_timer = (6*sleep_off_timer);  //Ĭ��60���Ӻ�ػ�
		}
		else if(kar_state_t == KAR_DORMANCY ) //KAR_˯��ʱ������  60�� ��ػ�
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
				get_sleep_timer = (6*sleep_off_timer);  //Ĭ��60���Ӻ�ػ�
				kar_off();
			}else get_sleep_timer--;
			
			if(bat_value < BAT_VALUE_LOW) //KAR�����µ�����5% ִ��ǿ�ƹػ�
			{
				#if defined C32F0
					if(!GPIO_GetPinState(GPIOA,USB_DET))
				#elif defined MM32F031K6
					if(!GPIO_ReadInputDataBit(GPIOA,USB_DET))
				#endif
					{
						//��ȡ�Ƿ��ڳ��״̬
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
				get_sleep_timer_t = 6; //60��δ��������˯��
				kar_off();
			}else get_sleep_timer_t--;
		}
	}	
}



static void kar_connect(void)
{
	uint8_t i;
	uart0_get_cmd(get_Com);
	if(kar_state_t == KAR_RUN  || get_Com[0] ==  KAR_RUN_STATE)
	{
		#if defined C32F0
		switch(get_Com[0]) {

			case HANDSHAKE_COMMAND:
				WriteUartBuf(POISON_VERSION);
				for (i=0;i<3;i++)
					{
						WriteUartBuf(FIRMWARE_VERSION[i]);
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
			case SLEEP_OFF_TIMER_SEY_COMMAN:  //kar˯�߹ػ�ʱ������
				#if defined( DeBug )
					LOG(LOG_DEBUG,"Set Sleep off timer ......");
				#endif
				sleep_off_timer  = get_Com[1];
				WriteUartBuf(0x01);
				UART_Send_t(SLEEP_OFF_TIMER_SEY_COMMAN);
			break;
			case SYSTEM_UPDATE_COMMAN:      //��������
				#if defined( DeBug )
					LOG(LOG_DEBUG,"receive SYSTEM_UPDATE_COMMAN\r\n");
				#endif
				WriteUartBuf(0x00);
				UART_Send_t(0xF0);
				NVIC_SystemReset();
				break;
			default:break;
		}
	#elif defined MM32F031K6
				switch(get_Com[0]) {

			case HANDSHAKE_COMMAND:
				WriteUartBuf(POISON_VERSION);
				for (i=0;i<3;i++)
					{
						WriteUartBuf(FIRMWARE_VERSION[i]);
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
			case SLEEP_OFF_TIMER_SEY_COMMAN:  //kar˯�߹ػ�ʱ������
				#if defined( DeBug )
					LOG(LOG_DEBUG,"Set Sleep off timer ......");
				#endif
				sleep_off_timer  = get_Com[1];
				WriteUartBuf(0x01);
				UART_Send_t(SLEEP_OFF_TIMER_SEY_COMMAN);
			break;
			case SYSTEM_UPDATE_COMMAN:      //��������
				#if defined( DeBug )
					LOG(LOG_DEBUG,"receive SYSTEM_UPDATE_COMMAN\r\n");
				#endif
				WriteUartBuf(0x00);
				UART_Send_t(0xF0);
				NVIC_SystemReset();
				break;
			default:break;
		}
		
		
	#endif
	}
	memset(get_Com,0,sizeof(get_Com));
}

//#define APPLICATION_ADDRESS 0x1800
//#define  VECTOR_SIZE (48*4)

#define ApplicationAddress 0x8001800
#define  VECTOR_SIZE (48*4)

void app_iap_init(void)
{
	#if defined C32F0
	
 	memcpy((void*)0x10000000, (void*)APPLICATION_ADDRESS, VECTOR_SIZE); 
	SYS_MemRemaptoSRAM; 
	#endif
	
	memcpy((void*)0x20000000, (void*)ApplicationAddress, VECTOR_SIZE);  //���ж�������ӳ�䵽RAM������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
//SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
	SYSCFG->CFGR |= 0x03;
}

void app_MAL_Erase()      
{
   uint32_t Address;
   uint8_t index,temp;
//   UART_Send_Buffer[0] = UART_Receive_Buffer[0] | 0xC0;
	 temp = 2;  //100 K
   Address = 0x08007800;         			// APP flash   0x08004000 -- 0x08020000  0x1C000  APP????
	 FLASH_Unlock();  
	 FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
   for(index=0;index<temp;index++ )   //???????APP????
   	{   	
     FLASH_ErasePage(Address);
	   Address = Address + 0x400;   
   	}
   FLASH_Lock();
//   UART2_Send_BUFF(UART_Send_Buffer,LEN);
}

#ifdef MM32F031K6
/********************************************************************************************************
**������Ϣ ��PVU_CheckStatus(void)                       
**�������� �����������Ź�Ԥ��Ƶλ״̬
**������� ����
**������� ����
********************************************************************************************************/
void PVU_CheckStatus(void)
{
    while(1)
    {
        /*���Ԥ��Ƶλ״̬,ΪRESET�ſɸı�Ԥ��Ƶֵ*/
        if(IWDG_GetFlagStatus(IWDG_FLAG_PVU)==RESET)                                                       
        {
            break;
        }
    }
}
/********************************************************************************************************
**������Ϣ ��RVU_CheckStatus(void)                  
**�������� �����������Ź����ر�־
**������� ����
**������� ����
********************************************************************************************************/
void RVU_CheckStatus(void)
{
    while(1)
    {
        /*������ر�־״̬*/
        if(IWDG_GetFlagStatus(IWDG_FLAG_RVU)==RESET)  
        {
            break;
        }
    }
}



/********************************************************************************************************
**������Ϣ ��Write_Iwdg_PR(void)             
**�������� �������������Ź�
**������� ��IWDG_Prescaler ��ѡIWDG_Prescaler_X, XΪ4,8,16,32,64,128,256,��Ӧ��Ƶֵ��Xȡֵ��ͬ
Reload<=0xfff,Ϊ����������ֵ
**������� ����
**    ��ע ����λʱ��������LSI 40KHzΪ�ο�
Tiwdg=(X/LSI)*Reload
********************************************************************************************************/
void Write_Iwdg_ON(unsigned short int IWDG_Prescaler,unsigned short int Reload)
{
    /*�����ڲ�����ʱ��,�ȴ�ʱ�Ӿ���*/
    RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);
    
    /*����ʱ��Ԥ��Ƶ*/	
    PVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetPrescaler(IWDG_Prescaler);
    
    /*�������ؼĴ���ֵ*/	
    RVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetReload(Reload&0xfff);	
    
    /*װ�ز�ʹ�ܼ�����*/	
    IWDG_ReloadCounter();
    IWDG_Enable();
}

/********************************************************************************************************
**������Ϣ ��void Write_Iwdg_RL(void)           
**�������� ��ι������
**������� ��
**������� ����
********************************************************************************************************/
void Write_Iwdg_RL(void)
{
    IWDG_ReloadCounter();
}


#endif

int main(void)
{
	uint8_t i =0x1f;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	app_iap_init();
	
	sys_init();
	
	#if defined C32F0
	while(Rtc_Check())
	{
		i--;
		dly1us(100000);
		led_mode_get_tt(LED_MODE_APERTURE_ALL_BLINK,0xff,10);
		if(!i){
//			kar_off();//����˯��
			i = 0xff;
		}
	}//���RTC
	
	#if defined( DeBug )
		LOG(LOG_DEBUG,"RTC Check Successful.. \r\n");
	#endif
	
	if ((*((uint32_t *)(0x7800))) != 0x55aaaa55)
	{
		if ((1==SYSCON->SYSRESSTAT.bit.SYSRST)||(1==SYSCON->SYSRESSTAT.bit.WDTRST))
			{
				GetLedComData_t.com = LED_MODE_APERTURE_ALL_BLINK;
				kar_state_t =  KAR_RUN;
				kar_state   =  KAR_RUN;
				POWER_ON;
				SYS_ClearResetStatus();
			}
		else
			kar_off();//����˯��
	}
	else
	{
		kar_state_t =  KAR_RUN;
		kar_state   =  KAR_RUN;
		POWER_ON;
		GetLedComData_t.com = LED_MODE_APERTURE_ALL_ON;
		IAP_FlashProgram(0x7800,0);
	}
	
	#elif defined MM32F031K6
	
	if ((*((uint32_t *)(0x8007800))) != 0x55aaaa55)
	{
		if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) == SET||RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)
		{
			kar_state_t =  KAR_RUN;
			kar_state   =  KAR_RUN;
			POWER_ON;
			GetLedComData_t.com = LED_MODE_APERTURE_ALL_ON;
			RCC_ClearFlag();
		}
		else
		{
			kar_off();//����˯��
		}
	}
	else
	{
		kar_state_t =  KAR_RUN;
		kar_state   =  KAR_RUN;
		POWER_ON;
		GetLedComData_t.com = LED_MODE_APERTURE_ALL_ON;
		
		app_MAL_Erase();
		FLASH_Unlock();
		FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
		FLASH_ProgramWord(0x8007800,0);
		FLASH_Lock();   //
	}
	
	Rtc_Check();

	#endif
	
	
	while(1)
	{
		if(1)
		{
			get_adc_value();
			kar_connect();
			Handler_event();
			power_OFF_ON();
			led_run_task();
			moto_run_task();
		//exceotion_management();
			state_run_monitoring();
		}
		//get_adc_value();
		#if defined C32F0
			WDT_Feed();
		#elif defined MM32F031K6
			Write_Iwdg_RL();
		#endif
		dly1us(100000);
	}
}



