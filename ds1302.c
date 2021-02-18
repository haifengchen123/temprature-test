
#include <reg52.h>
#include "delay.h"
#include "ds1302.h"

sbit ds1302_sclk = P2^1;
sbit ds1302_sda = P2^0;				//注意：使用时，需用跳帽短接J15
sbit ds1302_rst = P2^4;				//复位，在数据传输期间需拉高电平

//数据在时钟上升沿输入，下降沿输出
//控制指令
/*
	bit7 	bit6			bit5  	bit4		bit3		bit2		bit1  	bit0
	1			ram/#ck		a4			a3			a2			a1			a0			rd/#w

bit6:ram/#ck		：为1，用于操作RAM区，共31个字节
								：为0，用于操作时钟日历
*/

void ds1302_init()
{
	ds1302_rst = 0;
	ds1302_sclk = 0;
	ds1302_cancelProtect();		//取消写保护
	ds1302_start();						//起动走时
}

unsigned char ds1302_readClk(unsigned char addr)
{
	unsigned char temp = 0,i = 0;
	
	ds1302_rst = 0;
	ds1302_sclk = 0;
	Delay5us();								//大于4us即可
	ds1302_rst = 1;						//由低到高，使能读写
	
	temp = addr<<1 | 0x81;		//RD=1,bit7=1
	for(i=0;i<8;i++)					//先写低位，发送地址
	{
		ds1302_sclk = 0;
		if(temp & 0x01)
			ds1302_sda = 1;
		else
			ds1302_sda = 0;
		ds1302_sclk = 1;				//提供上升沿，将数据发送出去

		temp>>=1;								//准备发送下一位
	}
	
	//读取数据
	temp = 0;									//清零，准备接收读到的数，读前SDA = 1
	ds1302_sda = 1;
	
	for(i=0;i<8;i++)
	{
		temp >>=1;								//空出最高位，准备接收（也可直接放置到对应位）
		ds1302_sclk = 0;				//产生下降沿，读取数据
		if(ds1302_sda == 1)
			temp |= 0x80;
		ds1302_sclk = 1;				//拉高时钟
	}
	ds1302_sclk = 0;	
//	Delay5us();								//大于4us即可
	ds1302_rst = 0;			
	
	return temp;
}	

//写时钟
unsigned char ds1302_writeClk(unsigned char addr,unsigned char dat)
{
	unsigned char temp = 0,i = 0;
	
	ds1302_rst = 0;
	ds1302_sclk = 0;
	Delay5us();					//大于4us即可
	ds1302_rst = 1;			//由低到高，使能读写
	
	temp = addr<<1 | 0x81;		//RD=1,bit7=1
	for(i=0;i<8;i++)					//先写低位，发送地址
	{
		ds1302_sclk = 0;
		if(temp & 0x01)
			ds1302_sda = 1;
		else
			ds1302_sda = 0;
		ds1302_sclk = 1;				//提供上升沿，将数据发送出去

		temp>>=1;								//准备发送下一位
	}
	
	temp = dat;								//准备要发送的数据
	for(i=0;i<8;i++)
	{
		ds1302_sclk = 0;
		if(temp & 0x01)
			ds1302_sda = 1;
		else
			ds1302_sda = 0;
		ds1302_sclk = 1;				//提供上升沿，将数据发送出去

		temp>>=1;								//准备发送下一位
	}
	ds1302_sclk = 0;	
//	Delay5us();							//大于4us即可
	ds1302_rst = 0;			
	
	return temp;
}

