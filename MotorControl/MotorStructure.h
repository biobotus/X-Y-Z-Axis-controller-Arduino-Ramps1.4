#ifndef MotorStructure_h
#define MotorStructure_h
#include <Arduino.h> 

class Motor
{
public:
	// The positions are saved in substeps.
	// Ex. The user wants to move the robot X axis 1264.5mm.
	// This value is sent on an integer as 12645, the number is to be divided by 10.
	// To translate this into substeps, we must do the following :
	// float desiredPosition = (float)SerialValueReceived/10.
	// int desiredPositionSubsteps = (int)((1/desiredPosition)*(mm_per_step/1000))/number_of_substep;

	int _pinStep;
	int _pinDir;
	int _pinEnable;
	int _pinLimitSwitch;	
	long _maxPosition;	// In mm*10
	long _mm_per_step; //Saved in micrometers onto an integer -> 0.127mm is saved as 127 
	bool _invertedDirection;	// Tells that the direction must be inverted to work properly
	unsigned int _maxSpeed = 180;	// In mm/sec
	unsigned int _currentSpeed = 120;	// In mm/sec
	unsigned int _homingSpeedDelay = 80;
	long _currentPositionSubsteps = 0;
	long _lastPositionSubsteps = 0; // Saved in substeps -> Ex. Moving 1264.5mm (sent as 12645 on an integer by the user) -> lastPosition = 1264	
	long _desiredPositionSubsteps = 0;
	long _maxPositionSubsteps = 0;
	long _microstepsPerStep = 16;
	bool _towardHomePinState = LOW;
	bool _leavingHomePinState = HIGH;
	long _cruisingSpeedDelay;
	const int _initialDelay = 150;	// Delay in microseconds between each substep when starting to move
	const int _finalDelay = 1000;	// Delay in microseconds between each substep when stopping to move

	// Functions
	void setDesiredPositionSubsteps(String desiredPos);

	void setMaxPositionSubsteps(String maximum);

	void moveMotorWithKindOfTrajectory(void);

	void goHome(void);

	int moveMotor(void);

	int moveMotorTrapeizodally(void);

	int moveMotorTrapezoidally(void);

	void setSpeed(unsigned int speed);

	void testMaxSpeed(String desiredPosition, int minimalDelay);

	void moveTo(String desiredPosition, unsigned int speed);	// position in mm*10 --> for a 23.4mm movement, input 234, speed in mm/s directly (integer only)
	
	Motor(int pin_step, int pin_dir, int pin_enable, int pin_limitSwitch, long maxPosition, long mmPerStep, long microstepsPerStep, bool invertedDirection, int maxSpeed);
	
	
};
// the #include statment and code go here...

#endif