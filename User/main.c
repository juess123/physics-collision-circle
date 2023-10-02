#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "circle.h"
#include <stdlib.h>
#include <rtthread.h>

#include "MPU6050.h"

#include "TrajectoryProcessing.h"

#define AcceSen 1300

int16_t Acce_x=1;

int16_t Acce_y=1;


ACCE*stAcce=NULL;

void accelerationMeasure(void* S)
{

	while(1)
	{
		MPU6050_GetData(&Acce_y,&Acce_x);
		
		Acce_x/=AcceSen;
		
		Acce_y/=AcceSen;
		
		if(Acce_y>6)Acce_y=6;
		
		if(Acce_x>6)Acce_x=6;
		
		rt_thread_delay(3);
	}
	
}
void accelerationHandle(void* Null)
{
	stAcce=initAcce();
	
	ACCE*temp=initAcce();
	
	while(1)
	{
		acceDetach(temp,Acce_x,Acce_y);
		
		speedSystemX(temp,stAcce);
		
		speedSystemY(temp,stAcce);
		
		rt_thread_delay(3);
	}
}
void cricleMove_y(void* S)
{
	int16_t speed_y=0,distance_y=0;
	while(1)
	{
		speed_y=6-stAcce->A_y;
		move_y(S,&(stAcce->A_diry),1);
		if(speed_y==6)
		{
			stAcce->A_diry=0;
		}
		distance_y++;
		if(distance_y>50-speed_y*8)
		{
			speed_y++;
			distance_y=0;
			if(stAcce->A_y>0)stAcce->A_y--;
		}
		
		
		rt_thread_delay(speed_y);
	}
	
}
void cricleMove_x(void* S)
{
	
	complementCricle*C=initComCri();
	
	int16_t speed_x=0,distance_x=0;
	
	while(1)
	{
		speed_x=6-stAcce->A_x;
		move_x(S,&(stAcce->A_dirx),1,C);
		if(speed_x==6)
		{
			stAcce->A_dirx=0;
		}
		distance_x++;
		if(distance_x>50-speed_x*8)
		{
			speed_x++;
			distance_x=0;
			if(stAcce->A_x>0)stAcce->A_x--;
		}
		
		
		rt_thread_delay(speed_x);
	}
	
}
int main()
{
	OLED_Init();
	
	MPU6050_Init();
	
	SizeNode*S=creatCricle(64,32,7);
	
	
	rt_thread_t MPU6050=rt_thread_create("accelerationMeasure",accelerationMeasure,S,512,0,10);
	
	rt_thread_t AcceHand=rt_thread_create("accelerationHandle",accelerationHandle,NULL,512,1,10);
	
	rt_thread_t AricleMove_x=rt_thread_create("cricleMove_x",cricleMove_x,S,512,2,10);
	
	rt_thread_t AricleMove_y=rt_thread_create("cricleMove_y",cricleMove_y,S,512,2,10);
	
	rt_thread_startup(MPU6050);
	
	rt_thread_startup(AcceHand);
	
	rt_thread_startup(AricleMove_x);
	
	rt_thread_startup(AricleMove_y);
}
