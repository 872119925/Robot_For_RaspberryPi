#include "sys.h"
#include <stdlib.h>
#include "delay.h"
#include "led.h"
#include "oled.h"
#include "usart1.h"
#include "usart3.h"
#include "adc.h"
#include "timer.h"
#include "pwm.h"
#include "deal.h"
#include "actionArrFile.h"


short distance = 0;
short rate = 27;
char	pressEN = 0;
void Press(u16 distance, short rate, char *EN)
{
    u32 time = distance*rate + 1;
    if( *EN )
    {
        *EN = 0;
        doAction(pressArr, 4);
        while( time>1000 )
        {
            delay_ms( 1000 );	//ʱ���ӻ� ������ٿ��Ƶ�һ����
            time -= 1000;
        }
        delay_ms( time );	//ʱ���ӻ� ������ٿ��Ƶ�һ����
        doAction(&pressArr[0]+4, 4);
    }
}
void checkSet(u8 Usart1Data[], char *usart1Finish)
{
    short num;
    short i;
    if( *usart1Finish )//Usart1���յ����ָ���־λ
    {
        if( Usart1Data[0] == 'r' && Usart1Data[1] == '=' )//��������֡ͷ��־
        {
//            for( num=0, i = 2; Usart1Data[i]!='\r'; i++ )
//            {
//                num = num*10+Usart1Data[i]-48;
//            }
//            rate = num;
            rate = atoi(&Usart1Data[2]);
            USART1_PutStr("Set rate is OK!\r\n");
        }
        else
        {
//            for( num=0, i = 0; Usart1Data[i]!='\r'; i++ )
//                num = num*10+Usart1Data[i]-48;
//            distance = num;
            distance = atoi(Usart1Data);
            pressEN = 1;
        }
        *usart1Finish = 0;//��־λ����
    }
}

int main(void)
{

//		  short tmp[4] = {1000, 1000, 1000, 50};
    short tmp[4] = {1500, 1500, 1500, 50};
//    unsigned char data[10] = {0x55,0x55,0x01, 0x07,0x01,0xfe,0x01, 0xE8,0x03,0x0c};
    int i;
    SystemInit();
    delay_init();	    	 //��ʱ������ʼ��
    USART1_Init(115200);//��RPIͨ��
    USART2_Init(115200);//�봮�ڶ��ͨѶ
    USART3_Init(115200);//����Ӵ���ģ��ͨ��

//    OLED_Init();
    Adc_Init();		  			//ADC��ʼ��
    TIM4_PWM_Init(5000-1,72-1);	//����Ƶ��PWMƵ��=72000000/900=80Khz
    TIM3_Int_Init(eachFrequencyTime*10-1,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms
    doAction( tmp, 4);
    while(1)
    {
//        checkSet(USART1_RX_BUF, &USART1_RcvFinish);
//        Press( distance, rate, &pressEN);

//        for( i=0; i<sizeof(data); i++ );
//            USART3_PutChar(data[i]);
//        for( i=0; i<sizeof(data); i++ )
//            USART2_PutChar(data[i]);
//        delay_ms( 1000 );	//ʱ���ӻ� ������ٿ��Ƶ�һ����
        //ʹ���������doActionִ�ж���
        //doAction(actionArr, sizeof(actionArr)/2);
        checkAndDealActionDebug( USART1_RX_BUF1,  &USART1_RcvFinish1 );
        checkADCReq( USART1_RX_BUF2, &USART1_RcvFinish2 );
        checkDeal_UT( USART1_RX_BUF2, &USART1_RcvFinish2, USART3_RX_BUF, &USART3_RcvFinish, USART3_Len);
    }
}
