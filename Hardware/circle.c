#include <stdlib.h>
#include <math.h>
#include "circle.h"
#include "OLED.h"
#include "stm32f10x.h"


#define comdir_L 1
#define comdir_R -1

SizeNode* initSizeNode(int16_t a, int16_t b, int16_t r)
{
	SizeNode* size = malloc(sizeof(SizeNode));
	if (size)
	{
		size->circle_x = a;       //在头结点中y1是b,y2是r.在其他节点中y1，y2 是x对应的两个点。
		size->circle_y1 = b;
		size->circle_y2 = r;      //(x-a)²+(y-b)²=r²;
		size->next = NULL;
		size->pre = NULL;
	}
	return size;
}
uint8_t freeCricle(SizeNode**S)
{
	SizeNode* node = (*S)->next;
	if (node)
	{
		if (node)
		{
			(*S)->next = node->next;
			free(node);
		}
		freeCricle(S);
		return 0;
	}
	else
	{
		(*S) = NULL;
		free(*S);
		return 1;
	}
}
void getSize_x(SizeNode* S)
{
	int16_t index = 0;
	double X=S->circle_x-S->circle_y2;
	while (index <= S->circle_y2*2)
	{
		SizeNode* node = malloc(sizeof(SizeNode));
		node->circle_x = X;
		if(index==0)
		{
			node->next=S->next;
			node->pre=S;
			S->next=node;
		}
		else
		{
			node->pre = S;
			node->next = S->next;
			S->next->pre = node;
			S->next = node;
		}
		X++;
		index++;
	}
	
}
void getSize_y(SizeNode* S)
{
	if (S)
	{
		SizeNode* size=S->next;
		while (size!=NULL)
		{
			size->circle_y1 =(int16_t)(sqrt( pow((double)S->circle_y2, 2) - pow((double)(size->circle_x - S->circle_x), 2))+(double)S->circle_y1)/1;
			if (size->circle_y1 - S->circle_y1 == 0)
			{
				size->circle_y2 = size->circle_y1;
			}
			else
			{
				size->circle_y2 = size->circle_y2 = size->circle_y1 - (size->circle_y1-S->circle_y1)*2;
				if (size->circle_y2 < 0)size->circle_y2 = 0;
			}
			size = size->next;
		}
	}
	else
	{
		return;
	}
}
void drawCircle(SizeNode*S)
{
	SizeNode*size=S->next;
	uint8_t X,y1,y2,move,Y;
	while(size!=NULL)
	{
		X=(uint8_t)size->circle_x;
		y1=(uint8_t)size->circle_y1;
		y2=(uint8_t)size->circle_y2;
		
		move=y1%8;
		Y=y1/8;
		
		
		OLED_SetCursor(Y,X);
		OLED_WriteData(0x01<<move);
		
		move=y2%8;
		Y=y2/8;
		
		
		
		OLED_SetCursor(Y,X);
		OLED_WriteData(0x01<<move);
		size=size->next;
		
	}
}
uint8_t criclePartY1(uint8_t data)
{
	uint8_t temp=data%8;
	uint8_t oled=0x01;
	for(uint8_t i=temp;i>0;i--)
	{
		oled=oled|(0x01<<i);
	}
	return oled;             //Y1是大值，是在oled坐标系中是在圆心下面的填充的是8格中上面的
}
uint8_t criclePartY2(uint8_t data)
{
	uint8_t temp=data%8;
	uint8_t oled=0x80;
	for(uint8_t i=temp;i<8;i++)
	{
		oled=oled|(0x01<<i);
	}
	return oled;	         //Y2是小值，是在oled坐标系中是在圆心上面的填充的是8格中下面的
}
void cricleCenter(uint8_t Y1,uint8_t Y2,uint8_t X)
{
	uint8_t temp=Y1-Y2;
	while(temp>1)
	{
		OLED_SetCursor(Y2+=1,X);
		OLED_WriteData(0xff);
		temp--;
	}
}
void fillCricle(SizeNode*S)
{
	SizeNode*size=S->next;
	uint8_t X,y1,y2,Y1,Y2;
	while(size!=NULL)
	{
		X=(uint8_t)size->circle_x;
		y1=(uint8_t)size->circle_y1;
		y2=(uint8_t)size->circle_y2;
		if(y1==y2)
		{
			uint8_t temp=y1%8;
			Y1=y1/8;
			OLED_SetCursor(Y1,X);
			OLED_WriteData(0x01<<temp);
		}
		else
		{
			
			Y1=y1/8;
			OLED_SetCursor(Y1,X);
			OLED_WriteData(criclePartY1(y1));
			Y2=y2/8;
			OLED_SetCursor(Y2,X);
			OLED_WriteData(criclePartY2(y2));
			cricleCenter(Y1,Y2,X);
		}
		size=size->next;
	}
}
void reverseFillCricleX(complementCricle*C,int8_t movedir)
{
	uint8_t X,Y;
	while(C->comCri_x!=0&&C->comCri_y!=0)
	{
		X=C->comCri_x;
		Y=C->comCri_y/8;
		OLED_SetCursor(Y,X+movedir);
		OLED_WriteData(0x00);
		C=C->next;
	}
}
void reverseFillCricleY(SizeNode*S,int8_t movedir)
{
	uint8_t Y1,Y2;
	SizeNode*node=S->next;
	if(movedir==1)
	{
		while(node)
		{
			Y1=(uint8_t)node->circle_y2/8;
			Y2=(uint8_t)node->circle_y2%8;
			if(Y2==0)
			{
				OLED_SetCursor(Y1-1,(uint8_t)node->circle_x);
				OLED_WriteData(0x00);
				
			}
			node=node->next;
		}
	}
	else
	{
		while(node)
		{
			Y1=(uint8_t)node->circle_y1/8;
			Y2=(uint8_t)node->circle_y1%8;
			if(Y2==7)
			{
				OLED_SetCursor(Y1+1,(uint8_t)node->circle_x);
				OLED_WriteData(0x00);
			}
			node=node->next;
		}
	}
	
}
void moveCricle_x(SizeNode*S,int8_t *move_x,uint8_t speed)
{
	if(*move_x==-1)
	{
		if(S->circle_x<=S->circle_y2)
		{
			*move_x=1;
			return ;
		}
		SizeNode*node=S->next;
		S->circle_x-=speed;
		while(node!=NULL)
		{
			node->circle_x-=speed;
			node=node->next;
		}
	}
	else if(*move_x ==1)
	{
		if(S->circle_x>=127-S->circle_y2)
		{
			*move_x=-1;
			return ;
		}
		SizeNode*node=S->next;
		S->circle_x+=speed;
		while(node!=NULL)
		{
			node->circle_x+=speed;
			node=node->next;
		}
	}
}
void moveCricle_y(SizeNode*S,int8_t *move_y,uint8_t speed)
{
	if(*move_y==-1)
	{
		if(S->circle_y1<=S->circle_y2)
		{
			*move_y=1;
			return ;
		}
		SizeNode*node=S->next;
		S->circle_y1-=speed;
		while(node!=NULL)
		{
			node->circle_y1-=speed;
			node->circle_y2-=speed;
			node=node->next;
		}
	}
	else if(*move_y ==1)
	{
		if(S->circle_y1>=63-S->circle_y2)
		{
			*move_y=-1;
			return ;
		}
		SizeNode*node=S->next;
		S->circle_y1+=speed;
		while(node!=NULL)
		{
			node->circle_y1+=speed;
			node->circle_y2+=speed;
			node=node->next;
		}
	}
}
void move_x(SizeNode*S,int8_t *movedir,uint8_t speed,complementCricle*C)
{
	if(*movedir==1)
	{
		moveCricle_x(S,movedir,1);
		fillCricle(S);
		accComCri_R(S,C);
		reverseFillCricleX(C,comdir_R);
	}
	else if(*movedir==-1)
	{
		moveCricle_x(S,movedir,1);
		fillCricle(S);
		accComCri_L(S,C);
		reverseFillCricleX(C,comdir_L);
	}
	else
	{
		fillCricle(S);
	}
	
}
void move_y(SizeNode*S,int8_t *move,uint8_t speed)
{
	if(*move==0)
	{
		return;
	}
	else
	{
		fillCricle(S); 
	
		moveCricle_y(S,move,speed);
	
		reverseFillCricleY(S,*move);
	}
	
	
	
}
complementCricle* initComCri()
{
	uint8_t comCriSize=8;
	complementCricle* C=malloc(sizeof(complementCricle));
	C->comCri_x=0;
	C->comCri_y=0;
	C->next=NULL;
	while(comCriSize)
	{
		complementCricle*node=malloc(sizeof(complementCricle));
		node->comCri_x=0;
		node->comCri_y=0;
		node->next=C->next;
		C->next=node;
		comCriSize--;
	}
	return C;
}
void accComCri_L(SizeNode*S,complementCricle*C)
{
	SizeNode* node=S->next;
	uint8_t t1,t2,t3,t4,temp=S->circle_y2+1;
	while(temp)
	{
		t1=(uint8_t)node->circle_y1/8;
		t3=(uint8_t)node->circle_y2/8;
		if(t1!=t2)
		{
			C->comCri_x=(uint8_t)node->circle_x;
			C->comCri_y=(uint8_t)node->circle_y1;
			C=C->next;
			t2=t1;
		}
		if(t3!=t4)
		{
			C->comCri_x=(uint8_t)node->circle_x;
			C->comCri_y=(uint8_t)node->circle_y2;
			C=C->next;
			t4=t3;
		}
		temp--;
		node=node->next;
	}
	C->comCri_x=0;
	C->comCri_y=0;
}
void accComCri_R(SizeNode*S,complementCricle*C)
{
	SizeNode* node=S->next;
	uint8_t t1,t2,t3,t4,temp=S->circle_y2+1;
	while(node->next)
	{
		node=node->next;
	}
	
	while(temp)
	{
		t1=(uint8_t)node->circle_y1/8;
		t3=(uint8_t)node->circle_y2/8;
		if(t1!=t2)
		{
			C->comCri_x=(uint8_t)node->circle_x;
			C->comCri_y=(uint8_t)node->circle_y1;
			C=C->next;
			t2=t1;
		}
		if(t3!=t4)
		{
			C->comCri_x=(uint8_t)node->circle_x;
			C->comCri_y=(uint8_t)node->circle_y2;
			C=C->next;
			t4=t3;
		}
		temp--;
		node=node->pre;
	}
	C->comCri_x=0;
	C->comCri_y=0;
}
SizeNode* creatCricle(int16_t a,int16_t b,int16_t r)
{
	if(a>=r&&b>=r)
	{
		SizeNode*S=initSizeNode(a,b,r);
		getSize_x(S);
		getSize_y(S);
		return S;
	}
	else
	{
		OLED_ShowString(1,1,"Circle creation");
		OLED_ShowString(2,1,"failed");
		return NULL;
	}
}
