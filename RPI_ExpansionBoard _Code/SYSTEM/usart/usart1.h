#ifndef __USART1_H
#define __USART1_H
#include "stdio.h"	
#include "sys.h" 
#include "stm32f10x.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
#define USART1_REC_LEN1  			4096  	//�����������ֽ��� 8192
#define USART1_REC_LEN2  			4096  	//�����������ֽ��� 8192
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

extern u8 firstdata;//��һ�ؽ��յ���ֵ

// ��һ��������Ϣ
extern u8  USART1_RX_BUF1[USART1_REC_LEN1]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART1_RX_STA1;         		//����״̬���
extern char USART1_RcvFinish1;//����һ֡��ɱ�־λ

// �ڶ���������Ϣ
extern u8  USART1_RX_BUF2[USART1_REC_LEN2]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART1_RX_STA2;         		//����״̬���
extern char USART1_RcvFinish2;//����һ֡��ɱ�־λ

//����봮���жϽ��գ��벻Ҫע�����º궨��
extern void USART1_Init(u32 bound);			//����1��ʼ��
extern void USART1_PutChar(uint8_t Data);//����һ���ֽ�
extern void USART1_PutStr (uint8_t *str);//�����ַ���

extern void USART2_Init(u32 bound);
extern void USART2_PutChar(uint8_t Data);//����һ���ֽ�
extern void USART2_PutStr (uint8_t *str);//�����ַ���

#endif


