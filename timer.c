
#include <reg51.h>
#include "uart.h"
#include "ds18b20.h"
#include "led.h"

//timer0初始化
void timer0_init()
{
	TMOD &= 0XF0;
	TMOD |= 0X1;								            //采用模式 1
	TH0 = (65536-926)/256;		               	 //11.0592Mhz，定时1ms
	TL0 = (65536-926)%256;
	
	//中断设置 
	TF0 = 0;										            //清标记
	ET0 = 1;									                //开定时中断
	EA  = 1;									                 //开总中断

    //启动定时器
    TR0 = 1;
}

//起动TR0
void timer0_start()
{
	TR0 = 1;
}

//停止TR0
void timer0_stop()
{
	TR0 = 0;
}


void timer0_isr()			interrupt 1
{
 	//重装初值 
	TH0 = (65536-926)/256;						
	TL0 = (65536-926)%256;

	//用于判断串口是否接收超时
    if(rx_start == 1)
    {
        rx_timeout++;
        if(rx_timeout>10)
        {
            rx_start = 0;
            rx_ok = 1;											    //10ms
            rx_timeout = 0;									
            rx_buf[uart_cnt] = '\0';				  //超时完成标记
            uart_cnt = 0;									    //清接收数据的下标记录值
        }
    }

    //用于判断DS18B20温度是否转换完成
    if(ds18b20_start == 1 )
    {
        ds18b20_timeout++;
        if(ds18b20_timeout>900)								    	//900ms,大于750均可
        {
            ds18b20_timeout = 0;
						ds18b20_start = 0;
            ds18b20_ok = 1;
        } 
    }
}
