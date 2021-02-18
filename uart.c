
#include <reg51.h>

#include "uart.h"
#include "timer.h"

void uart_init(void)		//9600bps@11.0592MHz
{
	PCON &= 0x7F;				
	SCON = 0x50;				//使能接收
	TMOD &= 0x0F;			
	TMOD |= 0x20;			  //T1采用方式2,用于产生波特率
	TL1 = 0xFD;				      //设置初值
	TH1 = 0xFD;					  //设置初值
		
	ES = 1;							    //开中断
	EA = 1;							  
	TR1 = 1;						 //使能T1
}

void uart_sendByte(unsigned char ch)
{
	ES = 0;							     //暂停中断（发送不采用中断）
	SBUF = ch;					    //发数据
	while(TI == 0){}		     //等待发送完毕
	TI = 0;						    	 //清标记
	ES = 1;							    //开中断
}

void uart_sendStr(unsigned char *str)
{
	while(*str != '\0')
	{
		uart_sendByte(*str);		            //发送字符
		str++;									                //偏移指针
	}
}

unsigned char rx_ok = 0;				   //接收完成同步标记
unsigned char rx_buf[10] = {0};	     //接收数据缓冲区
unsigned char uart_cnt = 0;
unsigned char rx_start = 0;              //开始接收数据标记
unsigned char rx_timeout = 0;      //接收超时

void uart_irq()	interrupt 4
{
	if(TI == 1)
		TI = 0;
	else if(RI == 1)
	{
        rx_start = 1;                                      //开始接收标记置1
		RI = 0;											        //清标记
		rx_timeout = 0;						    	//清两次接收之间的时间值
		
		rx_buf[uart_cnt] = SBUF;          //接收数据
		uart_cnt++;
		if(uart_cnt == 9)
		{
			rx_buf[uart_cnt] = '\0';			//最多接收9个字符（最后一个位置存'\0')
			uart_cnt = 0;
			rx_ok = 1;
		}
	}
}