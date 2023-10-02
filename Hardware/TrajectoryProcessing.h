#ifndef TrajectoryProcessing_H
#define TrajectoryProcessing_H

typedef struct DIR
{
	uint8_t pace_x;
	uint8_t pace_y;
	uint8_t integralCumulativeError;
	uint8_t speed;
}DIR;

typedef struct ACCE
{
	int16_t A_x;
	int8_t A_dirx;
	int16_t A_y;
	int8_t A_diry;
}ACCE;

ACCE*initAcce(void);

DIR*initDir(void);

void acceDetach(ACCE*stAcce,int16_t Acce_x,int16_t Acce_y);

void move_direction(ACCE*stAcce,DIR*dir);


void speedSystemX(ACCE*temp,ACCE*stAcce);

void speedSystemY(ACCE*temp,ACCE*stAcce);

#endif
