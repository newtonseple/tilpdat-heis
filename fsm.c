#include "elev.h"
#include "fsm.h"
#include "timer.h"
#include "retn.h"

static state currentState = stInit;

static void setState(state s){
    //Handle state changes. See state diagram for descriptions

    currentState = s;
    switch (currentState){
        case stActive:
            elev_set_motor_direction(decideDirection());
            break;
        case stLanding:
            elev_set_motor_direction(DIRN_STOP);
            elev_set_door_open_lamp(1);
            startTimer();
            
            //Remove orders, and commands, for current floor.
            setOrder(BUTTON_CALL_UP, elev_get_floor_sensor_signal(), 0);
            setOrder(BUTTON_CALL_DOWN, elev_get_floor_sensor_signal(), 0);
            setOrder(BUTTON_COMMAND, elev_get_floor_sensor_signal(), 0);
            break;
        case stEStop:
            elev_set_motor_direction(0);
            elev_set_stop_lamp(1);
            if (elev_get_floor_sensor_signal() > -1) {
                elev_set_door_open_lamp(1);
            }
            else {
                if (getBetweenFloors() == -1){
                   setBetweenFloors(getCurrentFloor() + ((decideDirection() == 1) ? 0 : -1)); //Set floor below current position
                }
            }
            resetOrders();
            break;
        case stPEStop:
            elev_set_stop_lamp(0);
    }
}

void evInit(){
    // Initialisation. State doesn't really matter here, since it's only called once.

    elev_set_door_open_lamp(0);
    elev_set_motor_direction(DIRN_UP);
    resetOrders();
}

void evBtnSignal(elev_button_type_t btype, int floor){
    switch (currentState){
        case stActive:
            setOrder(btype, floor, 1);
            elev_set_motor_direction(decideDirection());
        case stPEStop:
            elev_set_door_open_lamp(0);
            setOrder(btype, floor, 1);
            setState(stActive);
        case stLanding:
            if (floor == getCurrentFloor()){
                startTimer();
            } else {
                setOrder(btype, floor, 1);
            }
    }
}

void evStop(){
    switch (currentState){
        case stPEStop:
        case stActive:
        case stLanding:
            setState(stEStop);
    }
}

void evUnStop(){
    switch (currentState){
        case stEStop:
            setState(stPEStop);
    }
}

void evFloorReached(int floor){
    setBetweenFloors(-1);
    setCurrentFloor(floor);
    
    switch (currentState){
        case stInit:
            elev_set_motor_direction(DIRN_STOP);
            setState(stActive);
            break;
        case stActive:
            if (isOrdered()){
                setState(stLanding);
            } else {
                elev_set_motor_direction(decideDirection());
            }
            
            
    }
}

void evTimeout(){
    switch (currentState){
        case stLanding:
            elev_set_door_open_lamp(0);
            setState(stActive);
    }
}

state getState(){
    return currentState;
}
