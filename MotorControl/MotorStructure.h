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

	long lastPositionSubsteps; // Saved in substeps -> Ex. Moving 1264.5mm (sent as 12645 on an integer by the user) -> lastPosition = 1264
	long currentPositionSubsteps;
	long desiredPositionSubsteps;
	long speed;
	long initialHomingSpeed;
	const long pintStep;
	const long pinDir;
	const long pinEnable;
	const long pinLimitSwitch;
	long maxPositionSubsteps;
	long maxPosition;	// In mm*10
	const long mm_per_step; //Saved in micrometers onto an integer -> 0.127mm is save as 127 
	const long number_of_substep;
	int towardHomeDirection;
	int leavingHomedirection;
	int maxDelay; // Slowest speed
	int maxRamp;
	int cruisingSpeedDelay;	

	void setDesiredPositionSubsteps(String desiredPos)
	{
		char temp[128] = { 0 };
		desiredPos.toCharArray(temp, sizeof(temp), 0);

		//int desiredPositionInt = desiredPos.toInt();
		long desiredPositionLong = atol(temp);

		// clamping the value:
		if (desiredPositionLong > this->maxPosition)
		{
			desiredPositionLong = this->maxPosition;
		}

		float desiredPositionFloat = (float)desiredPositionLong / 10;
		double millimeterPerStep = (double)mm_per_step / 1000;
		double InvertedMillimeterPerStep = (double)(1 / millimeterPerStep);
		double desiredPositionSubstepsVariable = (double)(desiredPositionFloat*InvertedMillimeterPerStep*number_of_substep);

		
		// Setting the object's property
		this->desiredPositionSubsteps = desiredPositionSubstepsVariable;
	}

	void setMaxPositionSubsteps(String maximum)
	{
		int maximumInint = maximum.toInt();
		float maximumFloat = (float)maximumInint / 10;
		double millimeterPerStep = (double)mm_per_step / 1000;
		double InvertedMillimeterPerStep = (double)(1 / millimeterPerStep);
		double maximumSubsteps = (double)(maximumFloat*InvertedMillimeterPerStep*number_of_substep);
		maxPositionSubsteps = maximumSubsteps;
	}
	//Motor(int lastPos, int currentPos, int nextPos, int pin_step, int pin_dir, int pin_enable, int pin_limitSwitch);

};
// the #include statment and code go here...

#endif