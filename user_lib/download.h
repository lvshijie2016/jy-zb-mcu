#ifndef __DOWNLOAD_H
#define __DOWNLOAD_H 	

#include "c32f0xx.h"
#include "c32f0xx_iap.h"
#include "stdio.h"
#include "RF.h"

#define ApplicationAddress    (0x800)    //2KB
#define FLASH_SIZE            (0x8000)   /* 32 KBytes */
#define FLASH_IMAGE_SIZE      (uint32_t) (FLASH_SIZE - ApplicationAddress)

#define DOWNLOAD       (0x31)  //���յ�����ָ��
#define DOWNLOADING    (0x32)  //���ʹ�������״̬
#define ACK            (0x06)  //����һ���������ݰ�
#define SEND_AGAIN     (0x07)  //���ݰ������ٷ���
#define SEND_ALL_AGAIN (0x08)  //������·�������·�����������     
#define EOT            (0x04)  //��������

#define BYTE_TIME_OVER     5000   //���ݰ����ճ�ʱ�ж�
#define DOWNLOAD_TIME_OVER 100      //���س�ʱ

//���ݰ���ʽ |��ͷ(2)   |�ڼ�����(1) |�ܹ�������(1) |�ܹ����ݳ���(2) |�������ݳ���(1) |����(128)  |����У��(1) |��β(1)
//            0xFF 0x55  0x00~0xff    0x00~0xff       0~0x7000          0~128            [128]        CRC_SUM      0xEE   
#define PACKET_B_HEADER1           0xFF
#define PACKET_B_HEADER2           0x55

#define PACKET_B_HEADER           (2)//0XFF  0X55
#define PACKET_B_NUM              (1)//�ڼ�����
#define PACKET_B_TOTAL_NUM        (1)//�ܹ�������
#define PACKET_B_DATA_TOTAL_SIZE  (2)//�ܹ����ݳ���
#define PACKET_B_DATA_SIZE        (1)//�������ݳ���
#define PACKET_B_OVERHEAD         (PACKET_B_HEADER + PACKET_B_NUM + PACKET_B_TOTAL_NUM + PACKET_B_DATA_TOTAL_SIZE + PACKET_B_DATA_SIZE)
#define PACKET_B_DATA             (128)//��Ч����
#define PACKET_B_CHECK_SUM        (1)  //У��λ
#define PACKET_B_TAIL             (1)  //0xEEһ�㲻����β
#define PACKET_B_SIZE             (PACKET_B_OVERHEAD + PACKET_B_DATA + PACKET_B_CHECK_SUM + PACKET_B_TAIL)

typedef  void (*pFunction)(void);
static pFunction Jump_To_Application;
static uint32_t JumpAddress;

//Ӳ�����ײ��װ�ӿ�ʵ��
uint8_t get_commond(void);
void send_ack(uint8_t ret);

//���ܺ���
void erash_flash(void);
void wirte_data_to_flash(uint8_t * data, uint16_t packet_length, uint16_t size);
uint8_t check_and_download(uint8_t * data);
uint8_t get_pack_data(void);
uint8_t Download(void);
void Main_Menu(void);
	
#endif
