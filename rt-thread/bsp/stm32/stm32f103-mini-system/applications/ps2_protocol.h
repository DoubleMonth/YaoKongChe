

#ifndef __PS2_PROTOCOL_H__
#define __PS2_PROTOCOL_H__

#include <rthw.h>
#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>
#include <board.h>



/*С������״̬ö��*/
enum{

  enSTOP = 0,
  enRUN,
  enBACK,
  enLEFT,
  enRIGHT,
  enUPLEFT,
  enUPRIGHT,
  enDOWNLEFT,
  enDOWNRIGHT,  
  enTLEFT,
  enTRIGHT

};

extern int g_CarState; //  1ǰ2��3��4��0ֹͣ
extern int CarSpeedControl;


void Protocol(void);
int ps2_sample(void);


#endif
