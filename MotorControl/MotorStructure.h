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

	// Variables set by the constructor
	int _pinStep;
	int _pinDir;
	int _pinEnable;
	int _pinLimitSwitch;	
	long _maxPosition;	// In mm*10
	float _mm_per_step; //Saved in micrometers onto an integer -> 0.127mm is saved as 127 
	int _microstepsPerStep = 16;
	unsigned int _maxSpeed = 180;	// In mm/sec
	unsigned int _homingSpeedDelay = 80;
	bool _invertedDirection;	// Tells that the direction must be inverted to work properly
	bool _towardHomePinState = LOW;
	bool _leavingHomePinState = HIGH;

	// Internal variables
	unsigned int _desiredCruisingSpeed = 120;	// In mm/sec
	long _currentPositionSubsteps = 0;	// Keeps tracks of the current position in substeps
	long _lastPositionSubsteps = 0;		// Saved in substeps -> Ex. Moving 1264.5mm (sent as 12645 on an integer by the user) -> lastPosition = 1264	
	long _desiredPositionSubsteps = 0;	// Desired position, in substeps, will change according to number of microsteps per full step for each motor
	int _cruisingSpeedDelay;
	const int _initialDelay = 1000;	// Delay in microseconds between each substep when starting to move
	const int _finalDelay = 1000;	// Delay in microseconds between each substep when stopping to move

	// Functions
	void setDesiredPositionSubsteps(String desiredPos);

	void setMaxPositionSubsteps(String maximum);

	void goHome(void);

	int moveMotor(void);

	int moveMotorTrapezoidally(void);

	void setSpeedAndDelay(unsigned int speed);	// Clamps speed and converts it in mm/s to a delay in microseconds and stores it in the "_cruisingSpeedDelay" variable

	void stop(void);

	void moveToWithTriangularSpeedProfile(String desiredPosition);

	void xAxisMoveWithTriangularSpeed(String desiredPosition);

	void moveTrapezoidally(String desiredPosition, float delayVariation, int cruiseSpeed); // delayVariation in microseconds per microstep, cruiseSpeed in mm/s

	void moveTo(String desiredPosition, unsigned int speed);	// position in mm*10 --> for a 23.4mm movement, input 234, speed in mm/s directly (integer only)
	
	Motor(int pin_step, int pin_dir, int pin_enable, int pin_limitSwitch, int maxPosition, int mmPerStep, int microstepsPerStep, int maxSpeed, int homingSpeedDelay, bool invertedDirection);
	
	
};
// the #include statment and code go here...

#endif