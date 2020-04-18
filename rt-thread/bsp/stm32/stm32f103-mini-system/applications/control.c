#include "control.h"
#include "ps2_protocol.h"
#include "bsp_ps2.h"
//#define THREAD_PRIORITY         25      /* 线程优先级 */
//#define THREAD_STACK_SIZE       512     /* 线程栈大小 */
//#define THREAD_TIMESLICE        5       /* 线程时间片大小 */

#define PWM_DEV_NAME            "pwm1"  /* PWM设备名称 */
#define PWM_2_DEV_NAME            "pwm2"  /* PWM设备名称 */
#define PWM_3_DEV_NAME            "pwm3"  /* PWM设备名称 */
#define PWM_DEV_CHANNEL         4       /* PWM通道 */
#define PWM_2_DEV_CHANNEL         3       /* PWM通道 */
#define PWM_3_DEV_CHANNEL         4       /* PWM通道 */
#define THREAD_PRIORITY         25      /* 线程优先级 */
#define THREAD_STACK_SIZE       512     /* 线程栈大小 */
#define THREAD_TIMESLICE        5       /* 线程时间片大小 */

static rt_thread_t tid1 = RT_NULL;      /* 线程句柄 */
struct rt_device_pwm *pwm_dev;          /* PWM设备句柄 */
struct rt_device_pwm *pwm_dev_2;          /* PWM设备句柄 */
struct rt_device_pwm *pwm_dev_3;          /* PWM设备句柄 */
static rt_uint32_t period = 5000000;     /* 周期为0.5ms，单位为纳秒ns */
static rt_uint32_t pulse = 0;           /* PWM脉冲宽度值的增减方向 */

rt_uint8_t key;
rt_uint8_t youZhuan_flag=0;
rt_uint8_t zuoZhuan_flag=0;

/* 线程 pwm_entry 的入口函数 */
static void pwm_entry(void *parameter)
{
////    rt_uint32_t count = 0;

////    while(1)
////	{
////		rt_thread_mdelay(50);
////		if(dir)
////			pulse+=50000;
////		else
////			pulse-=50000;
////		if(pulse>=period)
////			dir=0;
////		if(0==pulse)
////			dir=1;
////		rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
////		rt_pwm_set(pwm_dev_2, PWM_2_DEV_CHANNEL, period, pulse);
////		rt_pwm_set(pwm_dev_3, PWM_3_DEV_CHANNEL, period, pulse);
////	}
//////	while (count++ < 1000)
//////    {
//////        rt_thread_mdelay(50);
//////        /* step 2、设置 PWM 周期和脉冲宽度，输出特定的波形 */
//////        rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse++);
//////		rt_kprintf("pulse=%d\n",pulse);
//////    }
////	rt_thread_mdelay(10);
////    /* step 3、如果不再使用该通道，可以关闭 PWM 通道的输出 */
////    rt_pwm_disable(pwm_dev, PWM_DEV_CHANNEL);
}

int pwm_test(void)
{
    /* step 1.1、查找 PWM 设备 */
    pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
	pwm_dev_2 = (struct rt_device_pwm *)rt_device_find(PWM_2_DEV_NAME);
	pwm_dev_3 = (struct rt_device_pwm *)rt_device_find(PWM_3_DEV_NAME);
    if (pwm_dev == RT_NULL)
    {
        rt_kprintf("pwm sample run failed! can't find %s device!\n", PWM_DEV_NAME);
        return RT_ERROR;
    }
	if (pwm_dev_2 == RT_NULL)
    {
        rt_kprintf("pwm_2 sample run failed! can't find %s device!\n", PWM_DEV_NAME);
        return RT_ERROR;
    }
	if (pwm_dev_3 == RT_NULL)
    {
        rt_kprintf("pwm_3 sample run failed! can't find %s device!\n", PWM_DEV_NAME);
        return RT_ERROR;
    }
	
    /* step 1.2、设置 PWM 周期和脉冲宽度默认值 */
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
	rt_pwm_set(pwm_dev_2, PWM_2_DEV_CHANNEL, period, pulse);
	rt_pwm_set(pwm_dev_3, PWM_3_DEV_CHANNEL, period, pulse);
    /* step 1.3、使能 PWM 设备的输出通道 */
    rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL);
	rt_pwm_enable(pwm_dev_2, PWM_2_DEV_CHANNEL);
	rt_pwm_enable(pwm_dev_3, PWM_3_DEV_CHANNEL);
	

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

