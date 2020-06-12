#include "usart3.h"
#include "usart1.h"

u8 USART3_RX_BUF[USART3_REC_LEN] = {0};//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
u16 USART3_RX_STA = 0;         		//����״̬���
char USART3_RcvFinish = 0;		//����һ֡��ɱ�־λ
u16 USART3_Len = 0;	//���յ������ݳ���

void USART3_Init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//GPIOB
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��UART3

    //UART3_TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //UART3_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //Uart3 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������
    USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART3, &USART_InitStructure); //��ʼ������3
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���3

}

void USART3_IRQHandler(void)//����2�жϷ������
{
    u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
    OSIntEnter();
#endif
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        Res = USART_ReceiveData(USART3);	//��ȡ���յ�������

				if(Res == '\n')
				{
						if( USART3_RX_STA>1 && USART3_RX_BUF[USART3_RX_STA-1] == '\r' )
						{
								USART3_RX_BUF[ USART3_RX_STA++ ] = Res;
								USART3_Len = USART3_RX_STA;
								USART3_RX_STA = 0;//���¿�ʼ����
								USART3_RcvFinish = 1;//����һ֡��ɱ�־λ
						}
						else
						{
								USART3_RX_BUF[USART3_RX_STA++] = Res ;
						}
				}
				else
				{
						USART3_RX_BUF[ USART3_RX_STA++ ] = Res ;
				}
				if(USART3_RX_STA > (USART3_REC_LEN-1))
						USART3_RX_STA = 0;//�������ݴ���,���¿�ʼ����
				
        /*
        if((USART3_RX_STA&0x8000)==0)//����δ���
        {
        if(USART3_RX_STA&0x4000)//���յ���0x0d
        {
        if(Res!=0x0a)
        				{
            USART3_RX_STA=0;//���մ���,���¿�ʼ
        						USART3_Len = 0;
        				}
        else
        {
            USART3_RX_STA|=0x8000;	//���������
            USART3_RcvFinish = 1;//����һ֡��ɱ�־λ
        						USART3_Len ++;
        }
        }
        else //��û�յ�0X0D
        {
        if(Res==0x0d)
        {
            USART3_RX_BUF[USART3_RX_STA] = Res;
            USART3_RX_STA|=0x4000;
        }
        else
        {
            USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
            USART3_RX_STA++;
        						USART3_Len ++;
            if(USART3_RX_STA>(USART3_REC_LEN-1))
                USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����
        }
        }
        }
        */
    }
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
    OSIntExit();
#endif
}

//����һ���ֽ�
void USART3_PutChar(uint8_t Data)
{
    USART_SendData(USART3, Data);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}

//�����ַ���
void USART3_PutStr(uint8_t *str)
{
    while (0 != *str)
    {
        USART3_PutChar(*str);
        str++;
    }
}

