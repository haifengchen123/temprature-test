

#include <intrins.h>

void Delay5us()		//@11.0592MHz
{
}

void Delay10us()		//@11.0592MHz
{
	unsigned char i;

	i = 2;
	while (--i);
}

void Delay15us()		//@11.0592MHz
{
	unsigned char i;

	i = 4;
	while (--i);
}

void Delay50us()		//@11.0592MHz
{
	unsigned char i;

	 _nop_();
	 i = 20;
	while (--i);
}

void Delay60us()		//@11.0592MHz
{
	unsigned char i;

	i = 25;
	while (--i);
}


void Delay100us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 43;
	while (--i);
}

void Delay200us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 89;
	while (--i);
}




void Delay500us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 227;
	while (--i);
}

void Delay600us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	i = 2;
	j = 15;
	do
	{
		while (--j);
	} while (--i);
}


void Delay750us()		//@11.0592MHz
{
	unsigned char i, j;

  _nop_();                 //忽略一个1.08us
	i = 2;
	j = 84;
	do
	{
		while (--j);
	} while (--i);
}


void Delay2ms()		//@11.0592MHz
{
	unsigned char i, j;

  _nop_();
	i = 4;
	j = 146;
	do
	{
		while (--j);
	} while (--i);
}

void Delay5ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 9;
	j = 244;
	do
	{
		while (--j);
	} while (--i);
}


void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 18;
	j = 235;
	do
	{
		while (--j);
	} while (--i);
}


void Delay25ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 45;
	j = 208;
	do
	{
		while (--j);
		
	} while (--i);
}

//不精确延时
void delay(unsigned int ms)
{
    unsigned int i ,j;

    for(i = ms; i > 0; i--)
        for(j = 110; j > 0; j--);
}