void Pin_Init(void)
{

	/***    Init Input GPIO     ***/
	rt_pin_mode(SUDU_PIN,PIN_MODE_INPUT_PULLUP);
	rt_pin_mode(JIN_PIN,PIN_MODE_INPUT_PULLUP);
	rt_pin_mode(TUI_PIN,PIN_MODE_INPUT_PULLUP);
	rt_pin_mode(YAOKONG_PIN,PIN_MODE_INPUT_PULLUP);
	rt_pin_mode(JIAOTA_PIN,PIN_MODE_INPUT_PULLUP);
	rt_pin_mode(SWIITCH_PIN,PIN_MODE_INPUT_PULLUP);
		
	/***    Init Output GPIO     ***/
	rt_pin_mode(LEFT_MOTOT_PIN_1, PIN_MODE_OUTPUT);
	rt_pin_mode(LEFT_MOTOT_PIN_2, PIN_MODE_OUTPUT);
	rt_pin_mode(RIGHT_MOTOT_PIN_1, PIN_MODE_OUTPUT);
	rt_pin_mode(RIGHT_MOTOT_PIN_2, PIN_MODE_OUTPUT);
	rt_pin_mode(ZHUANXIANG_MOTOT_PIN_1, PIN_MODE_OUTPUT);
	rt_pin_mode(ZHUANXIANG_MOTOT_PIN_2, PIN_MODE_OUTPUT);
	
	rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);    //LED 
	zhuanXiangTing();
	tingZhi();
}

struct inputState_Type inputState; 
struct inputState_Type inputState_1; 
struct inputState_Type inputState_2; 
void DeInt_inputState(void)
{
	inputState.jiaota_state=1;
	inputState.jin_state=1;
	inputState.sudu_state=1;
	inputState.switch_state=1;
	inputState.tui_state=1;
	inputState.yaokong_state=1;
	
}
void pinRead(void)
{
	inputState_1.jiaota_state=rt_pin_read(JIAOTA_PIN);
	inputState_1.jin_state=rt_pin_read(JIN_PIN);
	inputState_1.sudu_state=rt_pin_read(SUDU_PIN);
	inputState_1.switch_state=rt_pin_read(SWIITCH_PIN);
	inputState_1.tui_state=rt_pin_read(TUI_PIN);
	inputState_1.yaokong_state=rt_pin_read(YAOKONG_PIN);
	rt_thread_mdelay(20);
	inputState_2.jiaota_state=rt_pin_read(JIAOTA_PIN);
	inputState_2.jin_state=rt_pin_read(JIN_PIN);
	inputState_2.sudu_state=rt_pin_read(SUDU_PIN);
	inputState_2.switch_state=rt_pin_read(SWIITCH_PIN);
	inputState_2.tui_state=rt_pin_read(TUI_PIN);
	inputState_2.yaokong_state=rt_pin_read(YAOKONG_PIN);
	if(inputState_1.jiaota_state==inputState_2.jiaota_state)
	{
		inputState.jiaota_state=inputState_2.jiaota_state;
	}
	if(inputState_1.jin_state==inputState_2.jin_state)
	{
		inputState.jin_state=inputState_2.jin_state;
	}
	if(inputState_1.sudu_state==inputState_2.sudu_state)
	{
		inputState.sudu_state=inputState_2.sudu_state;
	}
	if(inputState_1.switch_state==inputState_2.switch_state)
	{
		inputState.switch_state=inputState_2.switch_state;
	}
	if(inputState_1.tui_state==inputState_2.tui_state)
	{
		inputState.tui_state=inputState_2.tui_state;
	}
	if(inputState_1.yaokong_state==inputState_2.yaokong_state)
	{
		inputState.yaokong_state=inputState_2.yaokong_state;
	}
	
}
void Rigth_Qianjin(rt_uint32_t sudu)
{
	rt_pin_write(RIGHT_MOTOT_PIN_1,PIN_HIGH);
	rt_pin_write(RIGHT_MOTOT_PIN_2,PIN_LOW);
	rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, 5000000, sudu);
	rt_kprintf("sudu=%d",sudu);
}
void Rigth_HouTui(rt_uint32_t sudu)
{
	rt_pin_write(RIGHT_MOTOT_PIN_1,PIN_LOW);
	rt_pin_write(RIGHT_MOTOT_PIN_2,PIN_HIGH);
	rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, 5000000, sudu);
	rt_kprintf("sudu=%d",sudu);
}

