
#include <reg52.h>
#include "delay.h"
#include "key.h"

#include "led.h"


/*
	����˵����
	������·�������ֹ��ɣ�
	1. ����������
	2. ���󰴼���
*/

//��������
//	key1 :	P3.4
//	key2 :	P3.5
//	key3 :	P3.6
//	key4 :  P3.7

sbit key1 = P3^4;
sbit key2 = P3^5;
sbit key3 = P3^6;
sbit key4 = P3^7;

//����ɨ��
//�����㷨��֧�ֵ��δ�����Ҳ֧����������

//���ڶ����������������ж˿ڹ��ã�����ʶ��������ֿ���д
//�������߲���ͬʱʹ�ã����ﶨ��ȫ�ֺ꣬�û���ͨ���ú���ѡ����ʹ�ö������������Ǿ������


//��������ʶ��
unsigned char key_singleScan()
{
	static unsigned char flag = 1;					//�þ�̬���������������ּ�⣬������ʱ�޷�ˢ����ʾ�����
	static unsigned int cnt = 0;
	unsigned char temp = 0;
	unsigned char key = KEYS_UP;
	
	if(flag == 0)		//˵�������д��ڱպϵ�
	{
		delay(5);														//���ڼ�¼�����պϵ�ʱ�䣬�糬��һ��ʱ�䣬����������ʹ�ܹ���������
		cnt++;
		if(cnt >= 400)											//�ۼ�ʱ����2��
		{
			if(cnt%20 == 0)									//2�뼰֮��ÿ100ms���һ�η���		
			{
				flag = 1;												//�����ٴ�ɨ�裬ע�⣺��ʱ������δ����,�����֣�����cnt
			}
		}
	}
	
	
	if(flag == 1)				//����ɨ��
	{
		P3 = 0XFF;				//ȫд1������Ծ��󰴼�����ʶ��
		temp = P3;				//��P3��
		temp &= 0xf0;			//������4λ��key1~key4��
		if(temp != 0xf0)	//�м�����
		{
			delay(10);			//��ʱ����
			//�ٴ��ж�
			P3 = 0xff;				//ɨ��ǰ�������ȶ���Ӧ����д1��������ɨ���������
			temp = P3;				//��P3��
			temp &= 0xf0;			//������4λ��key1~key4��
			if(temp != 0xf0)	//ȷʵ�м�����
			{
				//�����ٴ�ɨ�裬���ֺ����
				flag = 0;

				//ʶ�𰴼�
				switch(temp)			//������жϵ���������������ͬʱ������µĴ���
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
		//�Ƿ����а�������������״̬
		P3 = 0XFF;
		temp = P3;
		temp &= 0xf0;
		if(temp == 0xf0)		//˵����ȫ������
		{
			flag = 1;					//�������
			key = KEYS_UP;				
			
			cnt = 0;
		}
	}
	
	return key;
	
}

//�������ɨ��
unsigned char key_matrixScan()
{
	static unsigned char flag = 1;			//�þ�̬���������������ּ�⣬������ʱ�޷�ˢ����ʾ�����
	static unsigned int cnt = 0;
	unsigned char temp = 0;
	unsigned char key = KEYS_UP;
	
	//key_mode���ڱ�ʶ��ǰ����״̬:
	// 0 :���м����ް���
	// 11 :�������һ�а���
	// 22 :������ڶ��а���
	// 33 :����������а���
	// 44 :����������а���
	static unsigned char key_mode = 0;	
	
	if(flag == 0)													//˵�������д��ڱպϵ�
	{
		delay(5);														//���ڼ�¼�����պϵ�ʱ�䣬�糬��һ��ʱ�䣬����������ʹ�ܹ���������
		cnt++;
		if(cnt >= 400)											//�ۼ�ʱ����2��
		{
			if(cnt%20 == 0)									//2�뼰֮��ÿ100ms���һ�η���		
			{
				flag = 1;												//�����ٴ�ɨ�裬ע�⣺��ʱ������δ����,�����֣�����cnt
			}
		}
	}
	
	if(flag == 1)
	{
		//����ʶ����󰴼�
		///////////////  ��һ��  //////////////////////////
		P3 = 0xfe;						//ʶ���һ��,����һ�е������õ�
		temp = P3;
		temp &= 0xf0;					//��ͨ����4λ��ƽ״̬�����ж��ĸ�������
		if(temp != 0xf0)			
		{
			delay(10);
			P3 = 0xfe;
			temp = P3;
			temp &= 0xf0;
			if(temp != 0xf0)		//ȷ���м�����
			{
				flag = 0;					//�����´ε�ɨ��
				key_mode = 11;			//���Ϊ���󰴼�����
				switch(temp)
				{
					case 0xe0:key = S1_DOWN;break;
					case 0xd0:key = S2_DOWN;break;
					case 0xb0:key = S3_DOWN;break;
					case 0x70:key = S4_DOWN;break;
				}
			}
		}
		
		///////////////  �ڶ���   /////////////////
		P3 = 0xfd;						//ʶ��ڶ���,���ڶ��е������õ�
		temp = P3;
		temp &= 0xf0;					//��ͨ����4λ��ƽ״̬�����ж��ĸ�������
		if(temp != 0xf0)			
		{
			delay(10);
			P3 = 0xfd;
			temp = P3;
			temp &= 0xf0;
			if(temp != 0xf0)		//ȷ���м�����
			{
				flag = 0;					//�����´ε�ɨ��
				key_mode = 22;		//���Ϊ���󰴼�����
				switch(temp)
				{
					case 0xe0:key = S5_DOWN;break;
					case 0xd0:key = S6_DOWN;break;
					case 0xb0:key = S7_DOWN;break;
					case 0x70:key = S8_DOWN;break;
				}
			}
		}
		
		///////////////  ������   /////////////////
		P3 = 0xfb;						//ʶ��ڶ���,���ڶ��е������õ�
		temp = P3;
		temp &= 0xf0;					//��ͨ����4λ��ƽ״̬�����ж��ĸ�������
		if(temp != 0xf0)			
		{
			delay(10);
			P3 = 0xfb;
			temp = P3;
			temp &= 0xf0;
			if(temp != 0xf0)		//ȷ���м�����
			{
				flag = 0;					//�����´ε�ɨ��
				key_mode = 33;		//���Ϊ���󰴼�����
				switch(temp)
				{
					case 0xe0:key = S9_DOWN;break;
					case 0xd0:key = S10_DOWN;break;
					case 0xb0:key = S11_DOWN;break;
					case 0x70:key = S12_DOWN;break;
				}
			}
		}
		
		///////////////  ������   /////////////////
		P3 = 0xf7;						//ʶ��ڶ���,���ڶ��е������õ�
		temp = P3;
		temp &= 0xf0;					//��ͨ����4λ��ƽ״̬�����ж��ĸ�������
		if(temp != 0xf0)			
		{
			delay(10);
			P3 = 0xf7;
			temp = P3;
			temp &= 0xf0;
			if(temp != 0xf0)		//ȷ���м�����
			{
				flag = 0;					//�����´ε�ɨ��
				key_mode = 44;		//���Ϊ���󰴼�����
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
	else if(flag == 0)		 //˵���Ѿ��м�������
	{
		if(key_mode == 0x11)	//�жϵ�һ���Ƿ�ȫ������
		{
			P3 = 0xfe;
			temp = P3;
			temp &= 0xf0;
			if(temp == 0xf0)		//��һ����ȫ������
			{
				key_mode = 0;			//key_mode���Ϊ�޼�����
				flag = 1;					//�������
				key = KEYS_UP;				
				
				cnt = 0;
			}
		}
		else if(key_mode == 0x22)	//�жϵڶ����Ƿ�ȫ������
		{
			P3 = 0xfd;
			temp = P3;
			temp &= 0xf0;
			if(temp == 0xf0)				//�ڶ�����ȫ������
			{
				key_mode = 0;					//key_mode���Ϊ�޼�����
				flag = 1;							//�������
				key = KEYS_UP;				
				
				cnt = 0;
			}
		}
		else if(key_mode == 0x33)	//�жϵ������Ƿ�ȫ������
		{
			P3 = 0xfb;
			temp = P3;
			temp &= 0xf0;
			if(temp == 0xf0)				//��������ȫ������
			{
				key_mode = 0;					//key_mode���Ϊ�޼�����
				flag = 1;							//�������
				key = KEYS_UP;				
				cnt = 0;
			}
		}
		else if(key_mode == 0x44)	//�жϵ������Ƿ�ȫ������
		{
			P3 = 0xf7;
			temp = P3;
			temp &= 0xf0;
			if(temp == 0xf0)				//��������ȫ������
			{
				key_mode = 0;					//key_mode���Ϊ�޼�����
				flag = 1;							//�������
				key = KEYS_UP;				
				
				cnt = 0;
			}
		}
	}
	
	return key;
}

