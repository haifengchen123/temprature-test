
#include <reg51.h>
#include "smg.h"
#include "led.h"
#include "uart.h"
#include "ds18b20.h"
#include "timer.h"

#include <stdio.h>

unsigned char buf[15] = {0};

void main()
{
    unsigned char ret = 0;
    unsigned char shi_z = 0,ge_z = 0,shi_f = 0,ge_f = 0;
    float tempValue = 0.0;
    unsigned char temp = 0;

    unsigned char flag = 0;
    
    smg_init();
    timer0_init();
    uart_init();
    uart_sendStr("this is temprature meter!\r\n");

    while(1)
    {
        if(flag == 0)
        {
            flag = 1;
            ret = ds18b20_startTemConv();
						if(ret == 0xff)		//起动转换失败，这一部分一定要写，否则flag一直为1，将无法再次进行转换
							flag = 0;
        }
        
        if(ds18b20_ok == 1)
        {
            ds18b20_ok = 0;
						led_toggleNumber(1);									//转换一次翻转一下灯，用于表示系统运行
            ret = ds18b20_readTemp(&tempValue) ;
            if(ret == 0)
            {		
								if(tempValue > 0)
								{
									temp = (unsigned char )tempValue;
									sprintf(buf,"temp:%2.2f\r\n",tempValue);
								}
								else
								{
									temp = -(unsigned char )tempValue;
									sprintf(buf,"temp:-%2.2f\r\n",tempValue);
								}
								
								//分离整数部分的十位和个位
                shi_z = temp/10;
                ge_z = temp%10;
								
								//获取到小数部分，如：12.34，得到34
								if(tempValue > 0)
									temp = (unsigned char)((tempValue - temp)*100);
								else
									temp = (unsigned char)((-tempValue - temp)*100);
								
								//分离小数部分的十位和个位
								shi_f = temp/10;
								ge_f = temp%10;
            } 
						
            flag = 0;               //允许下一次转换
						
														
						uart_sendStr(buf);			//温度每转换一次，串口输出一次
        }
        
        //刷新显示
  
				if(tempValue > 0 )
					smg_dispNumber(18,7);			//温度为正，不显示符号位
				else
					smg_dispNumber(17,7);			//温度为负，显示"-"
        smg_dispNumber(shi_z,6);
        smg_dispNumber(ge_z,5);
        smg_dispNumber(16,5);               //叠加小数点
        smg_dispNumber(shi_f,4);
        smg_dispNumber(ge_f,3);
    }

}
