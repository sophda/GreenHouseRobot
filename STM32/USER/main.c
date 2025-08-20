#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "auto.h"
#include "driving.h"
/************************************************

************************************************/


 
	extern int t1,t2,t3,t4,t[4][4];
	extern float dist;
	extern int timer;
	extern u16 type;
	extern u8 USART_RX_BUF1[USART_REC_LEN],USART_RX_BUF2[15][USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
  extern u16 USART_RX_STA1,USART_RX_STA2;         		//接收状态标记	
	
 int main(void)
 {int g=145,driving=0,turn=0;	
  delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
	uart2_init(9600);	 //串口初始化为9600
	TIM2_PWM_Init(799,719);
 	TIM3_PWM_Init(8999,0);	 //不分频。PWM频率=72000000/9000=8Khz
	TIM6_Int_Init(999,719);	//用于计时 
	UWave_Init();
	UWave_Int_Init();
	track_Init();
	
  while(1)
	{
	
////			if(USART_RX_BUF1[0]=='b')
////			{auto_driving();
////			 driving=1;
////			}
////			if(driving==1)
//			  auto_driving();
	
		if(USART_RX_STA1&0x8000)
			{	
//			printf("t6b18b5000b5000b5000b29.5664b66.3678\r\n");
			if(USART_RX_BUF1[0]=='c')
			{g=g-3;
			}
		  if(USART_RX_BUF1[0]=='d')
			{g=g+3;
			}
      if(USART_RX_BUF1[0]=='a')
		  {Stop();
			driving=0;
			turn=0;
			}
			if(USART_RX_BUF1[0]=='b')
			{auto_driving();
			 driving=1;
			 turn=0;
			}
			if(USART_RX_BUF1[0]=='e')
			{turn_around();
			 turn=1;
			}
			USART_RX_STA1=0;
		 }
			TIM_SetCompare1(TIM2,g);
			if(driving==0&turn==0)
		  Stop();
			if(driving==1&turn==0)
			auto_driving();
	    if(turn==1)
			turn_around();	

//			{
//		if(USART_RX_STA2&0x8000)
//			{	
//			
//			if(USART_RX_BUF2[type][2]==0x53)
//			{printf("\r\n翻滚角X为%f",((short)((USART_RX_BUF2[3][4]<<8)|USART_RX_BUF2[3][3])/32768.0*180));
//			
//		  printf("\r\n俯仰角Y为%f",((short)((USART_RX_BUF2[3][6]<<8)|USART_RX_BUF2[3][5])/32768.0*180));
//				
//			printf("\r\n偏航角Z为%f",((short)((USART_RX_BUF2[3][8]<<8)|USART_RX_BUF2[3][7])/32768.0*180));
//				
//			printf("\r\nax为%f",((short)((USART_RX_BUF2[1][4]<<8)|USART_RX_BUF2[1][3])/32768.0*16*9.8));
//			
//		  printf("\r\nay为%f",((short)((USART_RX_BUF2[1][6]<<8)|USART_RX_BUF2[1][5])/32768.0*16*9.8));
//				
//			printf("\r\naz为%f",((short)((USART_RX_BUF2[1][8]<<8)|USART_RX_BUF2[1][7])/32768.0*16*9.8));
//				
//			printf("\r\n温度为%f",((USART_RX_BUF2[1][10]<<8)|USART_RX_BUF2[1][9])/100.0);
//			printf("\r\n当前经度为%d",(USART_RX_BUF2[7][6]<<24)|(USART_RX_BUF2[7][5]<<16)|(USART_RX_BUF2[7][4]<<8)|USART_RX_BUF2[7][3]);
//			
//		  printf("\r\n当前纬度为%d",(USART_RX_BUF2[7][10]<<24)|(USART_RX_BUF2[7][9]<<16)|(USART_RX_BUF2[7][8]<<8)|USART_RX_BUF2[7][7]);
//			USART_RX_STA2=0;
//			
//			}
//   USART_RX_STA2=0;
//		}
			
}
	}
