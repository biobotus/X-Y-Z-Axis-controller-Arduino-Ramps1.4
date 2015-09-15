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
	long homingSpeed;
	const long pintStep;
	const long pinDir;
	const long pinEnable;
	const long pinLimitSwitch;
	long maxPositionSubsteps;
	const long mm_per_step; //Saved in micrometers onto an integer -> 0.127mm is save as 127 
	const long number_of_substep;
	int towardHomeDirection;
	int leavingHomedirection;

	void setDesiredPositionSubsteps(String desiredPos)
	{
		int desiredPositionInt = desiredPos.toInt();
		float desiredPositionFloat = (float)desiredPositionInt / 10;
		double millimeterPerStep = (double)mm_per_step / 1000;
		double InvertedMillimeterPerStep = (double)(1 / millimeterPerStep);
		double desiredPositionSubstepsVariable = (double)(desiredPositionFloat*InvertedMillimeterPerStep*number_of_substep);
		desiredPositionSubsteps = desiredPositionSubstepsVariable;

		/*
		Serial.println(F("Text to parse : "));
		Serial.println(desiredPos);
		
		Serial.println(F("Desired position converted to int : "));
		Serial.println(desiredPositionInt);
		
		Serial.println(F("Desired position converted to float : "));
		Serial.println(desiredPositionFloat);
		
		Serial.println(F("Micrometer per step value : "));
		Serial.println(millimeterPerStep);
		
		Serial.println(F("Inverted mm_per_step : "));
		Serial.println(InvertedMillimeterPerStep);
		
		Serial.println(F("Position substeps value: "));
		Serial.println(desiredPositionSubstepsVariable);*/		
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