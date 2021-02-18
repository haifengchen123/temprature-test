
#include <reg51.h>
/*
    需要连接J2
 */

//宏定义
#define LED_PORT        P1

//LED全亮
void led_allOn()
{
    LED_PORT = 0;
}

//LED全灭
void led_allOff()
{
    LED_PORT = 0xff;
}

//点亮第n盏灯
//number: 0~7，（0对应D1）
void led_onNumber(unsigned char number)
{
    LED_PORT &= ~(1<<number);
}

//熄灭第n盏灯
//number: 0~7，（0对应D1）
void led_offNumber(unsigned char number)
{
    LED_PORT |= 1<<number;
}

//翻转第n盏灯
void led_toggleNumber(unsigned char number)
{
    LED_PORT ^= 1<<number;
}
