
#ifndef __DS1302_H
#define __DS1302_H

//命令定义
//这里仅定义地址，即：A5-A0
#define 	ADDR_SEC			0X0		
#define 	ADDR_MIN			0X1		
#define 	ADDR_HR				0X2	
#define 	ADDR_DATE			0X3
#define 	ADDR_MONTH		0X4
#define 	ADDR_DAY			0X5
#define 	ADDR_YEAR			0X6
#define 	ADDR_CTRL			0X07

unsigned char ds1302_readClk(unsigned char addr);
//写时钟
unsigned char ds1302_writeClk(unsigned char addr,unsigned char dat);
//读RAM
unsigned char ds1302_readRam(unsigned char addr);
//写RAM
unsigned char ds1302_writeRam(unsigned char addr,unsigned char dat);

void ds1302_init(void);															//初始化

//秒操作
void ds1302_stop(void);															//停止DS1302
void ds1302_start(void);														//起动DS1302
unsigned char ds1302_readSec(void);									//读秒
void ds1302_writeSec(unsigned char sec);						//写秒

//分钟操作
unsigned char ds1302_readMin(void);									//读分
void ds1302_writeMin(unsigned char min);						//写分

//时操作
unsigned char ds1302_readHour(void);								//读时
void ds1302_writeHour(unsigned char hour);					//写时

//读日期
unsigned char ds1302_readDate(void);
unsigned char ds1302_writeDate(unsigned char date);	//写日期

//月份操作
unsigned char ds1302_readMonth(void);								//读月份
unsigned char ds1302_writeMonth(unsigned char month);	//写月份

//星期操作
unsigned char ds1302_readDay(void);										//读星期
unsigned char ds1302_writeDay(unsigned char day);			//写星期

//年份操作
unsigned char ds1302_readYear(void);									//读年
unsigned char ds1302_writeYear(unsigned char year);		//写年

void ds1302_writeProtect(void);												//写保护
void ds1302_cancelProtect(void);											//取消写保护


#endif