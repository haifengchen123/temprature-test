
#include <reg52.h>
#include "delay.h"
#include "key.h"

#include "led.h"


/*
	按键说明：
	按键电路有两部分构成：
	1. 独立按键；
	2. 矩阵按键。
*/

//独立按键
//	key1 :	P3.4
//	key2 :	P3.5
//	key3 :	P3.6
//	key4 :  P3.7

sbit key1 = P3^4;
sbit key2 = P3^5;
sbit key3 = P3^6;
sbit key4 = P3^7;

//按键扫描
//下面算法即支持单次触发，也支持连续触发

//由于独立按键与矩阵键盘有端口共用，所以识别代码必须分开来写
//并且两者不能同时使用，这里定义全局宏，用户可通过该宏来选择是使用独立按键，还是矩阵键盘


//独立按键识别
unsigned char key_singleScan()
{
	static unsigned char flag = 1;					//用静态封锁，若采用松手检测，则不松手时无法刷新显示数码管
	static unsigned int cnt = 0;
	unsigned char temp = 0;
	unsigned char key = KEYS_UP;
	
	if(flag == 0)		//说明按键有处于闭合的
	{
		delay(5);														//用于记录按键闭合的时间，如超过一定时间，则解除封锁，使能够连续触发
		cnt++;
		if(cnt >= 400)											//累计时长达2秒
		{
			if(cnt%20 == 0)									//2秒及之后，每100ms解除一次封锁		
			{
				flag = 1;												//允许再次扫描，注意：此时按键并未松手,若松手，则清cnt
			}
		}
	}
	
	
	if(flag == 1)				//允许扫描
	{
		P3 = 0XFF;				//全写1，避免对矩阵按键进行识别
		temp = P3;				//读P3口
		temp &= 0xf0;			//保留高4位（key1~key4）
		if(temp != 0xf0)	//有键按下
		{
			delay(10);			//延时消抖
			//再次判断
			P3 = 0xff;				//扫描前，必须先对相应引脚写1，这里先扫描独立按键
			temp = P3;				//读P3口
			temp &= 0xf0;			//保留高4位（key1~key4）
			if(temp != 0xf0)	//确实有键接下
			{
				//封锁再次扫描，松手后解锁
				flag = 0;

				//识别按键
				switch(temp)			//这里仅判断单个按键，即不做同时多键按下的处理
				{
					case 0x70:key = KEY4_DOWN;break;
					case 0xb0:key = KEY3_DOWN;break;
					case 0xd0:key = KEY2_DOWN;break;
					case 0xe0:key = KEY1_DOWN;break;
				}
			}
		}
	}
	else if(flag == 0)
	{
		//是否所有按键都处于松手状态
		P3 = 0XFF;
		temp = P3;
		temp &= 0xf0;
		if(temp == 0xf0)		//说明已全部松手
		{
			flag = 1;					//解除封锁
			key = KEYS_UP;				
			
			cnt = 0;
		}
	}
	
	return key;
	
}

