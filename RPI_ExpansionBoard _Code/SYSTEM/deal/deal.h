#ifndef __DEAL_H
#define __DEAL_H
#include "sys.h"


extern void rcvDataCvtToPwmVal(u8 *dat, char *rcvFinish);	//��ָ������֡ת����PWMֵ��ִ��
//����3�Ľ�����ת���Ŀ��Ƴ���
extern void checkDeal_UT(u8 Usart1Data[], char *usart1Finish, u8 Usart3Data[], char *Usart3Finish, u16 Uart3len );
extern char checkAndDealActionDebug(u8 dat[], char *rcvFinish );	//����������
extern char checkADCReq(u8 dat[], char *rcvFinish);	//ADC����������
extern u16 adcValue;


#endif

