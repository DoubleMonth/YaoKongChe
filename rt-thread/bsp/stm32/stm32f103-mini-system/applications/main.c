/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-03-08     obito0   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "bsp_ps2.h"
#include "ps2_protocol.h"
#include "control.h"
/* defined the LED0 pin: PC13 */





int main(void)
{
	PS2_Init();			 //�����˿ڳ�ʼ��
	PS2_SetInit();		 //�����ó�ʼ��,���á����̵�ģʽ������ѡ���Ƿ�����޸�
	                     //������ģʽ
	Pin_Init();
	rt_pin_write(LED0_PIN,PIN_HIGH);
	pwm_test();
	rt_thread_mdelay(500);
	ps2_sample();
	control_sample();
	
    /* set LED0 pin mode to output */
//    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
//	pwm_test(0,RT_NULL);
//    while (count++)
//    {
//        rt_pin_write(LED0_PIN, PIN_HIGH);
//        rt_thread_mdelay(500);
//        rt_pin_write(LED0_PIN, PIN_LOW);
//        rt_thread_mdelay(500);
//    }
	rt_kprintf("Init Finshed!\n");

    return RT_EOK;
}
