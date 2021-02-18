
#include <reg51.h>

#include "delay.h"
#include "timer.h"
#include "uart.h"
#include "led.h"

#include "crc8.h"

sbit  honey = P2^3;

sbit	dq = P2^2;            	//定义DQ引脚

//初始化
unsigned char ds18b20_init()
{ 
	unsigned char ret = 0;
 
	dq = 1; 										//主机先释放总线
	Delay5us();								
	dq = 0;           					//产生下降沿，用于通知DS18B20
	Delay500us() ;							//维持一段时间
	dq = 1;			  							//主机释放总线，将控制权将DS18B20
	Delay100us();     					//最多60us后，ds18b20即作出应答(持续时间：60~240)
	if(dq == 0)		 							//主机读取DS18B20应答信号
		ret = 0;	   							    
	else
		ret = 0xff;	  						 
	
	Delay500us();      					//维持一段时间
	return ret;									//返回应答结果
}

//向DS18B20发送一个字节
void ds18b20_writeByte(unsigned char Dat) 		
{
	unsigned char i;

	for(i=0; i<8; i++)
	{
		dq = 1;					
		Delay5us();									
		dq = 0;              	 		//起动信号
		Delay5us();							 	//等待一段时间（准备时间）
		if(Dat & 0x01)		 	 			//主机发数据
			dq = 1;         	 				 
		else 
			dq = 0;			 							
	 
		Delay50us();							//等待一段时间，用于DS18B20接收数据
		Dat >>= 1;								//准备发下一位
	}
	dq = 1;                  		//发送完毕，释放总线
	Delay5us();								
}
	

//接收ds18b20返回的数据
unsigned char ds18b20_readByte()
{
	unsigned char i;
	unsigned char Dat = 0;
	
	for(i=0; i<8; i++)
	{
		Dat >>= 1;								//空出最高位，用于接收数据
		dq = 1;                   //主机释放总线
		Delay5us();				 
		dq = 0;         					//起动信号
		Delay5us();				
		dq = 1;										//主机释放总线，由ds18b20控制
		Delay5us();	
		if(dq)										//主机接收数据
			Dat |= 0X80;
		Delay50us();			
	}
	return Dat;
}

//定义缓冲区，用于保存接收的数据
unsigned char ds18b20_buf[9]={0};
//读DS18B2064位ROM值
unsigned char ds18b20_readRomCode()   			
{
	unsigned char i = 0;
	unsigned char ret = 0;
	unsigned char crc = 0;

	ret = ds18b20_init();			 	            //初始化总线
	if(ret != 0)
		return 0xff;						        
	
	ds18b20_writeByte(0x33);	              //发送ROM命令（0x33:读ROM，用于单节点）
	for(i=0;i<8;i++)						            //64位，接收8次
	{
		ds18b20_buf[i] =  ds18b20_readByte();//ds18b20_buf[0]:产品标号（0x28)
																				 //ds18b20_buf[6]-buf[1]:序列号
	}																			 //ds18b20_buf[8]:CRC校验码
	
	//用户可进行校验收到的数据
	crc = CRC8_LSB_Bytes(ds18b20_buf,7);		//校验
	if(crc == ds18b20_buf[7])
		return 0;															//校验正确，返回0
	else 
		return 0xff;													//校验不通过，返回0xff
}


//温度转换
unsigned char ds18b20_ok = 0;						//转换完成标记
unsigned char  ds18b20_start = 0;                  //开始转换标记
unsigned int ds18b20_timeout = 0;               //用于记录转换超时时间

unsigned char ds18b20_startTemConv()
{
	unsigned char ret = 0;

	ret = ds18b20_init();			 	                            //初始化总线 
	if(ret != 0)
		return 0xff;
  
	
	ds18b20_writeByte(0xcc);		 	//跳过ROM(用于单节点)
	ds18b20_writeByte(0x44);		 	//启动转换
	//转换时间用定时中断来实现，这样不影响数码管刷新
	ds18b20_timeout = 0;
	ds18b20_ok = 0;
	ds18b20_start = 1;            //用于同步定时器，使其开始计时

	return 0;
}

//读取缓冲区数据
unsigned	char ds18b20_readBuffer()  
{
	unsigned char i;	
	unsigned char ret = 0;

	ret = ds18b20_init();			
	if(ret != 0)
		return 0xff;
	ds18b20_writeByte(0xcc);			    			//跳过ROM
	ds18b20_writeByte(0xbe);		 	    			//读取全部缓冲区
	for(i=0; i<9; i++)		 			            //读数
	{
		ds18b20_buf[i] =  ds18b20_readByte();	//Tem[0/1]  :温度值
																					//Tem[2/3]  :报警上下限值[2]是上限，[3]是下限
																					//Tem[4]    :配置值
																					//Tem[5/6/7] :保留                                                                                             
	}																				//Tem[8]	 :CRC校验值

	return 0;
}

//获取到温度值
//在ds18b20_ok为1时，转换完成，可读取温度
unsigned char ds18b20_readTemp(float *ptemp)   
{
	unsigned char ret = 0xff;
	unsigned char crc = 0;

	ret = ds18b20_readBuffer();
	if(ret == 0xff)
			return 0xff; 
	else
	{
		//校验结果
		crc = CRC8_LSB_Bytes(ds18b20_buf,8);
		if(crc != ds18b20_buf[8])
			return 0xff;
		else
		{
			//温度数据格式：
			//高字节的高5位为符号位，为1则是负温度，为0则为正温度
			if(ds18b20_buf[1] & 0xf8)		//仅保留符号位，为1，负温度
				 *ptemp = -((ds18b20_buf[1] & 0x07)<<8 | ds18b20_buf[0])*0.0625;//转换成小数
			else
				 *ptemp = ((ds18b20_buf[1] & 0x07)<<8 | ds18b20_buf[0])*0.0625;	
			
			return 0;
		}
	}
}

//写暂存器
//TH_AlarmValue:温度报警上限
//TL_AlarmValue:温度报警下限
//ConfValue:配置值（用于配置精度）
//配置字节格式：MSB 												 LSB
//							  0  R1   R0  1   1   1   1   1
//	R1R0:		00				9位精度，耗时93.75ms
//					01				10位		 耗时93.75x2 ms
//					10				11位		 93.75*4 	ms
//					11				12位		 93.75*8	ms(750ms)
//The factory default of these EEPROM bits is R0=1 and R1=1 (12-bit conversions)
unsigned char ds18b20_writeBuffer(unsigned char TH_AlarmValue,unsigned char TL_AlarmValue,unsigned char ConfValue)
{
	unsigned char ret = 0;

	ret = ds18b20_init();					//初始化总线 
	if(ret != 0)
		return 0xff;
	ds18b20_writeByte (0xcc);		 	//跳过ROM
	ds18b20_writeByte (0x4e);		 	//发送命令，用于写暂存器
	ds18b20_writeByte (TH_AlarmValue);		
	ds18b20_writeByte (TL_AlarmValue);   	
	ds18b20_writeByte (ConfValue);       	

	return 0;
}


//--------- 复制暂存器，将TH/TL/配置字节复制到EEPROM ---------------
unsigned char DS18B20_CopyBuffer()
{
	unsigned char ret = 0;

	ret = ds18b20_init();				
	if(ret != 0)
		return 0xff;
	ds18b20_writeByte (0xcc);			//跳过ROM
	ds18b20_writeByte (0x48);			//复制暂存器
	Delay2ms();               
	
	return 0;
} 