void Left_Qianjin(rt_uint32_t sudu)
{
	rt_pin_write(LEFT_MOTOT_PIN_1,PIN_HIGH);
	rt_pin_write(LEFT_MOTOT_PIN_2,PIN_LOW);
	rt_pwm_set(pwm_dev_2, PWM_DEV_CHANNEL, 5000000, sudu);
	rt_kprintf("sudu=%d",sudu);
}
void Left_HouTui(rt_uint32_t sudu)
{
	rt_pin_write(LEFT_MOTOT_PIN_1,PIN_LOW);
	rt_pin_write(LEFT_MOTOT_PIN_2,PIN_HIGH);
	rt_pwm_set(pwm_dev_2, PWM_DEV_CHANNEL, 5000000, sudu);
	rt_kprintf("sudu=%d",sudu);
}
void tingZhi(void)
{
	rt_pin_write(LEFT_MOTOT_PIN_1,PIN_LOW);
	rt_pin_write(LEFT_MOTOT_PIN_2,PIN_LOW);
	rt_pin_write(RIGHT_MOTOT_PIN_1,PIN_LOW);
	rt_pin_write(RIGHT_MOTOT_PIN_2,PIN_LOW);
	rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, 5000000, 0);
	rt_pwm_set(pwm_dev_2, PWM_DEV_CHANNEL, 5000000, 0);
}
void zuoZhuan(void)
{
	rt_pin_write(ZHUANXIANG_MOTOT_PIN_1,PIN_HIGH);
	rt_pin_write(ZHUANXIANG_MOTOT_PIN_2,PIN_LOW);
}
void youZhuan(void)
{
	rt_pin_write(ZHUANXIANG_MOTOT_PIN_1,PIN_LOW);
	rt_pin_write(ZHUANXIANG_MOTOT_PIN_2,PIN_HIGH);
}
void zhuanXiangTing(void)
{
	rt_pin_write(ZHUANXIANG_MOTOT_PIN_1,PIN_LOW);
	rt_pin_write(ZHUANXIANG_MOTOT_PIN_2,PIN_LOW);
	
}
static void led_flash(void)
{
	static rt_uint8_t led_counter;
	led_counter++;
	if(led_counter<10)
	{
		rt_pin_write(LED0_PIN,PIN_HIGH);
	}
	else if(led_counter<20)
	{
		rt_pin_write(LED0_PIN,PIN_LOW);
	}
	else
	{
		led_counter=0;
	}
	
}

