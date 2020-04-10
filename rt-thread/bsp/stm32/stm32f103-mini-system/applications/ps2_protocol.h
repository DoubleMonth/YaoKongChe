

#ifndef __PS2_PROTOCOL_H__
#define __PS2_PROTOCOL_H__

#include <rthw.h>
#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>
#include <board.h>



/*小车运行状态枚举*/
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

extern int g_CarState; //  1前2后3左4右0停止
extern int CarSpeedControl;


void Protocol(void);
int ps2_sample(void);


#endif
