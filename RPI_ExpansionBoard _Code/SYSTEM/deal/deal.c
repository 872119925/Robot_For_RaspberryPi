#include "sys.h"
#include "deal.h"
#include <stdlib.h>
#include "delay.h"
#include "usart1.h"
#include "usart3.h"
#include "adc.h"
#include "pwm.h"
#include "actionArrFile.h"

void rcvDataCvtToPwmVal(u8 *dat, char *rcvFinish);	//��ָ������֡ת����PWMֵ��ִ��
//����3�Ľ�����ת���Ŀ��Ƴ���
void checkDeal_UT(u8 Usart1Data[], char *usart1Finish, u8 Usart3Data[], char *Usart3Finish, u16 Uart3len );
char checkAndDealActionDebug(u8 dat[], char *rcvFinish );	//����������
char checkADCReq(u8 dat[], char *rcvFinish);	//ADC����������
u16 adcValue = 0;


/************************************************************************
�������ƣ�rcvDataCvtToPwmVal()
���������� �������ģʽ ���յ�����Ϣ֡ת��ΪPwm�źŲ�������Ч ��һ��ͬʱ����ִ��
					Լ400������
��ڲ����� dat: ��ʾҪ����������֡		rcvFinish: �����жϱ�־λ
�� �� ֵ�� none
����˵����
**************************************************************************/
void rcvDataCvtToPwmVal(u8 *dat, char *rcvFinish)
{
    short len = 1;//��ʶ��������ʱ��bufferָ��
    short pwm[3];//��ŵ�ǰ�������pwmֵ
    short actionNum = 0;//��Ŷ�����ĸ���ֵ
    short executionTime;//��ŵ�ǰ������Ҫִ�е�ʱ��
    char flag;
    char i;

    //������֡ͷ������(������ĸ���)
    while( dat[len] == ' ' ) len++;
    while( dat[len]<58 && dat[len]>47 )
    {
        actionNum = actionNum*10+dat[len++]-48;
    }

    *rcvFinish = 0;//��־λ����

    //����actionNum������
    while( actionNum-- && !(*rcvFinish) )//��־λ���±���һ�Ļ�˵�����µ�ָ�� ��ֹͣ��ǰ����
    {
        executionTime = 0;
        flag = 0;
        for( i = 0 ; i<3 ; i++)
            pwm[i] = 0;

        //����L���������ֵ
        while( flag<3 )
        {
            while( dat[len++] != ' ' );
            while( dat[len]<58 && dat[len]>47 )
            {
                pwm[flag] = pwm[flag]*10+dat[len++]-48;
            }
            if(  dat[len-1]<58 && dat[len-1]>47  )
                flag++;
        }
        //�������鶯����Ҫִ�е�ʱ��
        while( dat[len] == ' ' )	len++;
        while( dat[len]<58 && dat[len]>47 )
        {
            executionTime = executionTime*10+dat[len++]-48;
        }

        countAddPwm(pwm, executionTime/eachFrequencyTime);//����pwm�Լ����ٱ���5,��ʱ��6����ʱ���Զ�������Ч
        delay_ms(executionTime);//�ȴ���������
    }
}

/************************************************************************
�������ƣ�	Uart3Cvt( void )
����������	����3�շ����Ƴ���
��ڲ�����	dat��������֡		rcvFinish:����֡������־λ
�� �� ֵ��	none
����˵����	��ͷ��YS��Ϊ��������ָ�� ��ͷ��YT��Ϊ����ת��ָ��
**************************************************************************/
void checkDeal_UT(u8 Usart1Data[], char *usart1Finish, u8 Usart3Data[], char *Usart3Finish, u16 Uart3len)
{
    u8 *dat;
    u16 pos;
	
    if( *usart1Finish )//Usart1���յ����ָ���־λ
    {
        if( Usart1Data[0] == 'U' && Usart1Data[2] == 'T' )//��������֡ͷ��־
        {
            if( Usart1Data[1] == '2' )
            {
                for( dat=&Usart1Data[3]; *dat!='\r' || (*(dat+1)!='\n'); dat++)
                    USART2_PutChar( *dat );
                *usart1Finish = 0;//��־λ����
            }
            else if( Usart1Data[1] == '3' )
            {
                for( dat=&Usart1Data[3]; *dat!='\r' || (*(dat+1)!='\n'); dat++)
                    USART3_PutChar( *dat );
                *usart1Finish = 0;//��־λ����
                USART3_PutStr( "\r\n" );
            }
        }
    }
    //	ת������	����3������ͨ������1ת����ȥ
    if( *Usart3Finish )//Usart1���յ����ָ���־λ
    {
        for( pos = 0; pos<Uart3len; pos++ )
            USART1_PutChar( Usart3Data[pos] );
        *Usart3Finish = 0;//��־λ����
    }
}

/************************************************************************
�������ƣ�	checkAndDealActionDebug( void )
���������� ��鲢������������ָ��
��ڲ����� dat��������֡		rcvFinish:����֡������־λ
�� �� ֵ�� �Ƿ�ΪDebugģʽ
����˵���� ��ͷ��D��ΪDebugģʽ ����Ϊ����ָ��
**************************************************************************/
char checkAndDealActionDebug(u8 dat[], char *rcvFinish )
{
    if( *rcvFinish )//�Ƿ��ѽ�������֡
    {
        if( dat[0] == 'D' )//�������ģʽ֡ͷ��־
        {
            rcvDataCvtToPwmVal(dat, rcvFinish);//�������ģʽ ��һ��ͬʱ����ִ��400������
            *rcvFinish = 0;//��־λ����
            return 1;
        }
        return 0;
    }
    else
        return 0;
}

/************************************************************************
�������ƣ�	checkADCReq(const u8 dat[])
����������	���ADC����
��ڲ�����	dat��������֡		rcvFinish:����֡������־λ
�� �� ֵ��	char ��ʾ�Ƿ���ADC����
����˵����	ADC����ָ�������֡Ϊ"ADC?"
**************************************************************************/
char checkADCReq( u8 dat[], char *rcvFinish)
{
    if( *rcvFinish )//�Ƿ��ѽ�������֡
    {
        //�������ģʽ֡ͷ��־
        if( dat[0] == 'A' && dat[1] == 'D' && dat[2] == 'C' && dat[3] == '?' )
        {
            adcValue = Get_Adc(ADC_Channel_1);
            USART1_PutStr ( "ADC=" );
            USART1_PutChar( adcValue>>8 );
            USART1_PutChar( adcValue%256 );
            *rcvFinish = 0;//��־λ����
            return 1;
        }
        return 0;
    }
    else
        return 0;
}