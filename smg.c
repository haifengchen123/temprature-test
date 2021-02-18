
#include "delay.h"
#include <reg51.h>

#define SMG_PORT    P0
sbit SEG_CTRL  =   P2^6;           //段控
sbit POS_CTRL  =   P2^7;           //位控
/*
位选：P0口，位控P2.7
段选：P0口，段控P2.6
锁存器：74HC573，高电平直通，下降沿锁存
*/

//数码管段码:共阴数码管(0~9 A~F . - 灭)
 unsigned char code smg_code[] = {
    0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71, 0x80, 0x40, 0x00
};

void smg_init()
{
	POS_CTRL = 1;               
	SMG_PORT = 0X0;         //开所有数码管
	POS_CTRL = 0;		        
	SEG_CTRL = 1;
	SMG_PORT =0X0;			                //开所有段
	delay(2);                                           //熄灭所有段
	SEG_CTRL = 0;                               //锁存
}

/*
    在第pos个数码管上显示数字number
    注：此函数适用于动态扫描
*/
void smg_dispNumber(unsigned char number,unsigned char pos)
{
    POS_CTRL = 1;					
	SMG_PORT = ~(1<<(7-pos));	
	POS_CTRL = 0;
	
	SEG_CTRL = 1;
	SMG_PORT = smg_code[number];
	delay(2);
	SMG_PORT = 0X0;						    //消隐
	SEG_CTRL = 0;
}