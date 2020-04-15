#ifndef __CONTROL_H__
#define __CONTROL_H__
#include <rthw.h>
#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>
#include <board.h>

/***    Input GPIO     ***/
#define SUDU_PIN 				GET_PIN(A, 	1)
#define JIN_PIN					GET_PIN(B,	6)
#define TUI_PIN					GET_PIN(B,	7)
#define JIAOTA_PIN 				GET_PIN(A, 	5)

#define SWIITCH_PIN 			GET_PIN(B, 4)
#define YAOKONG_PIN 			GET_PIN(A, 4)


/***    Output GPIO     ***/
#define LEFT_MOTOT_PIN_1 		GET_PIN(A, 6)
#define LEFT_MOTOT_PIN_2 		GET_PIN(A, 7)

#define RIGHT_MOTOT_PIN_1 		GET_PIN(B, 13)
#define RIGHT_MOTOT_PIN_2 		GET_PIN(B, 15)

#define ZHUANXIANG_MOTOT_PIN_1 GET_PIN(A, 2)
#define ZHUANXIANG_MOTOT_PIN_2 GET_PIN(A, 3)

#define LED0_PIN    			GET_PIN(A, 8)



struct inputState_Type
{
	rt_uint8_t sudu_state;     //�ߵ��ٿ��أ���������1 ���١�2 ����
	rt_uint8_t jin_state;	//ǰ����������Ͽ��أ��������� jin_state=1,tui_state=0;ǰ����jin_state=0,tui_state=1;���ˣ�jin_state=1,tui_state=1;ֹͣ��
	rt_uint8_t tui_state;	//ǰ����������Ͽ��أ��������� jin_state=1,tui_state=0;ǰ����jin_state=0,tui_state=1;���ˣ�jin_state=1,tui_state=1;ֹͣ��
	rt_uint8_t yaokong_state;	//ң�ء��ֶ����أ���������1 �ֶ���2 ң��  ��ʱ����
	rt_uint8_t jiaota_state;	//��̤���أ��Ը�λ�� 0 ̤�£�1 δ̤�¡�
	rt_uint8_t switch_state;	//�ܿ��أ���������1 �أ�0 ����   ��ʱ����
};
void Pin_Init(void);
int control_sample(void);
void tingZhi(void);
void zhuanXiangTing(void);
int pwm_test(void);
void youZhuan(void);
void zuoZhuan(void);
#endif
