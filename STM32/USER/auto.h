#ifndef __AUTO_H
#define __AUTO_H	 
#include "sys.h"



void UWave_Init(void); //引脚初始化
void UWave_Int_Init(void); //超声波中断初始化
void UWave_Send(void);	
void track_Init(void);
void read_in(void);
void auto_driving(void);
#endif
