
#ifndef __DS1302_H
#define __DS1302_H

//�����
//����������ַ������A5-A0
#define 	ADDR_SEC			0X0		
#define 	ADDR_MIN			0X1		
#define 	ADDR_HR				0X2	
#define 	ADDR_DATE			0X3
#define 	ADDR_MONTH		0X4
#define 	ADDR_DAY			0X5
#define 	ADDR_YEAR			0X6
#define 	ADDR_CTRL			0X07

unsigned char ds1302_readClk(unsigned char addr);
//дʱ��
unsigned char ds1302_writeClk(unsigned char addr,unsigned char dat);
//��RAM
unsigned char ds1302_readRam(unsigned char addr);
//дRAM
unsigned char ds1302_writeRam(unsigned char addr,unsigned char dat);

void ds1302_init(void);															//��ʼ��

//�����
void ds1302_stop(void);															//ֹͣDS1302
void ds1302_start(void);														//��DS1302
unsigned char ds1302_readSec(void);									//����
void ds1302_writeSec(unsigned char sec);						//д��

//���Ӳ���
unsigned char ds1302_readMin(void);									//����
void ds1302_writeMin(unsigned char min);						//д��

//ʱ����
unsigned char ds1302_readHour(void);								//��ʱ
void ds1302_writeHour(unsigned char hour);					//дʱ

//������
unsigned char ds1302_readDate(void);
unsigned char ds1302_writeDate(unsigned char date);	//д����

//�·ݲ���
unsigned char ds1302_readMonth(void);								//���·�
unsigned char ds1302_writeMonth(unsigned char month);	//д�·�

//���ڲ���
unsigned char ds1302_readDay(void);										//������
unsigned char ds1302_writeDay(unsigned char day);			//д����

//��ݲ���
unsigned char ds1302_readYear(void);									//����
unsigned char ds1302_writeYear(unsigned char year);		//д��

void ds1302_writeProtect(void);												//д����
void ds1302_cancelProtect(void);											//ȡ��д����


#endif