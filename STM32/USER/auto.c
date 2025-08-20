#include "delay.h"
#include "timer.h"
#include "driving.h"

//////////////////////////////////////////////////////////////////////////////////	 
							  
////////////////////////////////////////////////////////////////////////////////// 	   



int t1,t2,t3,t4,t[4][4];
extern int timer;
float time,dist=0;

void UWave_Init(void)//引脚初始化
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO, ENABLE);  
 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//ECHO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//下拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;//TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	PEout(5)=1;
}

void UWave_Int_Init(void){//超声波中断初始化
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//使能复用功能时钟
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	


  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure); 	
}
void EXTI4_IRQHandler(void)
{
float t;
		if(EXTI_GetITStatus(EXTI_Line4)==SET){
		if(PEin(4)){//上升沿开始计时
			time=NOW;
			
		}else{//下降沿结束计时
			if(NOW-time<.005)//5毫秒跑1.7米，除以2得85厘米
			t=NOW-time;
			else
			t=0.005;
				//tr[i]=*t1;
				//i++;
				dist=t*170000;}//计算距离
		
		
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

void UWave_Send(void)
	{//发射超声波
	PEout(5)=1;
	delay_us(20);         //固定20us
	PEout(5)=0;
}
void track_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	    //使能PA端口时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	//PE 0-1端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	 //浮空输入
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
  GPIO_Init(GPIOE, &GPIO_InitStructure);			     //初始化GPIOE
	PEout(0)=PEout(1)=PEout(2)=PEout(3)=0;
}
void read_in(void)
{ int i=0;
	for(i=0;i<1;i++)
  {if(PEin(0)==1) t[i][0]=1;
	else if(PEin(0)==0) t[i][0]=0;
	if(PEin(1)==1) t[i][1]=1;
	else if(PEin(1)==0) t[i][1]=0;
	if(PEin(2)==1) t[i][2]=1;
	else if(PEin(2)==0) t[i][2]=0;
	if(PEin(3)==1)  t[i][3]=1;
	else if(PEin(3)==0) t[i][3]=0;
//	delay_ms(5);
	}
	t1=t[0][0];
	t2=t[0][1];
	t3=t[0][2];
	t4=t[0][3];
}

void auto_driving(void)
{
//	  UWave_Send();
//		delay_ms(5);
//	  if(dist>150)
//		{
			read_in();

		if((t1==1&t2==0&t3==0&t4==0)|(t1==1&t2==1&t3==1&t4==0)|(t1==1&t2==1&t3==0&t4==0))
		{ TurnRight();
			
		 while(!(t1==0&t2==1&t3==0&t4==0))
		 {TurnRight();
		read_in();}
	  }
		else if((t1==0&t2==0&t3==1&t4==1)|(t1==0&t2==0&t3==0&t4==1)|(t1==0&t2==1&t3==1&t4==1))
		{TurnLeft();
		 
		 while(!(t1==0&t2==0&t3==1&t4==0))
		 {TurnLeft();
		 read_in();}
	  }	
		else if(t1==0&t2==1&t3==1&t4==0)//黑为1，白为0
			Straight();
		else if(t1==0&t2==1&t3==0&t4==0)
			TrimRight();
		else if(t1==0&t2==0&t3==1&t4==0)
		  TrimLeft();
		else
		Straight();
//	}
//		else 
//		Stop();
}	
