#pragma once
//#define DEBUG_PRINT	// Define to let user see more informations about program running

#define BAUD_RATE				115200
#define MAX_LEN					128
#define FORWARD					HIGH
#define REVERSE					LOW

// Pins definition :
#define PIN_STEP_X				54
#define PIN_DIR_X				55
#define PIN_ENABLE_X			38
#define PIN_LMSWITCH_X			3
#define MAX_POSITION_X			12050		// Maximum position in mm X 10.
#define MAX_SPEED_X				180	// TODO: update this value to be representative
#define MM_PER_STEP_X			127         //0.127 (Saved in micrometers per step to fit nicely on an integer)
#define HOMING_SPEED_X			100			//step speed (delay between steps)
#define SUB_STEP_NUMBER_X		16
#define MAX_DELAY_X				150
#define MAX_RAMP_X				11250		// in delay (to be divided by 100 000)
#define CRUISE_SPEED_DELAY_X	80

#define PIN_STEP_Y				60
#define PIN_DIR_Y				61
#define PIN_ENABLE_Y			56
#define PIN_LMSWITCH_Y			14
#define MAX_POSITION_Y			7000
#define MAX_SPEED_Y				180	// TODO: update this value to be representative
#define MM_PER_STEP_Y			127         //0.127
#define HOMING_SPEED_Y			100
#define SUB_STEP_NUMBER_Y		16
#define MAX_DELAY_Y				150
#define MAX_RAMP_Y				11250		// in delay (to be divided by 100 000) 
#define CRUISE_SPEED_DELAY_Y	40

#define PIN_STEP_Z1				46
#define PIN_DIR_Z1				48
#define PIN_ENABLE_Z1			62
#define PIN_LMSWITCH_Z1			18
#define MAX_POSITION_Z1			3500
#define MAX_SPEED_Z1			100	// TODO: update this value to be representative
#define MM_PER_STEP_Z1			40         //0.640
#define HOMING_SPEED_Z1			100
#define SUB_STEP_NUMBER_Z1		16
#define MAX_DELAY_Z1			150
#define MAX_RAMP_Z1				5000		// in delay (to be divided by 100 000) 
#define CRUISE_SPEED_DELAY_Z1	80

#define PIN_STEP_Z2				26
#define PIN_DIR_Z2				28
#define PIN_ENABLE_Z2			24
#define PIN_LMSWITCH_Z2			15
#define MAX_POSITION_Z2			3500
#define MAX_SPEED_Z2			100	// TODO: update this value to be representative
#define MM_PER_STEP_Z2			40         //0.640
#define HOMING_SPEED_Z2			100
#define SUB_STEP_NUMBER_Z2		16
#define MAX_DELAY_Z2			150
#define MAX_RAMP_Z2				5000		// in delay (to be divided by 100 000) 
#define CRUISE_SPEED_DELAY_Z2	80

#define PIN_STEP_Z3				36
#define PIN_DIR_Z3				34
#define PIN_ENABLE_Z3			30
#define PIN_LMSWITCH_Z3			2
#define MAX_POSITION_Z3			3500
#define MAX_SPEED_Z3			100	// TODO: update this value to be representative
#define MM_PER_STEP_Z3			40         //0.640  
#define HOMING_SPEED_Z3			100
#define SUB_STEP_NUMBER_Z3		16
#define MAX_DELAY_Z3			150
#define MAX_RAMP_Z3				5000		// in delay (to be divided by 100 000) 
#define CRUISE_SPEED_DELAY_Z3	80

#define UNDEFINED	0

bool commandReceivedFlag = 0;
