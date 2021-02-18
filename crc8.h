
#ifndef __CRC8_H
#define __CRC8_H

//extern unsigned char code CRC8_MSB_table[];
extern unsigned char code CRC8_LSB_table[];

//unsigned char CRC8_MSB_Bytes(unsigned char *pvalue,unsigned char size);
unsigned char CRC8_LSB_Bytes(unsigned char *pvalue,unsigned char size);

#endif