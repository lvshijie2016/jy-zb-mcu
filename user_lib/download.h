#ifndef __DOWNLOAD_H
#define __DOWNLOAD_H 	

#include "c32f0xx.h"
#include "c32f0xx_iap.h"
#include "stdio.h"
#include "RF.h"

#define ApplicationAddress    (0x800)    //2KB
#define FLASH_SIZE            (0x8000)   /* 32 KBytes */
#define FLASH_IMAGE_SIZE      (uint32_t) (FLASH_SIZE - ApplicationAddress)

#define DOWNLOAD       (0x31)  //接收到下载指令
#define DOWNLOADING    (0x32)  //发送处于下载状态
#define ACK            (0x06)  //接收一个完整数据包
#define SEND_AGAIN     (0x07)  //数据包有误，再发送
#define SEND_ALL_AGAIN (0x08)  //数据链路有误，重新发送整包数据     
#define EOT            (0x04)  //结束传输

#define BYTE_TIME_OVER     5000   //数据包接收超时判断
#define DOWNLOAD_TIME_OVER 100      //下载超时

//数据包格式 |包头(2)   |第几个包(1) |总共几个包(1) |总共数据长度(2) |本包数据长度(1) |数据(128)  |数据校验(1) |包尾(1)
//            0xFF 0x55  0x00~0xff    0x00~0xff       0~0x7000          0~128            [128]        CRC_SUM      0xEE   
#define PACKET_B_HEADER1           0xFF
#define PACKET_B_HEADER2           0x55

#define PACKET_B_HEADER           (2)//0XFF  0X55
#define PACKET_B_NUM              (1)//第几个包
#define PACKET_B_TOTAL_NUM        (1)//总共几个包
#define PACKET_B_DATA_TOTAL_SIZE  (2)//总共数据长度
#define PACKET_B_DATA_SIZE        (1)//本包数据长度
#define PACKET_B_OVERHEAD         (PACKET_B_HEADER + PACKET_B_NUM + PACKET_B_TOTAL_NUM + PACKET_B_DATA_TOTAL_SIZE + PACKET_B_DATA_SIZE)
#define PACKET_B_DATA             (128)//有效数据
#define PACKET_B_CHECK_SUM        (1)  //校验位
#define PACKET_B_TAIL             (1)  //0xEE一般不检查包尾
#define PACKET_B_SIZE             (PACKET_B_OVERHEAD + PACKET_B_DATA + PACKET_B_CHECK_SUM + PACKET_B_TAIL)

typedef  void (*pFunction)(void);
static pFunction Jump_To_Application;
static uint32_t JumpAddress;

//硬件及底层封装接口实现
uint8_t get_commond(void);
void send_ack(uint8_t ret);

//功能函数
void erash_flash(void);
void wirte_data_to_flash(uint8_t * data, uint16_t packet_length, uint16_t size);
uint8_t check_and_download(uint8_t * data);
uint8_t get_pack_data(void);
uint8_t Download(void);
void Main_Menu(void);
	
#endif
