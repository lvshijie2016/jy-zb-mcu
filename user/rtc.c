#include "rtc.h"
uint8_t rtc_buf[4] = {0};


/**
  *****************************************************************************
  * @Name   : 将BIN转换为BCD
  *
  * @Brief  : none
  *
  * @Input  : BINValue: 输入BIN
  *
  * @Output : none
  *
  * @Return : BCD格式数值
  *****************************************************************************
**/
static unsigned char RTC_BinToBcd2(unsigned char BINValue)
{
	unsigned char bcdhigh = 0;
	
	while (BINValue >= 10)
	{
		bcdhigh++;
		BINValue -= 10;
	}
	
	return ((unsigned char)(bcdhigh << 4) | BINValue);
}

/**
  *****************************************************************************
  * @Name   : 将BCD转换为BIN
  *
  * @Brief  : none
  *
  * @Input  : BCDValue: 输入BCD
  *
  * @Output : none
  *
  * @Return : BIN格式数值
  *****************************************************************************
**/
static unsigned char RTC_Bcd2ToBin(unsigned char BCDValue)
{
	unsigned char tmp = 0;
	
	tmp = ((unsigned char)(BCDValue & (unsigned char)0xF0) >> (unsigned char)0x04) * 10;
	return (tmp + (BCDValue & (unsigned char)0x0F));
}


/**
  *****************************************************************************
  * @Name   : RTC某寄存器写入一个字节数据
  *
  * @Brief  : none
  *
  * @Input  : REG_ADD：要操作寄存器地址
  *           dat：    要写入的数据
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
static void RTC_Write_Byte(unsigned char REG_ADD, unsigned char dat)
{
	IIC_WriteByte(RTC_Write,REG_ADD,dat);
} 

/**
  *****************************************************************************
  * @Name   : RTC某寄存器读取一个字节数据
  *
  * @Brief  : none
  *
  * @Input  : REG_ADD：要操作寄存器地址
  *
  * @Output : none
  *
  * @Return : 读取得到的寄存器的值
  *****************************************************************************
**/
static unsigned char RTC_Read_Byte(unsigned char REG_ADD)
{
	return IIC_ReadByte(RTC_Write,REG_ADD);
}



/**
  *****************************************************************************
  * @Name   : RTC写入多组数据
  *
  * @Brief  : none
  *
  * @Input  : REG_ADD：要操作寄存器起始地址
  *           num：    写入数据数量
  *           *WBuff： 写入数据缓存
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
static void RTC_Write_nByte(unsigned char REG_ADD, unsigned char num, unsigned char *pBuff)
{
	IIC_WriteArray(RTC_Write,REG_ADD,pBuff,num);
}

/**
  *****************************************************************************
  * @Name   : RTC读取多组数据
  *
  * @Brief  : none
  *
  * @Input  : REG_ADD：要操作寄存器起始地址
  *           num：    读取数据数量
  *
  * @Output : *WBuff： 读取数据缓存
  *
  * @Return : none
  *****************************************************************************
**/
static void RTC_Read_nByte(unsigned char REG_ADD, unsigned char num, unsigned char *pBuff)
{
	IIC_ReadArray(RTC_Write,REG_ADD,num,pBuff);
}


