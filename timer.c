#include <time.h>

static time_t startTime;
static time_t currentTime;

static int timer_running = 0;

void startTimer(){
	startTime = time(NULL);
	timer_running = 1;
}


int isTimeout(){ 
    // Return 1 if three seconds have passed, else 0.
        
	currentTime = time(NULL);
    
	double secondsElapsed = difftime(currentTime, startTime);
	if (secondsElapsed >= 3.0 && timer_running){
	    timer_running = 0;
		return 1;
	}

	return 0;
}
