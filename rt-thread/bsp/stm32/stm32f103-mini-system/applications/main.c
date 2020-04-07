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
/* defined the LED0 pin: PC13 */
#define LED0_PIN    GET_PIN(C, 13)

#define PWM_DEV_NAME            "pwm1"  /* PWM设备名称 */
#define PWM_DEV_CHANNEL         4       /* PWM通道 */
#define THREAD_PRIORITY         25      /* 线程优先级 */
#define THREAD_STACK_SIZE       512     /* 线程栈大小 */
#define THREAD_TIMESLICE        5       /* 线程时间片大小 */

static rt_thread_t tid1 = RT_NULL;      /* 线程句柄 */
struct rt_device_pwm *pwm_dev;          /* PWM设备句柄 */
static rt_uint32_t period = 500000;     /* 周期为0.5ms，单位为纳秒ns */
static rt_uint32_t pulse = 0;           /* PWM脉冲宽度值的增减方向 */
static rt_uint16_t dir=1;
/* 线程 pwm_entry 的入口函数 */
static void pwm_entry(void *parameter)
{
    rt_uint32_t count = 0;

    while(1)
	{
		rt_thread_mdelay(50);
		if(dir)
			pulse+=5000;
		else
			pulse-=5000;
		if(pulse>=period)
			dir=0;
		if(0==pulse)
			dir=1;
		rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
	}
//	while (count++ < 1000)
//    {
//        rt_thread_mdelay(50);
//        /* step 2、设置 PWM 周期和脉冲宽度，输出特定的波形 */
//        rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse++);
//		rt_kprintf("pulse=%d\n",pulse);
//    }
//	rt_thread_mdelay(10);
    /* step 3、如果不再使用该通道，可以关闭 PWM 通道的输出 */
//    rt_pwm_disable(pwm_dev, PWM_DEV_CHANNEL);
}

static int pwm_test(int argc, char *argv[])
{
    /* step 1.1、查找 PWM 设备 */
    pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
    if (pwm_dev == RT_NULL)
    {
        rt_kprintf("pwm sample run failed! can't find %s device!\n", PWM_DEV_NAME);
        return RT_ERROR;
    }

    /* step 1.2、设置 PWM 周期和脉冲宽度默认值 */
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    /* step 1.3、使能 PWM 设备的输出通道 */
    rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL);

    /* 创建线程，名称是 pwm_thread ，入口是 pwm_entry*/
    tid1 = rt_thread_create("pwm_thread",
                             pwm_entry,
                             RT_NULL,
                             THREAD_STACK_SIZE,
                             THREAD_PRIORITY,
                             THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    return RT_EOK;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(pwm_test, pwm sample);


int main(void)
{
    int count = 1;
	rt_uint8_t key;
	PS2_Init();			 //驱动端口初始化
	PS2_SetInit();		 //配配置初始化,配置“红绿灯模式”，并选择是否可以修改
	                     //开启震动模式
	
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