/**
  *****************************************************************************
  * @Name   : RTC启动
  *
  * @Brief  : none
  *
  * @Input  : none
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
void RTC_Start(void)
{
	unsigned char temp = 0;
	
	temp = RTC_Read_Byte(RTC_Address_Control_Status_1);  //读取控制/状态寄存器1
	if (temp & PCF_Control_ChipStop)
	{
		temp &= PCF_Control_ChipRuns;  //运行芯片
	}
	if ((temp & (1<<7)) == 0)  //普通模式
	{
		temp &= PCF_Control_TestcClose;  //电源复位模式失效
	}
	RTC_Write_Byte(RTC_Address_Control_Status_1, temp);  //再写入数值
	RTC_Write_Byte(RTC_Address_Control_Status_2, 0x00);  
	RTC_Write_Byte(RTC_Address_CLKOUT,PCF_CLKOUT_Close);
	RTC_Write_Byte(RTC_Address_Timer,PCF_Timer_Close);
	#if defined( DeBug )
			LOG(LOG_DEBUG,"RTC_init.. \r\n");
	#endif

}


/**
  *****************************************************************************
  * @Name   : RTC写入时间日期信息
  *
  * @Brief  : 星期数值范围是: 0 ~ 6，十进制格式
  *
  * @Input  : 
  *           PCF_DataStruct: 日期结构指针
  *
  * @Output : none
  *
  * @Return : none	  unsigned char PCF_Century,
  *****************************************************************************
**/
void RTC_SetTimeDate(_RTC_Register_Typedef* PCF_DataStruct)
{
	

	if (PCF_DataStruct->Years > 99)    PCF_DataStruct->Years    = 0;  //恢复00年
	if (PCF_DataStruct->Months_Century > 12)   PCF_DataStruct->Months_Century   = 1;  //恢复1月
	if (PCF_DataStruct->Days > 31)     PCF_DataStruct->Days     = 1;  //恢复1日
	if (PCF_DataStruct->WeekDays > 6)  PCF_DataStruct->WeekDays = 1;  //恢复星期一
	if (PCF_DataStruct->Hours > 23)    PCF_DataStruct->Hours   = 0;  //恢复0小时
	if (PCF_DataStruct->Minutes > 59)  PCF_DataStruct->Minutes = 0;  //恢复0分钟
	if (PCF_DataStruct->Seconds > 59)  PCF_DataStruct->Seconds = 0;  //恢复0秒

	//
	PCF_DataStruct->Years    = RTC_BinToBcd2(PCF_DataStruct->Years);
	PCF_DataStruct->Months_Century   = RTC_BinToBcd2(PCF_DataStruct->Months_Century);
	PCF_DataStruct->Days     = RTC_BinToBcd2(PCF_DataStruct->Days);
	PCF_DataStruct->WeekDays = RTC_BinToBcd2(PCF_DataStruct->WeekDays);
	PCF_DataStruct->Hours   = RTC_BinToBcd2(PCF_DataStruct->Hours);
	PCF_DataStruct->Minutes = RTC_BinToBcd2(PCF_DataStruct->Minutes);
	PCF_DataStruct->Seconds = RTC_BinToBcd2(PCF_DataStruct->Seconds);

	PCF_DataStruct->Months_Century &= ~PCF_Century_SetBitC;
//
	//数据拷贝
	//
	rtc_buf[0] = PCF_DataStruct->Days;
	rtc_buf[1] = PCF_DataStruct->WeekDays;
	rtc_buf[2] = PCF_DataStruct->Months_Century;
	rtc_buf[3] = PCF_DataStruct->Years;
	//
	//写入数据到寄存器
	//
	RTC_Write_nByte(RTC_Address_Days, 4, rtc_buf);
	
	//拷贝数据
	//
	rtc_buf[0] = PCF_DataStruct->Seconds;
	rtc_buf[1] = PCF_DataStruct->Minutes;
	rtc_buf[2] = PCF_DataStruct->Hours;
	//
	//写入数据到寄存器
	//
	RTC_Write_nByte(RTC_Address_Seconds, 3, rtc_buf);
	
}




/**
  *****************************************************************************
  * @Name   : RTC读取时间日期信息
  *
  * @Brief  : 星期数值范围是: 0 ~ 6，十进制格式
  *
  * @Input  :
  *
  * @Output :
  *           PCF_DataStruct: 日期结构指针
  *
  * @Return : none		   unsigned char *PCF_Century,
  *****************************************************************************
**/
void RTC_GetTimeDate(_RTC_Register_Typedef* PCF_DataStruct)
{
	//读取全部寄存器数值
	RTC_Read_nByte(RTC_Address_Days, 4, rtc_buf);
	//屏蔽无效位
	rtc_buf[0] &= RTC_Shield_Days;
	rtc_buf[1] &= RTC_Shield_WeekDays;
	rtc_buf[2] &= RTC_Shield_Months_Century;
	rtc_buf[3] &= RTC_Shield_Years;
	PCF_DataStruct->Years    = RTC_Bcd2ToBin(rtc_buf[3]);
	PCF_DataStruct->Months_Century   = RTC_Bcd2ToBin(rtc_buf[2]);
	PCF_DataStruct->Days     = RTC_Bcd2ToBin(rtc_buf[0]);
	PCF_DataStruct->WeekDays = RTC_Bcd2ToBin(rtc_buf[1]);
	
	//读取寄存器数值
	RTC_Read_nByte(RTC_Address_Seconds, 3, rtc_buf);
	//
	//屏蔽无效位
	//
	rtc_buf[0] &= RTC_Shield_Seconds;
	rtc_buf[1] &= RTC_Shield_Minutes;
	rtc_buf[2] &= RTC_Shield_Hours;

	PCF_DataStruct->Hours   = RTC_Bcd2ToBin(rtc_buf[2]);
	PCF_DataStruct->Minutes = RTC_Bcd2ToBin(rtc_buf[1]);
	PCF_DataStruct->Seconds = RTC_Bcd2ToBin(rtc_buf[0]);

}



