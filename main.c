#include "elev.h"
#include "fsm.h"
#include "timer.h"

#include <stdio.h>

// Elevator control program, by Areg Babayan and Bernt Johan Damslora
// When variables t and f is seen in the code, t is used as an iterator of button type, and f as an iterator of floors
// Files:
//    main.c    Event listener. Polls all inputs and timer and calls FSM when anything interesting happens.
//    fsm.c     Event handler and state machine.
//    retn.c    Direction manager. When its decideDirection is called, returns appropriate direction
//              as long as the situation of the elevator and order table have been set.
//				The decideDirection function can not be called too often, and will not "flip-flop".
//    timer.c   Timer. Counts 3 seconds.

int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    elev_set_motor_direction(DIRN_STOP);
    
    int stopDepressed = 0;
    
    evInit(); // Initialise elevator and state machine
    printf("Ready.\n");
    
    while (1) {
    
        //Handle stop button
        if (elev_get_stop_signal()){
            if  (!stopDepressed){
                stopDepressed = 1;
                evStop();
            }
        } else {
            if  (stopDepressed){
                stopDepressed = 0;
                evUnStop();
            }
        }

        // Handle floor arrival
        int floor = elev_get_floor_sensor_signal();
        if (floor > -1){
            evFloorReached(floor);
            elev_set_floor_indicator(floor);
        }
        
        //Handle buttons
        for (int btype = 0; btype < 3; btype++) {
            for (int floor = 0; floor < N_FLOORS; floor++){
                if (elev_get_button_signal(btype,floor)){
                    evBtnSignal(btype,floor);
                }
            }
        }
        
        //Handle timeout
        if (isTimeout()){
            evTimeout();
        }
    }

    return 0;
}
