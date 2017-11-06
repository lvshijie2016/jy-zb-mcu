#ifndef _RTC_H
#define _RTC_H
#include "config.h"




#define RTC_Write                            (unsigned char)0xa2  //写命令
#define RTC_Read                             (unsigned char)0xa3  //读命令，或者用（RTC_Write + 1）


//电源复位功能
//
#define RTC_PowerResetEnable                 (unsigned char)0x08
#define RTC_PowerResetDisable                (unsigned char)0x09

//世纪位操作定义
//
#define PCF_Century_SetBitC                      (unsigned char)0x80


/******************************************************************************
                             参数寄存器地址宏定义
******************************************************************************/

#define RTC_Address_Control_Status_1         (unsigned char)0x00  //控制/状态寄存器1
#define RTC_Address_Control_Status_2         (unsigned char)0x01  //控制/状态寄存器2

#define RTC_Address_CLKOUT                   (unsigned char)0x0d  //CLKOUT频率寄存器
#define RTC_Address_Timer                    (unsigned char)0x0e  //定时器控制寄存器
#define RTC_Address_Timer_VAL                (unsigned char)0x0f  //定时器倒计数寄存器

#define RTC_Address_Years                    (unsigned char)0x08  //年
#define RTC_Address_Months                   (unsigned char)0x07  //月
#define RTC_Address_Days                     (unsigned char)0x05  //日
#define RTC_Address_WeekDays                 (unsigned char)0x06  //星期
#define RTC_Address_Hours                    (unsigned char)0x04  //小时
#define RTC_Address_Minutes                  (unsigned char)0x03  //分钟
#define RTC_Address_Seconds                  (unsigned char)0x02  //秒

#define RTC_Alarm_Minutes                    (unsigned char)0x09  //分钟报警
#define RTC_Alarm_Hours                      (unsigned char)0x0a  //小时报警
#define RTC_Alarm_Days                       (unsigned char)0x0b  //日报警
#define RTC_Alarm_WeekDays                   (unsigned char)0x0c  //星期报警

/******************************************************************************
                               参数屏蔽宏定义
******************************************************************************/

#define RTC_Shield_Control_Status_1          (unsigned char)0xa8
#define RTC_Shield_Control_Status_2          (unsigned char)0x11

#define RTC_Shield_Seconds                   (unsigned char)0x7f
#define RTC_Shield_Minutes                   (unsigned char)0x71
#define RTC_Shield_Hours                     (unsigned char)0x3f

#define RTC_Shield_Days                      (unsigned char)0x3f
#define RTC_Shield_WeekDays                  (unsigned char)0x09
#define RTC_Shield_Months_Century            (unsigned char)0x1f
#define RTC_Shield_Years                     (unsigned char)0xff

#define RTC_Shield_Minute_Alarm              (unsigned char)0x7f
#define RTC_Shield_Hour_Alarm                (unsigned char)0x31
#define RTC_Shield_Day_Alarm                 (unsigned char)0x3f
#define RTC_Shield_WeekDays_Alarm            (unsigned char)0x09

#define RTC_Shield_CLKOUT_Frequency          (unsigned char)0x03
#define RTC_Shield_Timer_Control             (unsigned char)0x03
#define RTC_Shield_Timer_Countdown_Value     (unsigned char)0xff


/******************************************************************************
                                 参数宏定义
******************************************************************************/

//
//控制/状态寄存器1 --> 0x00
//
#define PCF_Control_Status_NormalMode            (unsigned char)(~(1<<7))  //普通模式
#define PCF_Control_Status_EXT_CLKMode           (unsigned char)(1<<7)     //EXT_CLK测试模式
#define PCF_Control_ChipRuns                     (unsigned char)(~(1<<7))  //芯片运行
#define PCF_Control_ChipStop                     (unsigned char)(1<<7)     //芯片停止运行，所有芯片分频器异步置逻辑0
#define PCF_Control_TestcClose                   (unsigned char)(~(1<<3))  //电源复位功能失效（普通模式时置逻辑0）
#define PCF_Control_TestcOpen                    (unsigned char)(1<<3)     //电源复位功能有效

//
//控制/状态寄存器2 --> 0x01
//
#define PCF_Control_TI_TF1                       (unsigned char)(~(1<<4))  //当TF有效时INT有效，（取决于TIE的状态）
#define PCF_Control_TI_TF2                       (unsigned char)(1<<4)     //INT脉冲有效，（取决于TIE的状态）
                                                                           //注意：若AF和AIE有有效时，则INT一直有效
