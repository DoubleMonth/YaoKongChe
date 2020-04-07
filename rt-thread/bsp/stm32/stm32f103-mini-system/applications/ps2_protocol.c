/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         protcol.c
* @author       liusen
* @version      V1.0
* @date         2017.08.18
* @brief        Э����������
* @details      
* @par History  ������˵��
*                 
* version:	liusen_20170818
*/

#include "ps2_protocol.h"
#include "bsp_ps2.h"

#define THREAD_PRIORITY         25      /* �߳����ȼ� */
#define THREAD_STACK_SIZE       512     /* �߳�ջ��С */
#define THREAD_TIMESLICE        5       /* �߳�ʱ��Ƭ��С */

static rt_thread_t ps2_tid = RT_NULL;      /* �߳̾�� */


rt_uint8_t MODE;
rt_uint8_t key;
void readPS2Mode(void)
{
	MODE=PS2_RedLight();
	if(1==MODE)
	{
		rt_kprintf("Green Mode\n");
	}
	else
	{
		rt_kprintf("Red Mode\n");
	}
}
static void ps2Handle(void *parameter)
{
	while(1)
	{

		key=PS2_DataKey();
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
			}break; 
			case PSB_R1:      	
			{
				rt_kprintf("PSB_R1 \n"); 
				PS2_Vibration(0x00,0xFF);  //�����𶯺��������ʱ  delay_ms(1000);
				rt_thread_mdelay(1000);
			} break;     
			case PSB_TRIANGLE:	rt_kprintf("PSB_TRIANGLE \n");  break; 
			case PSB_CIRCLE:  	rt_kprintf("PSB_CIRCLE \n");  break; 
			case PSB_CROSS:   	rt_kprintf("PSB_CROSS \n");  break; 
			case PSB_SQUARE:  	rt_kprintf("PSB_SQUARE \n");  break;
		}
		rt_kprintf(" %5d %5d %5d %5d\r\n",PS2_AnologData(PSS_LX),PS2_AnologData(PSS_LY),
		                              PS2_AnologData(PSS_RX),PS2_AnologData(PSS_RY) );
		rt_thread_mdelay(100);
	}
}

static int ps2_sample(int argc, char *argv[])
{    
    /* �����̣߳������� pwm_thread ������� pwm_entry*/
    ps2_tid = rt_thread_create("ps2_handle",
                             ps2Handle,
                             RT_NULL,
                             THREAD_STACK_SIZE,
                             THREAD_PRIORITY,
                             THREAD_TIMESLICE);

    /* �������߳̿��ƿ飬��������߳� */
    if (ps2_tid != RT_NULL)
        rt_thread_startup(ps2_tid);

    return RT_EOK;
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(ps2_sample, ps2 sample);



	



