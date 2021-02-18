#ifndef __DELAY_H
#define __DELAY_H

//精确延时
void Delay5us(void);
void Delay10us(void);
void Delay15us(void);
void Delay50us(void);
void Delay60us(void)	;
void Delay100us(void)	;
void Delay200us(void);
void Delay500us(void);
void Delay600us(void);		//@11.0592MHz
void Delay750us(void);
void Delay2ms(void);
void Delay5ms(void);
void Delay10ms(void);
void Delay25ms(void);

//不精确延时
void delay(unsigned int ms);

#endif