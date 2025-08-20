#include "delay.h"
#include "timer.h"
#include "driving.h"

//////////////////////////////////////////////////////////////////////////////////	 
							  
////////////////////////////////////////////////////////////////////////////////// 	   



int t1,t2,t3,t4,t[4][4];
extern int timer;
float time,dist=0;

void UWave_Init(void)//���ų�ʼ��
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO, ENABLE);  
 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//ECHO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//��������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;//TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	PEout(5)=1;
}

void UWave_Int_Init(void){//�������жϳ�ʼ��
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//ʹ�ܸ��ù���ʱ��
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	


  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure); 	
}
void EXTI4_IRQHandler(void)
{
float t;
		if(EXTI_GetITStatus(EXTI_Line4)==SET){
		if(PEin(4)){//�����ؿ�ʼ��ʱ
			time=NOW;
			
		}else{//�½��ؽ�����ʱ
			if(NOW-time<.005)//5������1.7�ף�����2��85����
			t=NOW-time;
			else
			t=0.005;
				//tr[i]=*t1;
				//i++;
				dist=t*170000;}//�������
		
		
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

void UWave_Send(void)
	{//���䳬����
	PEout(5)=1;
	delay_us(20);         //�̶�20us
	PEout(5)=0;
}
void track_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	    //ʹ��PA�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	//PE 0-1�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	 //��������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOE, &GPIO_InitStructure);			     //��ʼ��GPIOE
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
		else if(t1==0&t2==1&t3==1&t4==0)//��Ϊ1����Ϊ0
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
