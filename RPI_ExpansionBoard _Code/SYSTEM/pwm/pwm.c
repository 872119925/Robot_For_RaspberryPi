#include "pwm.h"
#include "usart1.h"
#include "delay.h"


//�ϵ��������״̬(��ʼֵ����һ��վ��״̬)
short nowPwm[3] = {1500,1500,1500};//��ǰPwm
short targetPwm[3] = {1500,1500,1500};//Ŀ��Pwm
//ÿ�ζ�ʱ�ж�ʱҪ���ӵĶ����
short addPwm[3]= {0,0,0};


/************************************************************************
�������ƣ�	upData(void)
���������� ������nowPwm���ݸ������
��ڲ����� none
�� �� ֵ�� none
����˵���� �ú�����nowPwm����ֱ�����������õ�IO�˿�
**************************************************************************/
void upData(void )
{
    pwm1 = nowPwm[0];
    pwm2 = nowPwm[1];
    pwm3 = nowPwm[2];
}


/************************************************************************
�������ƣ�	countAddPwm(short temp_pwm[3], short frequency)
���������� ����ÿ��pwm������Ҫ��������(���ڼ��ٿ���)
��ڲ����� temp_pwm frequency:��������
�� �� ֵ�� none
����˵���� �ú�������pwm�Լ����ٱ���(��λ����ʱ��Ԥ��ֵΪ50ms),��ʱ��6����ʱ
					���Զ�������Ч
**************************************************************************/
void countAddPwm(short temp_pwm[3], short frequency)
{
    u8 i;
    for( i = 0 ; i<3 ; i++ )
    {
        if( temp_pwm[i]>0 )
        {
            targetPwm[i] = temp_pwm[i];
            addPwm[i] = (targetPwm[i]-nowPwm[i])/frequency;
        }
    }
}

/************************************************************************
�������ƣ�	doAction(const short *actionArr,short length)
���������� ִ�ж�������
��ڲ����� ָ���͵�actionArr length
�� �� ֵ�� none
����˵���� �ú���������Ķ�������ָ��actionArr���Լ�actionArr�ĳ���length(һ��
						Ҫ��4�ı��� ÿ���4λΪִ�е�ʱ����)����Ȥ�Ŀɸ�����Ҫ�����޸ġ�
						����Ŀ�д˺����Ѿ�û��ʵ����������ʱû����
**************************************************************************/
void doAction(const short *actionArr,short length)
{
    short i = 0;
    char j = 0;
    short temp_pwm[3];//��ʱ����
    for( i = 0 ; i<length ; i+=4 )	//����������;�жϹ���
//    for( i = 0 ; i<length && USART_RX_BUF[0] ; i+=4 )	//��Ҫ���յ�ֹͣ�����ʱ�򣬵�ǰ�����ſ��Ա����
    {
        for( j = 0 ; j<3 ; j++ )
        {
            temp_pwm[j] = actionArr[i+j];
        }
        countAddPwm(temp_pwm, actionArr[i+3]/eachFrequencyTime);//50msΪ��ʱ������pwm������
        delay_ms(actionArr[i+3]);//ʱ���ӻ� ������ٿ��Ƶ�һ����

        //������´��� ��������������ͬʱ���������Ի�
        /*
        if( usart3ReceiveSuccess2 )//��������������������յ����ָ��֡
        {
        if( USART_RX_BUF2[0] == 'Y' )//��������֡ͷ��־
        if( USART_RX_BUF2[1] == 'S' )
            YSOrder( USART_RX_BUF2[2] );//������������
        else if( USART_RX_BUF2[1] == 'T' ) //����ת��
            XFS_FrameInfo( &USART_RX_BUF2[2] );
        usart3ReceiveSuccess2 = 0;//��־λ����
        }
        */
    }
}
