/*****************************************************************************
 *   rtc.h:  Header file for C32F0xx
 *
 *   Copyright(C) 2015,
 *   All rights reserved.
 *
 *   History
 *   2012.07.21  ver 1.01    Prelimnary version
 *
******************************************************************************/
/* Includes ------------------------------------------------------------------*/

#include "c32f0xx.h"
#include "c32f0xx_rtc.h"
#include "c32f0xx_sys.h"
/*****************************************************************************
Function Name	RTC_DateTimetoSecond
Function Definition	uint32_t RTC_DateTimetoSecond(DateTime_TypeDef *dt)
Function Description	convert datetime to seconds since 2000.01.01 00:00
Input Parameters	*dt: datetime defined in RTC.h
Return Value	Seconds value
Condition	No
Function called	-

Last Chang Date: 2015/09/12
*****************************************************************************/
uint32_t RTC_DateTimetoSecond(DateTime_TypeDef *dt)
{
	uint16_t initialyear=2000;
	uint32_t days,seconds;
	if ((dt->year<initialyear)||(dt->month==0)||(dt->month>12)||(dt->day==0)||(dt->day>31)||(dt->hour>23)||(dt->minute>59)||(dt->second>59))
	    return 0;
	//current yaer days
	switch(dt->month)
	{
		case 1:
			days=dt->day;
			break;
		case 2:
			days=31+dt->day;
			break;
		default:
			days=(dt->month-1)*30-2; 
			if (dt->month<9)
			{
				days+=dt->month>>1;
			}else
			{
				days+=((dt->month-1)>>1+1);
			}
			
			if (!(((dt->year&0x03)||(dt->year/400)*400!=dt->year)))
				days++; //leap year
			days+=(dt->day-1);
			break;
	}
	//the days since 2000 and before this year
	days=days+365*(dt->year-initialyear)+(dt->year-initialyear)/4+1;
	
	seconds=days*86400+dt->hour*3600+dt->minute*60+dt->second;
	
	return seconds;
	
}
/*****************************************************************************
Function Name	RTC_SecondtoDateTime
Function Definition	void RTC_SecondtoDateTime(uint32_t seconds, DateTime_TypeDef *dt)
Function Description	Convert Seconds to datetime
Input Parameters	seconds: seconds since 2000:01:01 00:00
									*dt: datetime defined in RTC.h
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12
*****************************************************************************/
void RTC_SecondtoDateTime(uint32_t seconds, DateTime_TypeDef *dt)
{
	uint8_t monthday[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	uint32_t days,leapdays;
	
	days=seconds/86400; //convert seconds to days
	seconds-=days*86400;
	dt->hour=seconds/3600; //get hour
	seconds-=dt->hour*3600; 
	dt->minute=seconds/60; //get minute
	dt->second=seconds-dt->minute*60; //get second
	
	dt->year=days/365;
	leapdays=dt->year>>2+1;
	if (dt->year*365+leapdays>days)
		dt->year--;
	days-=(dt->year*365+dt->year>>2+1);
	dt->year+=2001;
	
	if (!(dt->year&0x3))
		monthday[1]=29; //leap year
	
	dt->month=1;
	while(days>monthday[dt->month-1])
	{
		days-=monthday[dt->month-1];
		dt->month++;
	}
	dt->day=days;
	return;
}
/*****************************************************************************
Function Name	RTC_SetDateTime
Function Definition	void RTC_SetDateTime(DateTime_TypeDef *dt)
Function Description	Set date and time
Input Parameters	*dt: datetime defined in RTC.h
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void RTC_SetDateTime(DateTime_TypeDef *dt)
{
	RTC->LR.all=RTC_DateTimetoSecond(dt);
	return;
}
/*****************************************************************************
Function Name	RTC_GetDateTime
Function Definition	void RTC_GetDateTime(DateTime_TypeDef *dt)
Function Description	Get date and time
Input Parameters	*dt: datetime defined in RTC.h
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void RTC_GetDateTime(DateTime_TypeDef *dt)
{
	RTC_SecondtoDateTime(RTC->DR.all, dt);
	return;
}
/*****************************************************************************
Function Name	RTC_SetAlarmDateTime
Function Definition	void RTC_SetAlarmDateTime (DateTime_TypeDef *dt)
Function Description	Set alarm date and time 
Input Parameters	*dt: datetime defined in RTC.h
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void RTC_SetAlarmDateTime (DateTime_TypeDef *dt)
{
	//set alarm match register
	RTC->MR.all=RTC_DateTimetoSecond(dt);
	//enable rtc interrupt
	RTC->ICSC.bit.RTCIC=1;
	return;		
}
/*****************************************************************************
Function Name	RTC_SetAlarmDateTime
Function Definition	void RTC_SetAlarmDateTime (DateTime_TypeDef *dt)
Function Description	Set alarm date and time 
Input Parameters	*dt: datetime defined in RTC.h
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void RTC_SetAlarmInTickLater(uint32_t ticks)
{
	//set alarm match register
	RTC->MR.all=RTC->DR.all+ticks;
	//enable rtc interrupt
	RTC->ICSC.bit.RTCIC=1;
	return;
}
/*****************************************************************************
Function Name	RTC_EnableAlarmInt
Function Definition	void RTC_EnableAlarmInt(void)
Function Description	Set alarm date and time 
Input Parameters	*dt: datetime defined in RTC.h
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void RTC_EnableAlarmInt(void)
{
	//enable rtc interrupt
	RTC->ICSC.bit.RTCIC=1;
	return;
}	
/*****************************************************************************
Function Name	RTC_DisableAlarmInt
Function Definition	void RTC_EnableAlarmInt(void)
Function Description	Disable alarm interrupt
Input Parameters	No
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void RTC_DisableAlarmInt(void)
{
	//disable rtc interrupt
	RTC->ICSC.bit.RTCIC=0;
	return;
}	
/*****************************************************************************
Function Name	RTC_ClearIntFlag
Function Definition	void RTC_ClearIntFlag(void)
Function Description	Clear alarm interrupt flag
Input Parameters	No
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void RTC_ClearIntFlag(void)
{
	//disable rtc interrupt
	RTC->ICR.bit.RTCICR=1;
	return;
}
/*****************************************************************************
Function Name	RTC_SetLoader
Function Definition	void RTC_SetLoader(uint32_t val)
Function Description	Set RTC timer new initial vale
Input Parameters	Val: new initial value
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
void RTC_SetLoader(uint32_t val)
{
	//set loader
	RTC->LR.all=val;
	return;
}	
/*****************************************************************************
Function Name	RTC_GetTimer
Function Definition	uint32_t RTC_GetTimer(void)
Function Description	Get RTC timer vale
Input Parameters	No 
Return Value	Current Timer value
Condition	No
Function called	-

Last Chang Date: 2015/09/12			By:YL
*****************************************************************************/
uint32_t RTC_GetTimer(void)
{
	return RTC->DR.all;
}	


/******************************************************************************
**                            End Of File
******************************************************************************/