//读RAM
unsigned char ds1302_readRam(unsigned char addr)
{
	unsigned char temp = 0,i = 0;
	
	ds1302_rst = 0;
	ds1302_sclk = 0;
	Delay5us();					//大于4us即可
	ds1302_rst = 1;			//由低到高，使能读写
	
	temp = addr<<1 | 0xc1;		//RD=1,bit7=1,bit6=1
	for(i=0;i<8;i++)					//先写低位，发送地址
	{
		ds1302_sclk = 0;
		if(temp & 0x01)
			ds1302_sda = 1;
		else
			ds1302_sda = 0;
		ds1302_sclk = 1;				//提供上升沿，将数据发送出去

		temp>>=1;								//准备发送下一位
	}
	
	//读取数据
	temp = 0;									//清零，准备接收读到的数，读前SDA = 1
	ds1302_sda = 1;
	
	for(i=0;i<8;i++)
	{
		temp >>=1;								//空出最高位，准备接收（也可直接放置到对应位）
		ds1302_sclk = 0;					//产生下降沿，读取数据
		if(ds1302_sda == 1)
			temp |= 0x80;
		ds1302_sclk = 1;				//拉高时钟
	}
	ds1302_sclk = 0;	
//	Delay5us();								//大于4us即可
	ds1302_rst = 0;			
	
	return temp;
}	

//写RAM
unsigned char ds1302_writeRam(unsigned char addr,unsigned char dat)
{
	unsigned char temp = 0,i = 0;
	
	ds1302_rst = 0;
	ds1302_sclk = 0;
	Delay5us();					//大于4us即可
	ds1302_rst = 1;			//由低到高，使能读写
	
	temp = addr<<1 | 0xc1;		//RD=1,bit7=1
	for(i=0;i<8;i++)					//先写低位，发送地址
	{
		ds1302_sclk = 0;
		if(temp & 0x01)
			ds1302_sda = 1;
		else
			ds1302_sda = 0;
		ds1302_sclk = 1;				//提供上升沿，将数据发送出去

		temp>>=1;								//准备发送下一位
	}
	
	temp = dat;								//准备要发送的数据
	for(i=0;i<8;i++)
	{
		ds1302_sclk = 0;
		if(temp & 0x01)
			ds1302_sda = 1;
		else
			ds1302_sda = 0;
		ds1302_sclk = 1;				//提供上升沿，将数据发送出去

		temp>>=1;								//准备发送下一位
	}
	ds1302_sclk = 0;	
//	Delay5us();							//大于4us即可
	ds1302_rst = 0;			
	
	return temp;
}

//时钟寄存器中数据存储格式为BCD码
unsigned char  bcd2dec(unsigned char bcd)
{
	unsigned char dec = 0;
	
	dec = (bcd>>4)*10 + (bcd & 0x0f);
	
	return dec;
}

unsigned char  dec2bcd(unsigned char dec)
{
	unsigned char bcd = 0;
	
	bcd = ((dec/10)<<4 ) | (dec%10);

	return bcd;
}


//秒的读、写
unsigned char ds1302_readSec()
{
	unsigned char temp = 0;
	
	temp = ds1302_readClk(ADDR_SEC);
	temp &= 0x7f;						//最高位是停止位，屏蔽掉
	
	temp = bcd2dec(temp);		//转成10进制
	return temp;						
}

//写秒
void ds1302_writeSec(unsigned char sec)
{
	sec %= 60;			//限制取值：0~59
	sec = dec2bcd(sec);	//转bcd
	ds1302_writeClk(ADDR_SEC,sec);
}

//停止DS1302
void ds1302_stop(void)
{
	unsigned char temp = 0;
	
	temp = ds1302_readClk(ADDR_SEC);
	temp |= 0x80;
	ds1302_writeClk(ADDR_SEC,temp);
}

//起动ds1302
void ds1302_start(void)
{
	unsigned char temp = 0;
	
	temp = ds1302_readClk(ADDR_SEC);
	temp &= 0x7f;
	ds1302_writeClk(ADDR_SEC,temp);
}

//读分
unsigned char ds1302_readMin()
{
	unsigned char temp = 0;
	
	temp = ds1302_readClk(ADDR_MIN);
	temp = bcd2dec(temp);		//转成10进制
	return temp;						
}

