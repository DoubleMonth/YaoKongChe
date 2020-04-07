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

#define PWM_DEV_NAME            "pwm1"  /* PWM�豸���� */
#define PWM_DEV_CHANNEL         4       /* PWMͨ�� */
#define THREAD_PRIORITY         25      /* �߳����ȼ� */
#define THREAD_STACK_SIZE       512     /* �߳�ջ��С */
#define THREAD_TIMESLICE        5       /* �߳�ʱ��Ƭ��С */

static rt_thread_t tid1 = RT_NULL;      /* �߳̾�� */
struct rt_device_pwm *pwm_dev;          /* PWM�豸��� */
static rt_uint32_t period = 500000;     /* ����Ϊ0.5ms����λΪ����ns */
static rt_uint32_t pulse = 0;           /* PWM������ֵ���������� */
static rt_uint16_t dir=1;
/* �߳� pwm_entry ����ں��� */
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
//        /* step 2������ PWM ���ں������ȣ�����ض��Ĳ��� */
//        rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse++);
//		rt_kprintf("pulse=%d\n",pulse);
//    }
//	rt_thread_mdelay(10);
    /* step 3���������ʹ�ø�ͨ�������Թر� PWM ͨ������� */
//    rt_pwm_disable(pwm_dev, PWM_DEV_CHANNEL);
}

static int pwm_test(int argc, char *argv[])
{
    /* step 1.1������ PWM �豸 */
    pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
    if (pwm_dev == RT_NULL)
    {
        rt_kprintf("pwm sample run failed! can't find %s device!\n", PWM_DEV_NAME);
        return RT_ERROR;
    }

    /* step 1.2������ PWM ���ں�������Ĭ��ֵ */
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    /* step 1.3��ʹ�� PWM �豸�����ͨ�� */
    rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL);

    /* �����̣߳������� pwm_thread ������� pwm_entry*/
    tid1 = rt_thread_create("pwm_thread",
                             pwm_entry,
                             RT_NULL,
                             THREAD_STACK_SIZE,
                             THREAD_PRIORITY,
                             THREAD_TIMESLICE);

    /* �������߳̿��ƿ飬��������߳� */
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    return RT_EOK;
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(pwm_test, pwm sample);


int main(void)
{
    int count = 1;
	rt_uint8_t key;
	PS2_Init();			 //�����˿ڳ�ʼ��
	PS2_SetInit();		 //�����ó�ʼ��,���á����̵�ģʽ������ѡ���Ƿ�����޸�
	                     //������ģʽ
	
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
