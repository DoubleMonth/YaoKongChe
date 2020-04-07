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
#include "protocol.h"
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
int g_readSpeed = 0;

/**
* Function       app_ps2_deal
* @author        liusen
* @date          2017.08.23    
* @brief         PS2Э�鴦��
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   ��
*/
//void app_ps2_deal(void)
//{
//	rt_uint8_t PS2_KEY = 0, X1=0,Y1=0,X2=0,Y2=0;

//	
//	__set_PRIMASK(1);  //���жϣ�  
//	 
//	PS2_KEY = PS2_DataKey();	 //�ֱ�����������
//	   
//	__set_PRIMASK(0);//���ж�

//	switch(PS2_KEY)
//	{
//		case PSB_SELECT: 	rt_kprintf("PSB_SELECT \r\n");  break;
//		case PSB_L3:     	g_CarState = enSTOP;  rt_kprintf("PSB_L3 \r\n");  break;  
//		case PSB_R3:     	g_CarState = enSTOP;	 rt_kprintf("PSB_R3 \r\n");  break;  
//		case PSB_START:  	rt_kprintf("PSB_START \r\n");  break;  
//		case PSB_PAD_UP: 	g_CarState = enRUN;   rt_kprintf("PSB_PAD_UP11 \r\n");  break;  
//		case PSB_PAD_RIGHT:	g_CarState = enRIGHT; rt_kprintf("PSB_PAD_RIGHT11 \r\n");  break;
//		case PSB_PAD_DOWN:	g_CarState = enBACK;  rt_kprintf("PSB_PAD_DOWN11 \r\n");  break; 
//		case PSB_PAD_LEFT:	g_CarState = enLEFT;  rt_kprintf("PSB_PAD_LEFT11 \r\n");  break; 
//		case PSB_L2:      	//����
//		{
//			CarSpeedControl += 600;
//			if (CarSpeedControl > 7200)
//			{
//				CarSpeedControl = 7200;
//			}
//			 
//			rt_kprintf("PSB_L2 \r\n");  	

//		}break; 
//		case PSB_R2:      	//����
//		{
//			CarSpeedControl -= 600;
//			if (CarSpeedControl < 3600)
//			{
//				CarSpeedControl = 3600;
//			} 
//			rt_kprintf("PSB_R2 \r\n");

//		}  break; 
//		case PSB_L1:      	rt_kprintf("PSB_L1 \r\n");  break; 
//		case PSB_R1:      	rt_kprintf("PSB_R1 \r\n");  break;     
//		case PSB_TRIANGLE:	rt_kprintf("PSB_TRIANGLE \r\n");  break; 							//���
//		case PSB_CIRCLE:  	g_CarState = enTLEFT; rt_kprintf("PSB_CIRCLE \r\n");  break;  	//����
//		case PSB_CROSS:   	 rt_kprintf("PSB_CROSS \r\n");  break; 					//����
//		case PSB_SQUARE:  	g_CarState = enTRIGHT;rt_kprintf("PSB_SQUARE \r\n");  break;  	//����
//		default: g_CarState = enSTOP; break; 
//	}


//	      

//	//��ȡģ��ֵ
//	if(PS2_KEY == PSB_L1 || PS2_KEY == PSB_R1)
//	{
//		X1 = PS2_AnologData(PSS_LX);
//		Y1 = PS2_AnologData(PSS_LY);
//		X2 = PS2_AnologData(PSS_RX);
//		Y2 = PS2_AnologData(PSS_RY);
//		/*��ҡ��*/
//	    if (Y1 < 5 && X1 > 80 && X1 < 180) //��
//	    {
//			rt_kprintf("X1=%d \n",X1);
//			rt_kprintf("Y1=%d \n",Y1);
//		//	g_CarState = enRUN;
//		  
//	    }
//	    else if (Y1 > 230 && X1 > 80 && X1 < 180) //��
//	    {
//	      rt_kprintf("X1=%d \n",X1);
//			rt_kprintf("Y1=%d \n",Y1);
//		//	g_CarState = enBACK;
//		  
//	    }
//	    else if (X1 < 5 && Y1 > 80 && Y1 < 180) //��
//	    {
//	      rt_kprintf("X1=%d \n",X1);
//			rt_kprintf("Y1=%d \n",Y1);
//		//	g_CarState = enLEFT;

//	    }
//	    else if (Y1 > 80 && Y1 < 180 && X1 > 230)//��
//	    {
//	      rt_kprintf("X1=%d \n",X1);
//			rt_kprintf("Y1=%d \n",Y1);
//		//	g_CarState = enRIGHT;

//	    }
//	    else if (Y1 <= 80 && X1 <= 80) //����
//	    {
//	      rt_kprintf("X1=%d \n",X1);
//			rt_kprintf("Y1=%d \n",Y1);
//		//	g_CarState = enUPLEFT;

