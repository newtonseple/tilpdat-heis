#include "retn.h"
#include "fsm.h"
#include "elev.h"

static int orders[3][N_FLOORS];

static elev_motor_direction_t currentDirection = DIRN_STOP;

static int currentFloor = 0;
static int betweenFloors = -1;

static void updateLights(){
    //Make sure the order lights mirror the order table

    for (int t = 0; t < 3; t++){
		for (int f = 0; f < N_FLOORS; f++){
			elev_set_button_lamp(t, f, orders[t][f]);
		}
	}
}

static int isOrderedBeyond(elev_motor_direction_t direction){
	//Check if ordered beyond current floor in given direction.

	int uThresh = currentFloor + 1;
	int dThresh = currentFloor - 1;
	
	//In case we are between floors, we need to change the thresholds for "upwards" and "downwards".
	if (betweenFloors > -1){
        uThresh = betweenFloors + 1;
        dThresh = betweenFloors;
	}
	
	
    int floor_min, floor_max;
    //Depending on the direction we are checking, set appropriate limits of iteration.
    if (direction == DIRN_UP){
        floor_min = uThresh;
        floor_max = N_FLOORS - 1;
    } else {
        floor_min = 0;
        floor_max = dThresh;
    }
    
    //Iterate through floors, returning 1 if any order is found.
	for (int f = floor_min; f <= floor_max; f++){
		for (int t = 0; t<3; t++){
			if (orders[t][f] == 1){
				return 1;
			}
		}
	}

	return 0;
}

elev_motor_direction_t decideDirection(){
    //Decide new direction of the carriage, based on the previous direction and order table

    //Make sure we have a direction. If we don't, previous direction is arbitrary
	if (currentDirection == DIRN_STOP){
		currentDirection = DIRN_DOWN;
	}
	
	//If we have an order in the last direction, continue.
	if (isOrderedBeyond(currentDirection)){
		return currentDirection;
	}
	
	//Otherwise, if we have an order in the reverse direction, reverse the direction.
	if (isOrderedBeyond(DIRN_STOP - currentDirection)){
	    currentDirection = DIRN_STOP - currentDirection;
	    return currentDirection;
	}
	
	//If no orders, don't move.
	currentDirection = DIRN_STOP;
    return DIRN_STOP;
}

int isOrdered(){
    //Check if ordered to current floor in an acceptable way.
    
    //Command buttons are always acceptable
	if (orders[2][currentFloor] == 1){
		return 1;

    //Orders in the current direction is always acceptable.
	//If decideDirection has stopped the elevator (no orders, currentDirection is STOP), both directional order types are acceptable.
	} else if ((currentDirection == DIRN_UP || currentDirection == DIRN_STOP) && orders[0][currentFloor] == 1){
		return 1;
	} else if ((currentDirection == DIRN_DOWN || currentDirection == DIRN_STOP) && orders[1][currentFloor] == 1){
		return 1;
	}

	return 0;
}

void setOrder(elev_button_type_t btype, int floor, int on){
    //Put specified order in order table
    
    orders[btype][floor] = on;
    updateLights();
}

void resetOrders(){
    //Clear order table

	for (int t = 0; t < 3; t++){
		for (int f = 0; f < N_FLOORS; f++){
			orders[t][f] = 0;
		}
	}
	updateLights();
}

//Interface for current direction, location when between floors, and last known floor.

int getCurrentFloor(){
    return currentFloor;
}
void setCurrentFloor(int floor){
    currentFloor = floor;
}
void setBetweenFloors(int floor){
    betweenFloors = floor;
}
int getBetweenFloors(){
    return betweenFloors;
}
