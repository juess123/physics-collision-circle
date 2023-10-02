#include "stm32f10x.h"                  // Device header
#ifndef __CIRCLE_H
#define __CIRCLE_H
extern uint8_t move_y1;
extern uint8_t move_y2;
typedef struct SizeNode
{
	double circle_x;
	double circle_y1;
	double circle_y2;           
	struct SizeNode* next;
	struct SizeNode* pre;
}SizeNode;

typedef struct complementCricle
{
	uint8_t comCri_x;
	uint8_t comCri_y;
	struct complementCricle* next;
		
}complementCricle;

SizeNode* initSizeNode(int16_t a, int16_t b, int16_t r);

complementCricle* initComCri(void);

void getSize_x(SizeNode* S);

void getSize_y(SizeNode* S);

void drawCircle(SizeNode*S);

void fillCricle(SizeNode*S);

uint8_t freeCricle(SizeNode**S);

void moveCricle_x(SizeNode*S,int8_t *move_x,uint8_t speed);

void reverseFillCricleX(complementCricle*C,int8_t movedir);

void reverseFillCricleY(SizeNode*S,int8_t movedir);

void moveCricle_y(SizeNode*S,int8_t *move_y,uint8_t speed);

SizeNode* creatCricle(int16_t a,int16_t b,int16_t r);

void move_x(SizeNode*S,int8_t *movedir,uint8_t speed,complementCricle*C);

void move_y(SizeNode*S,int8_t *move,uint8_t speed);

void accComCri_L(SizeNode*S,complementCricle*C);

void accComCri_R(SizeNode*S,complementCricle*C);

#endif
