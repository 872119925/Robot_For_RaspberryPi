#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

#define  pwm1 TIM4->CCR1
#define  pwm2 TIM4->CCR2
#define  pwm3 TIM4->CCR3

#define eachFrequencyTime 50		//���������λʱ��(Ԥ��50ms �ɸ���)

extern void upData(void);//������nowPwm���ݸ������
extern void countAddPwm(short temp_pwm[3], short frequency);//����ÿ��pwm������Ҫ��������(���ڼ��ٿ���)
extern void changePwm(const short pwmValue[]);
extern void doAction(const short *actionArr,short length);

extern short targetPwm[3];//����Ŀ��PWM
extern short nowPwm[3];//���ڵ�Pwm
extern short addPwm[3];//ÿ��������Pwm��

#endif