/**
  *****************************************************************************
  * @Name   : RTC写入闹铃信息
  *
  * @Brief  : 星期数值范围是: 0 ~ 6，十进制格式
  *
  * @Input  : AlarmType_EN:    
  *           PCF_DataStruct: 结构指针
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
void RTC_SetAlarm(unsigned char AlarmType_EN, _RTC_Register_Typedef* PCF_DataStruct)
{
	unsigned char Alarm_Interrupt = 0;  //控制/状态寄存器闹铃中断缓存
	
	//判断数值是否在范围之内
	//
	if (PCF_DataStruct->Minute_Alarm > 59)  PCF_DataStruct->Minute_Alarm  = 0;  //恢复0分钟
	if (PCF_DataStruct->Hour_Alarm > 23)    PCF_DataStruct->Hour_Alarm    = 0;  //恢复0小时
	if (PCF_DataStruct->Day_Alarm > 31)     PCF_DataStruct->Day_Alarm     = 1;  //恢复1日
	if (PCF_DataStruct->WeekDays_Alarm > 6)  PCF_DataStruct->WeekDays_Alarm = 1;  //恢复星期一
	//
	//需要转换一下
	//
	PCF_DataStruct->Minute_Alarm  = RTC_BinToBcd2(PCF_DataStruct->Minute_Alarm);
	PCF_DataStruct->Hour_Alarm    = RTC_BinToBcd2(PCF_DataStruct->Hour_Alarm);
	PCF_DataStruct->Day_Alarm     = RTC_BinToBcd2(PCF_DataStruct->Day_Alarm);
	PCF_DataStruct->WeekDays_Alarm = RTC_BinToBcd2(PCF_DataStruct->WeekDays_Alarm);


	//
	//读取控制/状态寄存器2值
	//
	Alarm_Interrupt = RTC_Read_Byte(RTC_Address_Control_Status_2);
	Alarm_Interrupt &= PCF_Alarm_INT_Close;  //先关闭中断输出
	Alarm_Interrupt &= PCF_Control_ClearAF;;  //清除标志
	RTC_Write_Byte(RTC_Address_Control_Status_2, Alarm_Interrupt);
	
	//
	PCF_DataStruct->Day_Alarm 		= (AlarmType_EN & AlarmType_Days_EN) 		? PCF_DataStruct->Day_Alarm&PCF_Alarm_DaysOpen 						: PCF_DataStruct->Day_Alarm			|PCF_Alarm_DaysClose;  //日期闹铃
	PCF_DataStruct->Hour_Alarm		=(AlarmType_EN & AlarmType_Hours_EN)		? PCF_DataStruct->Hour_Alarm	 &PCF_Alarm_HoursOpen 			: PCF_DataStruct->Hour_Alarm    |PCF_Alarm_HoursClose;  //小时闹铃
	PCF_DataStruct->Minute_Alarm	=(AlarmType_EN & AlarmType_Minutes_EN)	?  PCF_DataStruct->Minute_Alarm  &PCF_Alarm_MinutesOpen 	: PCF_DataStruct->Minute_Alarm  |PCF_Alarm_MinutesClose;  //分钟闹铃
	PCF_DataStruct->WeekDays_Alarm=(AlarmType_EN & AlarmType_WeekDays_EN) ? PCF_DataStruct->WeekDays_Alarm &PCF_Alarm_WeekDaysOpen 	: PCF_DataStruct->WeekDays_Alarm|PCF_Alarm_WeekDaysClose;  //分钟闹铃

	//开启中断输出
	//
		Alarm_Interrupt |= PCF_Alarm_INT_Open;
		Alarm_Interrupt &= PCF_Control_ClearAF;;  //清除标志
	//
	//拷贝数据
	//
	rtc_buf[0] = PCF_DataStruct->Minute_Alarm;
	rtc_buf[1] = PCF_DataStruct->Hour_Alarm;
	rtc_buf[2] = PCF_DataStruct->Day_Alarm;
	rtc_buf[3] = PCF_DataStruct->WeekDays_Alarm;
	//
	//写入数据到寄存器
	//
	RTC_Write_nByte(RTC_Alarm_Minutes, 4, rtc_buf);
	//
	//写入控制/状态寄存器2数值
	//
	RTC_Write_Byte(RTC_Address_Control_Status_2, Alarm_Interrupt);
}





/**
  *****************************************************************************
  * @Name   : RTC读取闹铃信息
  *
  * @Brief  : 星期数值范围是: 0 ~ 6，十进制格式；只是返回了闹铃寄存器数值，开关位、中断输出什么的不返回
  *
  * @Input  :
  *
  * @Output : PCF_DataStruct: 闹铃结构指针
  *
  * @Return : none
  *****************************************************************************
**/
void RTC_GetAlarm(_RTC_Register_Typedef* PCF_DataStruct)
{
	//读取
	RTC_Read_nByte(RTC_Alarm_Minutes, 4, rtc_buf);
	//屏蔽无效位，分钟报警值全部返回
	rtc_buf[0] &= RTC_Shield_Minute_Alarm;  //分钟报警值
	rtc_buf[1] &= RTC_Shield_Hour_Alarm;  //小时报警值
	rtc_buf[2] &= RTC_Shield_Day_Alarm;  //日期报警值
	rtc_buf[3] &= RTC_Shield_WeekDays_Alarm;  //星期报警值
	PCF_DataStruct->Day_Alarm     = RTC_Bcd2ToBin(rtc_buf[2]);
	PCF_DataStruct->Hour_Alarm    = RTC_Bcd2ToBin(rtc_buf[1]);
	PCF_DataStruct->Minute_Alarm  = RTC_Bcd2ToBin(rtc_buf[0]);
	PCF_DataStruct->WeekDays_Alarm = RTC_Bcd2ToBin(rtc_buf[3]);

}


