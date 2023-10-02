#include "stm32f10x.h"                  // Device header
#include <stdlib.h>
#include "TrajectoryProcessing.h"

DIR*initDir(void)
{
	DIR*dir=malloc(sizeof(DIR));
	dir->pace_x=0;
	dir->pace_y=0;
	dir->speed=0;
	dir->integralCumulativeError=0;
	return dir;
}
ACCE*initAcce(void)
{
	ACCE* st_Acce=malloc(sizeof(ACCE));
	st_Acce->A_dirx=0;
	st_Acce->A_x=0;
	st_Acce->A_diry=0;
	st_Acce->A_y=0;
	return st_Acce;
}
void acceDetach(ACCE*stAcce,int16_t Acce_x,int16_t Acce_y)
{

	if(Acce_x<0)
	{
		stAcce->A_x=(int16_t)abs((int)Acce_x);
		stAcce->A_dirx=-1;
	}
	else if(Acce_x>0)
	{
		stAcce->A_x=Acce_x;
		stAcce->A_dirx=1;
	}
	else
	{
		stAcce->A_x=Acce_x;
		stAcce->A_dirx=0;
	}
	if(Acce_y<0)
	{
		stAcce->A_y=(int16_t)abs((int)Acce_y);
		stAcce->A_diry=-1;
	}
	else if(Acce_y>0)
	{
		stAcce->A_y=Acce_y;
		stAcce->A_diry=1;
	}
	else	
	{
		stAcce->A_y=Acce_y;
		stAcce->A_diry=0;
	}
	
}
void speedSystemX(ACCE*temp,ACCE*stAcce)
{
	if(temp->A_x>6)temp->A_x=6;
	if(stAcce->A_dirx==0)
	{
		stAcce->A_x=temp->A_x;
		stAcce->A_dirx=temp->A_dirx;
	}
	else if(stAcce->A_dirx==temp->A_dirx)
	{
		if(stAcce->A_x<=temp->A_x)
		{
			stAcce->A_x=temp->A_x;
		}
	}
	else if(stAcce->A_dirx!=temp->A_dirx)
	{
		if(stAcce->A_x-temp->A_x>=0)
		{
			stAcce->A_x-=temp->A_x/2;
		}
		else
		{
			stAcce->A_x=0;
		}
	}
}
void speedSystemY(ACCE*temp,ACCE*stAcce)
{
	if(temp->A_y>6)temp->A_y=6;
	if(stAcce->A_diry==0)
	{
		stAcce->A_y=temp->A_y;
		stAcce->A_diry=temp->A_diry;
	}
	else if(stAcce->A_diry==temp->A_diry)
	{
		if(stAcce->A_y<=temp->A_y)
		{
			stAcce->A_y=temp->A_y;
		}
	}
	else if(stAcce->A_diry!=temp->A_diry)
	{
		if(stAcce->A_y-temp->A_y>=0)
		{
			stAcce->A_y-=temp->A_y/2;
		}
		else
		{
			stAcce->A_y=0;
		}
	}
}