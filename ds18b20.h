
#ifndef __DS18B20_H
#define __DS18B20_H

extern unsigned char ds18b20_ok;		
extern unsigned char  ds18b20_start ;    
extern unsigned int ds18b20_timeout ;  
extern unsigned char ds18b20_buf[10];

unsigned char ds18b20_readRomCode(void);
unsigned char ds18b20_startTemConv(void);
//读取缓冲区数据
unsigned	char ds18b20_readBuffer(void);
  //获取到温度值
//在ds18b20_ok为1时，转换完成，可读取温度
unsigned char ds18b20_readTemp(float *ptemp) ;

//写暂存器
//TH_AlarmValue:温度报警上限
//TL_AlarmValue:温度报警下限
//ConfValue:配置值（用于配置精度）
unsigned char ds18b20_writeBuffer(unsigned char TH_AlarmValue,unsigned char TL_AlarmValue,unsigned char ConfValue);

//复制暂存器，将TH/TL/配置字节复制到EEPROM
unsigned char DS18B20_CopyBuffer(void);



#endif