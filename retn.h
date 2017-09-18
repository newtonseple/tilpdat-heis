#ifndef __INCLUDE_RETN_H__
#define __INCLUDE_RETN_H__

#include "elev.h" //Included for motor direction type

int getCurrentFloor();
void setCurrentFloor(int floor);
int getBetweenFloors();
void setBetweenFloors(int floor);

void setOrder(elev_button_type_t btype,int floor,int on);
void resetOrders();

int isOrdered();

elev_motor_direction_t decideDirection();

#endif
