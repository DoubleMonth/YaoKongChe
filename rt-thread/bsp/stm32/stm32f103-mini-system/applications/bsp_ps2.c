#include "bsp_ps2.h"

rt_uint16_t Handkey;
rt_uint8_t Comd[2]={0x01,0x42};	//��ʼ�����������
//rt_uint8_t scan[9]={0x01,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//{0x01,0x42,0x00,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A};	// ���Ͷ�ȡ

rt_uint8_t Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //���ݴ洢����
rt_uint16_t MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
};	//����ֵ�밴����

void PS2_Init(void)
{
	rt_pin_mode(DI_PIN,PIN_MODE_INPUT_PULLDOWN);
	rt_pin_mode(DO_PIN, PIN_MODE_OUTPUT);
	rt_pin_mode(CS_PIN, PIN_MODE_OUTPUT);
	rt_pin_mode(CLK_PIN, PIN_MODE_OUTPUT);
	
//	rt_pin_write(DO_PIN,PIN_HIGH);
//	rt_pin_write(CS_PIN,PIN_HIGH);
//	rt_pin_write(CLK_PIN,PIN_HIGH);
}
//void PS2_Cmd(rt_uint8_t  CMD)
//{
//	volatile rt_uint16_t ref=0x01;
//	Data[1]=0;
//	for(ref=0x01;ref<0x0100;ref<<=1)
//	{
//		if(ref&CMD)
//		{
//			rt_pin_write(DO_PIN,PIN_HIGH);
//		}
//		else
//		{
//			rt_pin_write(DO_PIN,PIN_LOW);
//		}
//		rt_pin_write(CLK_PIN,PIN_HIGH);
//		rt_hw_us_delay(15);//5us
//		rt_pin_write(CLK_PIN,PIN_LOW);
//		rt_hw_us_delay(15);//5us
//		rt_pin_write(CLK_PIN,PIN_HIGH);
//		if(PIN_HIGH==rt_pin_read(DI_PIN))
//			Data[1]=ref|Data[1];
//	}
//	rt_hw_us_delay(16);//16us
//}
void PS2_Cmd(rt_uint8_t  CMD)
{
	rt_uint8_t ref=0,i=0;
	Data[1]=0;
	ref=CMD;
	for(i=0;i<8;i++)
	{
		rt_pin_write(CLK_PIN,PIN_HIGH);
		if((ref&0x01)==0x01)
		{
			rt_pin_write(DO_PIN,PIN_HIGH);
		}
		else
		{
			rt_pin_write(DO_PIN,PIN_LOW);
		}
		rt_hw_us_delay(3);//5us
		rt_pin_write(CLK_PIN,PIN_LOW);
		rt_hw_us_delay(3);//5us
		if(PIN_HIGH==rt_pin_read(DI_PIN))
		{
			
			Data[1]=(0x01<<i)|Data[1];
		}
		rt_pin_write(CLK_PIN,PIN_HIGH);
		ref>>=1;
	}
	rt_hw_us_delay(30);//16us
}

//�ж��Ƿ�Ϊ���ģʽ,0x41=ģ���̵ƣ�0x73=ģ����
//����ֵ��0�����ģʽ
//		  ����������ģʽ
rt_uint8_t PS2_RedLight(void)
{
	rt_pin_write(CS_PIN,PIN_LOW);
	rt_hw_us_delay(10);//16us
	PS2_Cmd(Comd[0]);  //��ʼ����
	PS2_Cmd(Comd[1]);	//��������
	rt_hw_us_delay(10);//16us
	rt_pin_write(CS_PIN,PIN_HIGH);
	if(Data[1]==0x73)
	{
		rt_kprintf("Data[0]=%d\n",Data[0]);
		rt_kprintf("Data[1]=%d\n",Data[1]);
		return 0;  //ģ����
	}
	else
	{
		rt_kprintf("Data[0]=%d\n",Data[0]);
		rt_kprintf("Data[1]=%d\n",Data[1]);
		return 1;  //ģ���̵�
	}
		
}