#define PCF_Control_ClearAF                      (unsigned char)(~(1<<3))  //清除报警
#define PCF_Control_ClearTF                      (unsigned char)(~(1<<2))
                                                                           //当报警发生时，AF被值逻辑1；在定时器倒计数结束时，
                                                                           //TF被值逻辑1，他们在被软件重写前一直保持原有值，
                                                                           //若定时器和报警中断都请求时，中断源有AF和TF决定，
                                                                           //若要使清除一个标志位而防止另一标志位被重写，应运用逻辑
                                                                           //指令AND
#define PCF_Alarm_INT_Open                       (unsigned char)(1<<1)     //报警中断有效
#define PCF_Alarm_INT_Close                      (unsigned char)(~(1<<1))  //报警中断无效



//频率输出寄存器 --> 0x0d
//
#define PCF_CLKOUT_Close                         (unsigned char)(~(1<<6))  //CLKOUT输出被禁止并设成高阻抗
//定时器控制寄存器 --> 0x0e
#define PCF_Timer_Close                          (unsigned char)(~(1<<6))  //定时器无效

//闹铃开关
//定义闹铃类型
//
#define AlarmType_Minutes_EN                    (unsigned char)0x01
#define AlarmType_Hours_EN                      (unsigned char)0x02
#define AlarmType_Days_EN                       (unsigned char)0x04
#define AlarmType_WeekDays_EN                   (unsigned char)0x08
//
//秒寄存器 --> 0x02
//
#define PCF_Accuracy_ClockNo                     (unsigned char)(1<<7)     //不保证准确的时钟/日历数据
#define PCF_Accuracy_ClockYes                    (unsigned char)(~(1<<7))  //保证准确的时钟/日历数据

//
//分钟闹铃寄存器 --> 0x09
//
#define PCF_Alarm_MinutesOpen                    (unsigned char)(~(1<<7))  //分钟报警有效
#define PCF_Alarm_MinutesClose                   (unsigned char)(1<<7)     //分钟报警无效

//
//小时闹铃寄存器 --> 0x0a
//
#define PCF_Alarm_HoursOpen                      (unsigned char)(~(1<<7))  //小时报警有效
#define PCF_Alarm_HoursClose                     (unsigned char)(1<<7)     //小时报警无效

//
//日期闹铃寄存器 --> 0x0b
//
#define PCF_Alarm_DaysOpen                       (unsigned char)(~(1<<7))  //日报警有效
#define PCF_Alarm_DaysClose                      (unsigned char)(1<<7)     //日报警无效

//
//星期闹铃寄存器 --> 0x0c
//
#define PCF_Alarm_WeekDaysOpen                   (unsigned char)(~(1<<7))  //星期报警有效
#define PCF_Alarm_WeekDaysClose                  (unsigned char)(1<<7)     //星期报警无效






/**
   ==================================================================
                               全部寄存器结构
   ==================================================================
**/
typedef struct
{
	unsigned char Control_Status_1;  //控制寄存器1
	unsigned char Control_Status_2;  //控制寄存器2
	unsigned char Seconds;  //秒寄存器
	unsigned char Minutes;  //分钟寄存器
	unsigned char Hours;  //小时寄存器
	unsigned char Days;  //日期寄存器
	unsigned char WeekDays;  //星期寄存器，数值范围：0 ~ 6
	unsigned char Months_Century;  //月份寄存器，bit7为世纪位，0：指定世纪数为20xx；1：指定世纪数为19xx
	unsigned char Years;  //年寄存器
	unsigned char Minute_Alarm;  //分钟报警寄存器
	unsigned char Hour_Alarm;  //小时报警寄存器
	unsigned char Day_Alarm;  //日期报警寄存器
	unsigned char WeekDays_Alarm;  //星期报警寄存器，数值范围：0 ~ 6
	unsigned char CLKOUT_Frequency;  //频率管脚输出控制寄存器
	unsigned char Timer_Control;  //定时器控制寄存器
	unsigned char Timer_Countdown_Value;  //定时器计数寄存器
}_RTC_Register_Typedef;


void 			RTC_Start					(void);  //RTC启动
void 			RTC_SetTimeDate				(_RTC_Register_Typedef* PCF_DataStruct);  //RTC写入时间日期信息 
void 			RTC_GetTimeDate				(_RTC_Register_Typedef* PCF_DataStruct);		//RTC读取时间日期信息 
void 			RTC_SetAlarm				(unsigned char AlarmType_EN, _RTC_Register_Typedef* PCF_DataStruct);  //RTC写入闹铃信息
void 			RTC_GetAlarm				(_RTC_Register_Typedef* PCF_DataStruct);  //RTC读取闹铃信息
void 			RTC_Clear_GetControlStatus_2(void);
void 			RTC_Close_Alarm				(void);
void 			Set_date_timer				(uint8_t *command);
void 			Set_Alarm_Clock				(uint8_t *command);
void 			Get_date_timer				(void);


#endif





