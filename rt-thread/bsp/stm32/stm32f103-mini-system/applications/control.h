#ifndef __CONTROL_H__
#define __CONTROL_H__
#include <rthw.h>
#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>
#include <board.h>

/***    Input GPIO     ***/
#define SUDU_PIN 		GET_PIN(A, 1)
#define JIN_PIN		GET_PIN(B,	6)
#define TUI_PIN		GET_PIN(B,	7)
#define YAOKONG_PIN 	GET_PIN(A, 4)
#define JIAOTA_PIN 		GET_PIN(A, 5)
#define SWIITCH_PIN 		GET_PIN(B, 4)



/***    Output GPIO     ***/
#define LEFT_MOTOT_PIN_1 GET_PIN(A, 6)
#define LEFT_MOTOT_PIN_2 GET_PIN(A, 7)

#define RIGHT_MOTOT_PIN_1 GET_PIN(B, 13)
#define RIGHT_MOTOT_PIN_2 GET_PIN(B, 15)

#define ZHUANXIANG_MOTOT_PIN_1 GET_PIN(A, 2)
#define ZHUANXIANG_MOTOT_PIN_2 GET_PIN(A, 3)

#define LED0_PIN    GET_PIN(A, 8)



struct inputState_Type
{
	rt_uint8_t sudu_state;     
	rt_uint8_t jin_state;
	rt_uint8_t tui_state;
	rt_uint8_t yaokong_state;
	rt_uint8_t jiaota_state;
	rt_uint8_t switch_state;
};
void Pin_Init(void);
int control_sample(void);
int pwm_test(void);
#endif