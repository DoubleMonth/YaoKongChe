/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         protcol.c
* @author       liusen
* @version      V1.0
* @date         2017.08.18
* @brief        协议解析总入口
* @details      
* @par History  见如下说明
*                 
* version:	liusen_20170818
*/

#include "ps2_protocol.h"
#include "bsp_ps2.h"

#define THREAD_PRIORITY         25      /* 线程优先级 */
#define THREAD_STACK_SIZE       512     /* 线程栈大小 */
#define THREAD_TIMESLICE        5       /* 线程时间片大小 */

static rt_thread_t ps2_tid = RT_NULL;      /* 线程句柄 */


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
				PS2_Vibration(0xFF,0x00);  //发出震动后必须有延时  delay_ms(1000);
				rt_thread_mdelay(1000);
			}break; 
			case PSB_R1:      	
			{
				rt_kprintf("PSB_R1 \n"); 
				PS2_Vibration(0x00,0xFF);  //发出震动后必须有延时  delay_ms(1000);
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
    /* 创建线程，名称是 pwm_thread ，入口是 pwm_entry*/
    ps2_tid = rt_thread_create("ps2_handle",
                             ps2Handle,
                             RT_NULL,
                             THREAD_STACK_SIZE,
                             THREAD_PRIORITY,
                             THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (ps2_tid != RT_NULL)
        rt_thread_startup(ps2_tid);

    return RT_EOK;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(ps2_sample, ps2 sample);



	



