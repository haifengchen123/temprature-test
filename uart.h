
#ifndef __UART_H
#define __UART_H

//全局变量声明
extern unsigned char rx_ok;
extern unsigned char rx_buf[10];
extern unsigned char uart_cnt;
extern unsigned char rx_start;
extern unsigned char rx_timeout;

void uart_init(void);
void uart_sendByte(unsigned char ch);
void uart_sendStr(unsigned char *str);

#endif