//��ȡ�ֱ�����
void PS2_ReadData(void)
{
	volatile rt_uint8_t byte=0,ref=0,i=0;
	rt_pin_write(CS_PIN,PIN_LOW);
	PS2_Cmd(Comd[0]);  //��ʼ����
	PS2_Cmd(Comd[1]);	//��������
	for(byte=2;byte<9;byte++)
	{
		for(i=0; i<8; i++)
		{
			rt_pin_write(CLK_PIN,PIN_HIGH);
			rt_hw_us_delay(5);//5us
			rt_pin_write(CLK_PIN,PIN_LOW);
			rt_hw_us_delay(5);//5us
			if(PIN_HIGH==rt_pin_read(DI_PIN))
			{
				Data[byte]=(0x01<<i)|Data[byte];
			}
			rt_pin_write(CLK_PIN,PIN_HIGH);
		}
		rt_hw_us_delay(16);//16us
	}
	rt_pin_write(CS_PIN,PIN_HIGH);
}

//��ȡ�ֱ�����
//void PS2_ReadData(void)
//{
//	volatile rt_uint8_t byte=0;
//	volatile rt_uint16_t ref=0x01;
//	rt_pin_write(CS_PIN,PIN_LOW);
//	PS2_Cmd(Comd[0]);  //��ʼ����
//	PS2_Cmd(Comd[1]);	//��������
//	for(byte=2;byte<9;byte++)
//	{
//		for(ref=0x01;ref<0x100;ref<<=1)
//		{
//			rt_pin_write(CLK_PIN,PIN_HIGH);
//			rt_hw_us_delay(5);//5us
//			rt_pin_write(CLK_PIN,PIN_LOW);
//			rt_hw_us_delay(5);//5us
//			
//			if(rt_pin_read(DI_PIN))
//			Data[byte]=(ref)|Data[byte];
//			rt_pin_write(CLK_PIN,PIN_HIGH);
//		}
//		rt_hw_us_delay(16);//16us
//	}
//	rt_pin_write(CS_PIN,PIN_HIGH);
//}



//�Զ�������PS2�����ݽ��д���,ֻ����������  
//ֻ��һ����������ʱ����Ϊ0�� δ����Ϊ1
rt_uint8_t PS2_DataKey()
{
	rt_uint8_t index;

	PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3];     //����16������  ����Ϊ0�� δ����Ϊ1
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		return index+1;
	}
	return 0;          //û���κΰ�������
}

//�õ�һ��ҡ�˵�ģ����	 ��Χ0~256
rt_uint8_t PS2_AnologData(rt_uint8_t button)
{
	return Data[button];
}
//������ݻ�����
void PS2_ClearData()
{
	rt_uint8_t a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}


