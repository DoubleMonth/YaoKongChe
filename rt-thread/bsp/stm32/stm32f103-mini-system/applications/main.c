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
#include "control.h"
/* defined the LED0 pin: PC13 */





int main(void)
{
    int count = 1;
	rt_uint8_t key;
	PS2_Init();			 //驱动端口初始化
	PS2_SetInit();		 //配配置初始化,配置“红绿灯模式”，并选择是否可以修改
	                     //开启震动模式
	Pin_Init();
	rt_pin_write(LED0_PIN,PIN_HIGH);
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
