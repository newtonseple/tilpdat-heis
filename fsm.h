#ifndef __INCLUDE_FSM_H__
#define __INCLUDE_FSM_H__

#include "elev.h"

typedef enum {stInit, stActive, stEStop, stPEStop, stLanding} state;

state getState();

void evTimeout();
void evInit();
void evFloorReached(int floor);
void evUnStop();
void evStop();
void evBtnSignal(elev_button_type_t btype,int floor);

#endif