/******************************************************
Function:    void PS2_Vibration(u8 motor1, u8 motor2)
Description: �ֱ��𶯺�����
Calls:		 void PS2_Cmd(u8 CMD);
Input: motor1:�Ҳ�С�𶯵�� 0x00�أ�������
	   motor2:�����𶯵�� 0x40~0xFF �������ֵԽ�� ��Խ��
******************************************************/
void PS2_Vibration(rt_uint8_t motor1, rt_uint8_t motor2)
{
	rt_pin_write(CS_PIN,PIN_LOW);
	rt_hw_us_delay(16);
    PS2_Cmd(0x01);  //��ʼ����
	PS2_Cmd(0x42);  //��������
	PS2_Cmd(0X00);
	PS2_Cmd(motor1);
	PS2_Cmd(motor2);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	rt_pin_write(CS_PIN,PIN_HIGH);
	rt_hw_us_delay(16);
}
//short poll
void PS2_ShortPoll(void)
{
	rt_pin_write(CS_PIN,PIN_LOW);
	rt_hw_us_delay(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
	rt_pin_write(CS_PIN,PIN_HIGH);
	rt_hw_us_delay(16);	
}
//��������
void PS2_EnterConfing(void)
{
    rt_pin_write(CS_PIN,PIN_LOW);
	rt_hw_us_delay(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	rt_pin_write(CS_PIN,PIN_HIGH);
	rt_hw_us_delay(16);
}
//����ģʽ����
void PS2_TurnOnAnalogMode(void)
{
	rt_pin_write(CS_PIN,PIN_LOW);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x44);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01); //analog=0x01;digital=0x00  ������÷���ģʽ
	PS2_Cmd(0xEE); //Ox03�������ã�������ͨ��������MODE������ģʽ��
				   //0xEE������������ã���ͨ��������MODE������ģʽ��
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	rt_pin_write(CS_PIN,PIN_HIGH);
	rt_hw_us_delay(16);
}
//������
void PS2_VibrationMode(void)
{
	rt_pin_write(CS_PIN,PIN_LOW);
	rt_hw_us_delay(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x4D);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
	rt_pin_write(CS_PIN,PIN_HIGH);
	rt_hw_us_delay(16);
}
//��ɲ���������
void PS2_ExitConfing(void)
{
    rt_pin_write(CS_PIN,PIN_LOW);
	rt_hw_us_delay(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	rt_pin_write(CS_PIN,PIN_HIGH);
	rt_hw_us_delay(16);
}
//�ֱ����ó�ʼ��
void PS2_SetInit(void)
{
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();		//��������ģʽ
	PS2_TurnOnAnalogMode();	//�����̵ơ�����ģʽ����ѡ���Ƿ񱣴�
	PS2_VibrationMode();	//������ģʽ
	PS2_ExitConfing();		//��ɲ���������
}


//��ȡ�ֱ�����
//rt_uint8_t PS2_ReadData(rt_uint16_t command)
//{

//	rt_uint8_t i,j=1;
//	rt_uint8_t res=0; 
//    for(i=0; i<=7; i++)          
//    {
//		if(command&0x01)
//			rt_pin_write(DO_PIN,PIN_HIGH);
//		else
//			rt_pin_write(DO_PIN,PIN_LOW);
//		command = command >> 1;
//		rt_hw_us_delay(5);//10
//		rt_pin_write(CLK_PIN,PIN_LOW);
//		rt_hw_us_delay(5); //10
//		if(rt_pin_read(DI_PIN)) 
//			res = res + j;
//		j = j << 1; 
//		rt_pin_write(CLK_PIN,PIN_HIGH);
//		rt_hw_us_delay(10);//10	 
//    }
//    rt_pin_write(DO_PIN,PIN_HIGH);
//	rt_hw_us_delay(30);//50
//    return res;	
//}

//������ݻ�����
//void PS2_ClearData(void)
//{
//	rt_uint8_t a;
//	for(a=0;a<9;a++)
//		Data[a]=0x00;
//}

////�Զ������� PS2 �����ݽ��д���
////����Ϊ 0�� δ����Ϊ 1
//unsigned char PS2_DataKey(void)
//{
//	rt_uint8_t index = 0, i = 0;

//	PS2_ClearData();
//	rt_pin_write(CS_PIN,PIN_LOW);
//	for(i=0;i<9;i++)	//����ɨ�谴��
//	{
//		Data[i] = PS2_ReadData(scan[i]);	
//	} 
//	rt_pin_write(CS_PIN,PIN_HIGH);
//	

//	Handkey=(Data[4]<<8)|Data[3];     //����16������  ����Ϊ0�� δ����Ϊ1
//	for(index=0;index<16;index++)
//	{	    
//		if((Handkey&(1<<(MASK[index]-1)))==0)
//			return index+1;
//	}
//	return 0;          //û���κΰ�������
//}

////�õ�һ��ҡ�˵�ģ����	 ��Χ0~256
//rt_uint8_t PS2_AnologData(rt_uint8_t button)
//{
//	return Data[button];
//}