//写分
void ds1302_writeMin(unsigned char min)
{
	min %= 60;			//限制取值：0~59
	min = dec2bcd(min);	//转bcd
	ds1302_writeClk(ADDR_MIN,min);
}

//这里采用24小时制
//读时
//读分
unsigned char ds1302_readHour()
{
	unsigned char temp = 0;
	
	temp = ds1302_readClk(ADDR_HR);
	temp &= 0x3f;											//只要低6位
	temp = bcd2dec(temp);							//转成10进制
	return temp;						
}

//写时
void ds1302_writeHour(unsigned char hour)
{
	hour %= 24;			//限制取值：0~23
	hour = dec2bcd(hour);	//转bcd
	ds1302_writeClk(ADDR_HR,hour);
}


//读日期
unsigned char ds1302_readDate()
{
	unsigned char temp = 0;
	
	temp = ds1302_readClk(ADDR_DATE);
	temp &= 0x3f;											//只要低6位
	temp = bcd2dec(temp);							//转成10进制
	return temp;						
}


//写日期
unsigned char ds1302_writeDate(unsigned char date)
{
	unsigned char temp = 0;
	
	temp = ds1302_readClk(ADDR_MONTH);			//先读月份，根据月份限制参数取值范围
	temp = bcd2dec(temp);
	switch(temp)
	{
		case 1:		//1~31天
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			if(date > 31 || date < 1)
				temp = 0xff;
			break;
		case 2:
				temp = ds1302_readClk(ADDR_YEAR);
				temp = bcd2dec(temp);
				if((temp%4==0  && temp%100 != 0) || temp%400 == 0)		//闰年
				{
					if(date<1 || date>29)		//闰年，不能超29
						temp = 0xff;
				}
				else
				{
					if(date<1 || date>28)
						temp = 0xff;
				}
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			if(date>30 || date<1)
				temp = 0xff;
			break;
	}
	
	if(temp != 0xff)
	{
		date = dec2bcd(date);
		ds1302_writeClk(ADDR_DATE,date);
		temp = 0;																//成功写入
	}
	
	return temp;						
}

//读月份
unsigned char ds1302_readMonth()
{
	unsigned char temp = 0;
	
	temp = ds1302_readClk(ADDR_MONTH);
	temp &= 0x1f;
	temp = bcd2dec(temp);
	
	return temp;
}

//写月份
unsigned char ds1302_writeMonth(unsigned char month)
{
	unsigned char temp = 0;
	
	if(month<1 || month>12)
		temp = 0xff;
	else
	{
		month = dec2bcd(month);
		ds1302_writeClk(ADDR_MONTH,month);
	}
		
	return temp;
}


//读星期
unsigned char ds1302_readDay()
{
	unsigned char temp = 0;
	
	temp = ds1302_readClk(ADDR_DAY);
	temp &= 0x07;
	temp = bcd2dec(temp);
	
	return temp;
}

//写星期
unsigned char ds1302_writeDay(unsigned char day)
{
	unsigned char temp = 0;
	
	if(day<1 || day>7)
		temp = 0xff;
	else
	{
		day = dec2bcd(day);
		ds1302_writeClk(ADDR_DAY,day);
	}
		
	return temp;
}

//读年
unsigned char ds1302_readYear()
{
	unsigned char temp = 0;
	
	temp = ds1302_readClk(ADDR_YEAR);
	temp = bcd2dec(temp);
	
	return temp;
}

//写年
unsigned char ds1302_writeYear(unsigned char year)
{
	unsigned char temp = 0;
	
	if(year>99)
		temp = 0xff;
	else
	{
		year = dec2bcd(year);
		ds1302_writeClk(ADDR_YEAR,year);
	}
		
	return temp;
}

//写保护
void ds1302_writeProtect()
{	
	ds1302_writeClk(ADDR_CTRL,0x80);	//最高位写1
}

//取消写保护
void ds1302_cancelProtect()
{						
	ds1302_writeClk(ADDR_CTRL,0);
}