void motorControl(void)
{
	key=PS2_DataKey();   //
	if ((0==PS2_RedLight())&&((key==PSB_PAD_UP)||(key==PSB_PAD_RIGHT)||(key==PSB_PAD_DOWN)||(key==PSB_PAD_LEFT)||(key==PSB_CIRCLE)||(key==PSB_SQUARE)||(key==PSB_TRIANGLE)||(key==PSB_CROSS) 
		||(PS2_AnologData(PSS_LX)>128)||(PS2_AnologData(PSS_LX)<127)||(PS2_AnologData(PSS_RY)>128)||(PS2_AnologData(PSS_RY)<127)    //红灯模式  遥控器有效按键按下时，执行遥控器指令
		))
	{
		switch(key)
		{
			case PSB_PAD_UP: 	
			{
				Rigth_Qianjin(5000000);  //低速前进
				rt_kprintf("PSB_PAD_UP \n");
			} break;  
			case PSB_PAD_RIGHT:	
			{
				zuoZhuan();  //左转
				rt_kprintf("PSB_PAD_RIGHT \n");
			}  break;
			case PSB_PAD_DOWN:	
			{
				Rigth_HouTui(5000000);  //低速后退	
				rt_kprintf("PSB_PAD_DOWN \n");
			}  break; 
			case PSB_PAD_LEFT:	
			{
				youZhuan();   //右转
				rt_kprintf("PSB_PAD_LEFT \n"); 
			} break; 
			
			case PSB_TRIANGLE:	rt_kprintf("PSB_TRIANGLE \n");  break; 
			case PSB_CIRCLE:  	
			{
				youZhuan();
				rt_kprintf("PSB_CIRCLE \n");
			}  break; 
			case PSB_CROSS:   	rt_kprintf("PSB_CROSS \n");  break; 
			case PSB_SQUARE:  	
			{
				zuoZhuan();  //左转
				rt_kprintf("PSB_SQUARE \n"); 
			} break;
			default: 
			{
				tingZhi();  //停止
			}
			break;
		}			
		//前进和后退控制	
		if(PS2_AnologData(PSS_LX)>128)
		{
			rt_kprintf(" Left_Anolog=%5d %5d  ",PS2_AnologData(PSS_LX),PS2_AnologData(PSS_LY));
			Rigth_HouTui((PS2_AnologData(PSS_LX)-128)*39000);
		}
		else if(PS2_AnologData(PSS_LX)<127)
		{
			rt_kprintf(" Left_Anolog=%5d %5d  ",PS2_AnologData(PSS_LX),PS2_AnologData(PSS_LY));
			
			Rigth_Qianjin((127-PS2_AnologData(PSS_LX))*39000);
		}
		//左转和右转控制	
		if(PS2_AnologData(PSS_RY)>180)
		{
			rt_kprintf(" Right_Anolog=%5d %5d  ",PS2_AnologData(PSS_RX),PS2_AnologData(PSS_RY));
			youZhuan();
			
		}
		else if(PS2_AnologData(PSS_RY)<80)
		{
			rt_kprintf(" Right_Anolog=%5d %5d  ",PS2_AnologData(PSS_RX),PS2_AnologData(PSS_RY));
			zuoZhuan();  //左转
		}	
		rt_kprintf(" zuoZhuan_flag=%d,  youZhuan_flag=%d \n",zuoZhuan_flag,youZhuan_flag);
	}
	else if((1==PS2_RedLight())&&((key==PSB_PAD_UP)||(key==PSB_PAD_RIGHT)||(key==PSB_PAD_DOWN)||(key==PSB_PAD_LEFT)||(key==PSB_CIRCLE)||(key==PSB_SQUARE)||(key==PSB_TRIANGLE)||(key==PSB_CROSS) ))   //绿灯模式。遥控器有效按键按下时，执行遥控器指令
	{
		switch(key)
		{
			case PSB_PAD_UP: 	
			{
				Rigth_Qianjin(3000000);  //低速前进
				rt_kprintf("PSB_PAD_UP \n");
			} break;  
			case PSB_PAD_RIGHT:	
			{
				youZhuan();
				rt_kprintf("PSB_PAD_RIGHT \n");
			}  break;
			case PSB_PAD_DOWN:	
			{
				Rigth_HouTui(3000000);  //低速后退	
				rt_kprintf("PSB_PAD_DOWN \n");
			}  break; 
			case PSB_PAD_LEFT:	
			{
				zuoZhuan();  //左转
				rt_kprintf("PSB_PAD_LEFT \n"); 
			} break; 
			
			case PSB_TRIANGLE:	rt_kprintf("PSB_TRIANGLE \n");  break; 
			case PSB_CIRCLE:  	
			{
				youZhuan();
				rt_kprintf("PSB_CIRCLE \n");
			}  break; 
			case PSB_CROSS:   	rt_kprintf("PSB_CROSS \n");  break; 
			case PSB_SQUARE:  	
			{
				zuoZhuan();  //左转
				rt_kprintf("PSB_SQUARE \n"); 
			} break;
			default: 
			{
				tingZhi();  //停止
			}
			break;
		}				
	}
	else							//没有收到遥控器有效指令时执行手动功能。
	{
		zhuanXiangTing();
		pinRead();
		if(((inputState.jin_state==1)&&(inputState.tui_state==1))||(inputState.jiaota_state==1))
		{
			tingZhi();  //停止
		}
		if(inputState.jiaota_state==0)  //脚踏开
		{
			if((inputState.tui_state==0)&&(inputState.jin_state==1))  //前进模式
			{
				if(inputState.sudu_state==1)
				{
					Rigth_Qianjin(3000000);  //低速前进
				}
				else if(inputState.sudu_state==0)		
				{
					Rigth_Qianjin(5000000);  //高速前进
				}
			}
			else if((inputState.tui_state==1)&&(inputState.jin_state==0))  //后退模式
			{
				if(inputState.sudu_state==1)
				{
					Rigth_HouTui(2000000);  //低速后退
				}
				else if(inputState.sudu_state==0)		
				{
					Rigth_HouTui(4000000);  //高速后退
				}
			}
		}
		else //脚踏关
		{
			tingZhi();  //停止
			zhuanXiangTing(); //转向停
		}
	}
}
static rt_thread_t control_tid = RT_NULL;      /* 线程句柄 */
static void conntrolHandle(void *parameter)
{
	while(1)
	{
		motorControl();
		led_flash();
		rt_thread_mdelay(100);
	}
}

int control_sample(void)
{    
    /* 创建线程，名称是 pwm_thread ，入口是 pwm_entry*/
    control_tid = rt_thread_create("control_handle",
                             conntrolHandle,
                             RT_NULL,
                             THREAD_STACK_SIZE,
                             THREAD_PRIORITY,
                             THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (control_tid != RT_NULL)
        rt_thread_startup(control_tid);

    return RT_EOK;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(control_sample, conntrol sample);