//矩阵键盘扫描
unsigned char key_matrixScan()
{
	static unsigned char flag = 1;			//用静态封锁，若采用松手检测，则不松手时无法刷新显示数码管
	static unsigned int cnt = 0;
	unsigned char temp = 0;
	unsigned char key = KEYS_UP;
	
	//key_mode用于标识当前按键状态:
	// 0 :所有键都无按下
	// 11 :矩阵键第一行按下
	// 22 :矩阵键第二行按下
	// 33 :矩阵键第三行按下
	// 44 :矩阵键第四行按下
	static unsigned char key_mode = 0;	
	
	if(flag == 0)													//说明按键有处于闭合的
	{
		delay(5);														//用于记录按键闭合的时间，如超过一定时间，则解除封锁，使能够连续触发
		cnt++;
		if(cnt >= 400)											//累计时长达2秒
		{
			if(cnt%20 == 0)									//2秒及之后，每100ms解除一次封锁		
			{
				flag = 1;												//允许再次扫描，注意：此时按键并未松手,若松手，则清cnt
			}
		}
	}
	
	if(flag == 1)
	{
		//下面识别矩阵按键
		///////////////  第一行  //////////////////////////
		P3 = 0xfe;						//识别第一行,将第一行的行线置低
		temp = P3;
		temp &= 0xf0;					//仅通过高4位电平状态即可判断哪个键动作
		if(temp != 0xf0)			
		{
			delay(10);
			P3 = 0xfe;
			temp = P3;
			temp &= 0xf0;
			if(temp != 0xf0)		//确定有键按下
			{
				flag = 0;					//封锁下次的扫描
				key_mode = 11;			//标记为矩阵按键按下
				switch(temp)
				{
					case 0xe0:key = S1_DOWN;break;
					case 0xd0:key = S2_DOWN;break;
					case 0xb0:key = S3_DOWN;break;
					case 0x70:key = S4_DOWN;break;
				}
			}
		}
		
		///////////////  第二行   /////////////////
		P3 = 0xfd;						//识别第二行,将第二行的行线置低
		temp = P3;
		temp &= 0xf0;					//仅通过高4位电平状态即可判断哪个键动作
		if(temp != 0xf0)			
		{
			delay(10);
			P3 = 0xfd;
			temp = P3;
			temp &= 0xf0;
			if(temp != 0xf0)		//确定有键按下
			{
				flag = 0;					//封锁下次的扫描
				key_mode = 22;		//标记为矩阵按键按下
				switch(temp)
				{
					case 0xe0:key = S5_DOWN;break;
					case 0xd0:key = S6_DOWN;break;
					case 0xb0:key = S7_DOWN;break;
					case 0x70:key = S8_DOWN;break;
				}
			}
		}
		
		///////////////  第三行   /////////////////
		P3 = 0xfb;						//识别第二行,将第二行的行线置低
		temp = P3;
		temp &= 0xf0;					//仅通过高4位电平状态即可判断哪个键动作
		if(temp != 0xf0)			
		{
			delay(10);
			P3 = 0xfb;
			temp = P3;
			temp &= 0xf0;
			if(temp != 0xf0)		//确定有键按下
			{
				flag = 0;					//封锁下次的扫描
				key_mode = 33;		//标记为矩阵按键按下
				switch(temp)
				{
					case 0xe0:key = S9_DOWN;break;
					case 0xd0:key = S10_DOWN;break;
					case 0xb0:key = S11_DOWN;break;
					case 0x70:key = S12_DOWN;break;
				}
			}
		}
		
		///////////////  第四行   /////////////////
		P3 = 0xf7;						//识别第二行,将第二行的行线置低
		temp = P3;
		temp &= 0xf0;					//仅通过高4位电平状态即可判断哪个键动作
		if(temp != 0xf0)			
		{
			delay(10);
			P3 = 0xf7;
			temp = P3;
			temp &= 0xf0;
			if(temp != 0xf0)		//确定有键按下
			{
				flag = 0;					//封锁下次的扫描
				key_mode = 44;		//标记为矩阵按键按下
				switch(temp)
				{
					case 0xe0:key = S13_DOWN;break;
					case 0xd0:key = S14_DOWN;break;
					case 0xb0:key = S15_DOWN;break;
					case 0x70:key = S16_DOWN;break;
				}
			}
		}
	}
	else if(flag == 0)		 //说明已经有键按下了
	{
		if(key_mode == 0x11)	//判断第一行是否全部松手
		{
			P3 = 0xfe;
			temp = P3;
			temp &= 0xf0;
			if(temp == 0xf0)		//第一行已全部松手
			{
				key_mode = 0;			//key_mode标记为无键按下
				flag = 1;					//解除封锁
				key = KEYS_UP;				
				
				cnt = 0;
			}
		}
		else if(key_mode == 0x22)	//判断第二行是否全部松手
		{
			P3 = 0xfd;
			temp = P3;
			temp &= 0xf0;
			if(temp == 0xf0)				//第二行已全部松手
			{
				key_mode = 0;					//key_mode标记为无键按下
				flag = 1;							//解除封锁
				key = KEYS_UP;				
				
				cnt = 0;
			}
		}
		else if(key_mode == 0x33)	//判断第三行是否全部松手
		{
			P3 = 0xfb;
			temp = P3;
			temp &= 0xf0;
			if(temp == 0xf0)				//第三行已全部松手
			{
				key_mode = 0;					//key_mode标记为无键按下
				flag = 1;							//解除封锁
				key = KEYS_UP;				
				cnt = 0;
			}
		}
		else if(key_mode == 0x44)	//判断第四行是否全部松手
		{
			P3 = 0xf7;
			temp = P3;
			temp &= 0xf0;
			if(temp == 0xf0)				//第四行已全部松手
			{
				key_mode = 0;					//key_mode标记为无键按下
				flag = 1;							//解除封锁
				key = KEYS_UP;				
				
				cnt = 0;
			}
		}
	}
	
	return key;
}