//	    }
//	    else if (Y1 <= 80 && X1 >= 180) //����
//	    {
//	      rt_kprintf("X1=%d \n",X1);
//			rt_kprintf("Y1=%d \n",Y1);
//		//	g_CarState = enUPRIGHT;
//	
//	    }
//	    else if (X1 <= 80 && Y1 >= 180) // ����
//	    {
//	      rt_kprintf("X1=%d \n",X1);
//			rt_kprintf("Y1=%d \n",Y1);
//		//	g_CarState = enDOWNLEFT;
//	
//	    }
//	    else if (Y1 >= 180 && X1 >= 180) //����
//	    {
//	      rt_kprintf("X1=%d \n",X1);
//			rt_kprintf("Y1=%d \n",Y1);
//		//	g_CarState = enDOWNRIGHT;
//		  
//	    }
//	    else//ͣ
//	    {
//	      g_CarState = enSTOP;
//	    }

//		 /*��ҡ��*/
//	    if (X2 < 5 && Y2 > 110 && Y2 < 150) //��
//	    {
//	      rt_kprintf("X2=%d \n",X2);
//			rt_kprintf("Y2=%d \n",Y2);
//	    }
//	    else if (Y2 > 110 && Y2 < 150 && X2 > 230)//��
//	    {
//	      rt_kprintf("X2=%d \n",X2);
//			rt_kprintf("Y2=%d \n",Y2);
//	    }
//	    else//��λ
//	    {
//	      rt_kprintf("X2=Y2=90\n");
//	    }
//	}

//	
//}

int main(void)
{
    int count = 1;
	rt_uint8_t MODE;
	rt_uint8_t key;
	PS2_Init();			 //�����˿ڳ�ʼ��
	PS2_SetInit();		 //�����ó�ʼ��,���á����̵�ģʽ������ѡ���Ƿ�����޸�
	                     //������ģʽ
	MODE=PS2_RedLight();
	rt_kprintf("Data[0]=%d\n",Data[0]);
	
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

//	while(1)
//	{
//		PS2_RedLight();
//		PS2_Cmd(0x01);  //��ʼ����
//		PS2_Cmd(0x42);  //��ʼ����
//		rt_thread_mdelay(500);
//	}
	while(1)
	{

		key=PS2_DataKey();
		if(key!=0)		
		{
			rt_kprintf("key=%d\n",key);
//			rt_thread_mdelay(200);
		}
		
		switch(key)
		{
			case PSB_SELECT: 	rt_kprintf("PSB_SELECT \n");  break;
			case PSB_L3:     	rt_kprintf("PSB_L3 \n");  break;  
			case PSB_R3:     	rt_kprintf("PSB_R3 \n");  break;  
			case PSB_START:  	rt_kprintf("PSB_START \n");  break;  
			case PSB_PAD_UP: 	rt_kprintf("PSB_PAD_UP \n");  break;  
			case PSB_PAD_RIGHT:	rt_kprintf("PSB_PAD_RIGHT \n");  break;
			case PSB_PAD_DOWN:	rt_kprintf("PSB_PAD_DOWN \n");  break; 
			case PSB_PAD_LEFT:	rt_kprintf("PSB_PAD_LEFT \n");  break; 
			case PSB_L2:      	rt_kprintf("PSB_L2 \n");  break; 
			case PSB_R2:      	rt_kprintf("PSB_R2 \n");  break; 
			case PSB_L1:           	
			{
				rt_kprintf("PSB_L1 \n");  
				PS2_Vibration(0xFF,0x00);  //�����𶯺��������ʱ  delay_ms(1000);
				rt_thread_mdelay(1000);
//				PS2_Vibration(0x00,0x00); 
			}break; 
			case PSB_R1:      	
			{
				rt_kprintf("PSB_R1 \n"); 
				PS2_Vibration(0x00,0xFF);  //�����𶯺��������ʱ  delay_ms(1000);
				rt_thread_mdelay(1000);
//				PS2_Vibration(0x00,0x00); 
			} break;     
			case PSB_TRIANGLE:	rt_kprintf("PSB_TRIANGLE \n");  break; 
			case PSB_CIRCLE:  	rt_kprintf("PSB_CIRCLE \n");  break; 
			case PSB_CROSS:   	rt_kprintf("PSB_CROSS \n");  break; 
			case PSB_SQUARE:  	rt_kprintf("PSB_SQUARE \n");  break;
		}
		rt_kprintf(" %5d %5d %5d %5d\r\n",PS2_AnologData(PSS_LX),PS2_AnologData(PSS_LY),
		                              PS2_AnologData(PSS_RX),PS2_AnologData(PSS_RY) );
		rt_thread_mdelay(300);
	}
 
	
    return RT_EOK;
}
