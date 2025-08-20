#include "timer.h"
#include "auto.h"
#include "delay.h"




void Straight(void)         //走直
{ 
	PGout(11)=PGout(12)=1;
	PGout(13)=PGout(14)=0;
  TIM_SetCompare1(TIM3,2500);
	TIM_SetCompare2(TIM3,3500);//
}
void TurnLeft(void)         //左转
{ 
	//	PGout(11)=PGout(14)=0;
//	PGout(13)=PGout(12)=1;
//  TIM_SetCompare1(TIM3,1500);//
//	TIM_SetCompare2(TIM3,3200);//
	PGout(11)=PGout(12)=1;
	PGout(13)=PGout(14)=0;
  TIM_SetCompare1(TIM3,500);
	TIM_SetCompare2(TIM3,6000);
	
}
void TurnRight(void)           //右转
{ 
//	PGout(11)=PGout(14)=1;
//	PGout(13)=PGout(12)=0;
//  TIM_SetCompare1(TIM3,2200);
//	TIM_SetCompare2(TIM3,4000);
//	
	PGout(11)=PGout(12)=1;
	PGout(13)=PGout(14)=0;
  TIM_SetCompare1(TIM3,4000);
	TIM_SetCompare2(TIM3,0);
}

void TrimLeft(void)          //左转微调
{ 
	PGout(11)=PGout(12)=1;
	PGout(13)=PGout(14)=0;
  TIM_SetCompare1(TIM3,2500);
	TIM_SetCompare2(TIM3,5200);
}
void TrimRight(void)         //右转微调
{ 
	PGout(11)=PGout(12)=1;
	PGout(13)=PGout(14)=0;
  TIM_SetCompare1(TIM3,3700);
	TIM_SetCompare2(TIM3,3500);
}
void Stop(void)
{
  PGout(11)=PGout(12)=1;
	PGout(13)=PGout(14)=0;
  TIM_SetCompare1(TIM3,0);
	TIM_SetCompare2(TIM3,0);
}
void turn_around(void)
{
	PGout(11)=PGout(14)=1; //左轮正转，右轮反转
	PGout(13)=PGout(12)=0;
  TIM_SetCompare1(TIM3,3000);
	TIM_SetCompare2(TIM3,4000);
}
