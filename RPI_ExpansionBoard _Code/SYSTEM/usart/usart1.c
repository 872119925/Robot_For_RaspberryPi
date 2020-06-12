#include "sys.h"
#include "usart1.h"
//////////////////////////////////////////////////////////////////////////////////
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

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
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
    int handle;
};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
_sys_exit(int x)
{
    x = x;
}
//�ض���fputc����
int fputc(int ch, FILE *f)
{
    while((USART1->SR&0X40)==0);//ѭ������,ֱ���������
    USART1->DR = (u8) ch;
    return ch;
}
#endif

/*ʹ��microLib�ķ���*/
/*
int fputc(int ch, FILE *f)
{
USART_SendData(USART1, (uint8_t) ch);

while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}

   return ch;
}
int GetKey (void)  {

   while (!(USART1->SR & USART_FLAG_RXNE));

   return ((int)(USART1->DR & 0x1FF));
}
*/

#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u8 USART1_RX_BUF1[USART1_REC_LEN1];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART1_RX_STA1=0;       //����״̬���
char USART1_RcvFinish1;//����һ֡��ɱ�־λ

u8 USART1_RX_BUF2[USART1_REC_LEN2];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART1_RX_STA2=0;       //����״̬���
char USART1_RcvFinish2;//����һ֡��ɱ�־λ

void USART1_Init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��

    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

    //USART1_RX	  GPIOA.10��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1

}

u8 firstdata = 0;//��һ�ؽ��յ���ֵ
void USART1_IRQHandler(void)                	//����1�жϷ������
{
    u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
    OSIntEnter();
#endif
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        Res = USART_ReceiveData(USART1);	//��ȡ���յ�������
        if( USART1_RX_STA1==0 && Res == 'D' && USART1_RX_STA2==0)
        {
            firstdata = Res;
            USART1_RX_BUF1[0] = Res;
        }
        else if( USART1_RX_STA2==0 && USART1_RX_STA1==0)
        {
            firstdata = Res;
            USART1_RX_BUF2[0] = Res;
        }
				
        if( firstdata == 'D' )
        {
							if(Res == '\n')
							{
									if( USART1_RX_STA1>1 && USART1_RX_BUF1[USART1_RX_STA1-1] == '\r' )
									{
											USART1_RX_BUF1[ USART1_RX_STA1 ] = Res;
											USART1_RX_STA1 = 0;//���¿�ʼ����
											USART1_RcvFinish1 = 1;//����һ֡��ɱ�־λ
									}
									else
									{
											USART1_RX_BUF1[USART1_RX_STA1++] = Res ;
									}
							}
							else
									USART1_RX_BUF1[ USART1_RX_STA1++ ] = Res ;
							if(USART1_RX_STA1 > (USART1_REC_LEN1-1))
									USART1_RX_STA1 = 0;//�������ݴ���,���¿�ʼ����
        }
        else
        {
							if(Res == '\n')
							{
									if( USART1_RX_STA2>1 && USART1_RX_BUF2[USART1_RX_STA2-1] == '\r' )
									{
											USART1_RX_BUF2[ USART1_RX_STA2 ] = Res ;
											USART1_RX_STA2 = 0;//���¿�ʼ����
											USART1_RcvFinish2 = 1;//����һ֡��ɱ�־λ
									}
									else
									{
											USART1_RX_BUF2[USART1_RX_STA2++]=Res ;
									}
							}
							else
									USART1_RX_BUF2[ USART1_RX_STA2++ ] = Res ;
							if(USART1_RX_STA2 > (USART1_REC_LEN2-1))
									USART1_RX_STA2 = 0;//�������ݴ���,���¿�ʼ����
        }
    }
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
    OSIntExit();
#endif
}
#endif

//����һ���ֽ�
void USART1_PutChar(uint8_t Data)
{
    USART_SendData(USART1, Data);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

//�����ַ���
void USART1_PutStr (uint8_t *str)
{
    while ( *str )
    {
        USART1_PutChar(*str);
        str++;
    }
}

void USART2_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    //USART1_TX   GPIOA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.2

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������
    USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
    USART_Init(USART2, &USART_InitStructure); //��ʼ������2
    USART2->CR2 &=(0<<14);
    USART2->CR2 &=(0<<11);
    USART2->CR3 &=(0<<1);
    USART2->CR3 &=(0<<5);
    USART2->CR3 |=(1<<3);
//    USART_HalfDuplexCmd(USART2,ENABLE);     //��˫��ʹ��

    USART_Cmd(USART2, ENABLE);      //�򿪴���2


}

//����һ���ֽ�
void USART2_PutChar(uint8_t Data)
{
//    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
////    USART2->CR1 &= ~(0<<2);
//    USART_SendData(USART2, Data);
//    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

    USART_SendData(USART2, Data);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}

//�����ַ���
void USART2_PutStr (uint8_t *str)
{
    while ( *str )
    {
        USART2_PutChar(*str);
        str++;
    }
}
void USART2_IRQHandler(void)//����2�жϷ������
{
    u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
    OSIntEnter();
#endif
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        Res = USART_ReceiveData(USART2);	//��ȡ���յ�������
        USART2_PutChar(Res);
    }
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
    OSIntExit();
#endif
}
//void USART2_IRQHandler(void)                	//����2�жϷ������
//{
//    u8 Res;
//#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//    OSIntEnter();
//#endif
//    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//    USART_ClearFlag(USART2, USART_FLAG_RXNE);
//    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//    {
//        Res = USART_ReceiveData(USART2);	//��ȡ���յ�������
//				USART2_PutChar(Res);
//    }
//#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//    OSIntExit();
//#endif
//}