void RTC_Clear_GetControlStatus_2(void)
{
	unsigned char Alarm_Interrupt = 0;  //控制/状态寄存器闹铃中断缓存
	Alarm_Interrupt = RTC_Read_Byte(RTC_Address_Control_Status_2);
	Alarm_Interrupt &= ~(3<<2);
	Alarm_Interrupt |= (1<<1); //开报警中断
	RTC_Write_Byte(RTC_Address_Control_Status_2, Alarm_Interrupt);
}
	
void RTC_Close_Alarm(void)
{
	unsigned char Alarm_Interrupt = 0;  //控制/状态寄存器闹铃中断缓存
	Alarm_Interrupt = RTC_Read_Byte(RTC_Address_Control_Status_2);
	Alarm_Interrupt &= ~(3<<2);
	Alarm_Interrupt &= ~(1<<1); //关报警中断
	RTC_Write_Byte(RTC_Address_Control_Status_2, Alarm_Interrupt);
}



void Set_Alarm_Clock(uint8_t *command)
{
	_RTC_Register_Typedef  PCF_DataStruct;
	
	RTC_Clear_GetControlStatus_2();
	PCF_DataStruct.Minute_Alarm = command[4];
	PCF_DataStruct.Day_Alarm = command[1];
	PCF_DataStruct.Hour_Alarm = command[3];
	PCF_DataStruct.WeekDays_Alarm = command[2];
	RTC_SetAlarm(AlarmType_Minutes_EN|AlarmType_Hours_EN,&PCF_DataStruct);
	
	RTC_GetAlarm(&PCF_DataStruct);
	if(PCF_DataStruct.Minute_Alarm == command[4] && PCF_DataStruct.Hour_Alarm == command[3]) 
	{
		WriteUartBuf(0x01);
		UART_Send_t(0x23);
		#if defined( DeBug )
			LOG(LOG_DEBUG,"闹钟设置成功.. \r\n");
		#endif
		
	}
	else 
	{
		WriteUartBuf(0x00);
		UART_Send_t(0x23);
		#if defined( DeBug )
			LOG(LOG_DEBUG,"闹钟设置失败.. \r\n");
		#endif
		
	}
	
	
	
}



void Set_date_timer(uint8_t *command)
{
	_RTC_Register_Typedef  PCF_DataStruct;

	PCF_DataStruct.Years = command[1];
	PCF_DataStruct.Months_Century = command[2];
	PCF_DataStruct.Days = command[3];
	PCF_DataStruct.WeekDays = command[4];
	PCF_DataStruct.Hours = command[5];
	PCF_DataStruct.Minutes = command[6];
	PCF_DataStruct.Seconds = command[7];
	RTC_SetTimeDate(&PCF_DataStruct);
	
	RTC_GetTimeDate(&PCF_DataStruct);
	if(PCF_DataStruct.Minutes == command[6] && PCF_DataStruct.Hours == command[5] && PCF_DataStruct.Days == command[3])
	{
		WriteUartBuf(0x01);
		UART_Send_t(0x22);
		#if defined( DeBug )
			LOG(LOG_DEBUG,"RTC_SET_TRUE.. \r\n");
		#endif
		
	}else 
	{
		WriteUartBuf(0x00);
		UART_Send_t(0x22);
		#if defined( DeBug )
			LOG(LOG_DEBUG,"RTC_SET_FALSE.. \r\n");
		#endif
	}
	

	
}



void Get_date_timer(void)
{
	_RTC_Register_Typedef  PCF_DataStruct;
	RTC_GetTimeDate(&PCF_DataStruct);
	
	WriteUartBuf (PCF_DataStruct.Years);
	WriteUartBuf (PCF_DataStruct.Months_Century);
	WriteUartBuf (PCF_DataStruct.Days);
	WriteUartBuf (PCF_DataStruct.WeekDays);
	WriteUartBuf (PCF_DataStruct.Hours);
	WriteUartBuf (PCF_DataStruct.Minutes);
	WriteUartBuf (PCF_DataStruct.Seconds);
	UART_Send_t(0x24);

}



