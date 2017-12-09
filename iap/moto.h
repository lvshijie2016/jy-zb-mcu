#ifndef _MOTO_H
#define _MOTO_H
#include "config.h"
 
#define  MOTO_SP     									  0x05    //序列包
#define  MOTO_RUN_TIMER                                   0x04		//运行时间

#define	 MOTO_RUN_L_DUTY								  0x02    //运行频率
#define	 MOTO_RUN_R_DUTY								  0x03    //运行频率

#define MOTO_ASP                              			  0x01   //动作序列包

#define MOTO_START_TW  																		0x09   //开始跳舞
#define MOTO_TW                                           0x04   //跳舞
#define MOTO_TC                                           0x02   //退出电机运行
#define MOTO_RUN                                          0x01   //电机运行
#define MOTO_Clear                                        0x00   //电机等待运行
#define ACTIONNUM											20     //BUF动作序列包

#define DUTY                          					  10


typedef struct
{
	unsigned char R_duty;
	unsigned char L_duty;			 
	unsigned char timer;
	unsigned char model;
	unsigned char num;
	unsigned char ADFlag;
	unsigned char DancingPag[ACTIONNUM][4];
	
}_MOTO_Typedef_t;


typedef enum
{
	MOTO_P,
	MOTO_D,
	MOTO_H,
	MOTO_R_D,
	MOTO_L_D,
	MOTO_R_H,
	MOTO_L_H,
	MOTO_R_T,
	MOTO_L_T,
	MOTO_MODE_MAX
} moto_mode_t;



void GetMotoCom(unsigned char *com);
void moto_run_task(void);
void moto_P(void);
void moto_D(void);
void get_moto_current_state(uint16_t R_state, uint16_t L_state,uint8_t bat_state);



#endif



