#include "MotorStructure.h"


Motor::Motor(int pin_step, int pin_dir, int pin_enable, int pin_limitSwitch, int maxPosition, int mmPerStep, int microstepsPerStep, int maxSpeed, int homingSpeedDelay, bool invertedDirection)
{
	_pinStep = pin_step;
	_pinDir = pin_dir;
	_pinEnable = pin_enable;
	_pinLimitSwitch = pin_limitSwitch;
	_maxPosition = maxPosition;
	_microstepsPerStep = microstepsPerStep;	
	_mm_per_step = (float)mmPerStep/1000;	// Parameter passed by user is set in mm*1000 to keep it as an integer, we then convert it to float for internal use
	_homingSpeedDelay = homingSpeedDelay;
	_invertedDirection = invertedDirection;

	// Set direction pin inversion
	if (_invertedDirection == true)
	{
		_leavingHomePinState = LOW;
		_towardHomePinState = HIGH;
	}
	else
	{
		_leavingHomePinState = HIGH;
		_towardHomePinState = LOW;
	}
}

void Motor::setSpeedAndDelay(unsigned int desiredSpeed)
{
	if (desiredSpeed > _maxSpeed)
	{
		_desiredCruisingSpeed = _maxSpeed;
	}
	else if(desiredSpeed > 0)
	{
		_desiredCruisingSpeed = desiredSpeed;
	}
	else
	{
		// Do nothing, keep previous speed
	}
	
	// Set the delay (in microseconds) according to the set speed
	int delay = 0;
	delay = (1 / (_microstepsPerStep * _desiredCruisingSpeed / _mm_per_step)) * 1000000;

	_cruisingSpeedDelay = delay;
}

// Desired position in mm coming from serial port, acceleration ramp in delay (microseconds), and speed set in mm/s
void Motor::moveTrapezoidally(String desiredPosition, float delayVariation, int cruiseSpeed)
{
	long microStepCounter = 0;	// Will hold the step count to know when to change delay to affect speed accordingly
	long decelStepCounter = 0;	// Will hold the step count related to deceleration only
	int rampDivider = 16;
	// Clamping the distance to travel
	setDesiredPositionSubsteps(desiredPosition);

	int movementDirection = ((_desiredPositionSubsteps - _currentPositionSubsteps) > 0 ? _leavingHomePinState : _towardHomePinState);
	
	digitalWrite(_pinDir, movementDirection);  // Set direction polarity
	digitalWrite(_pinEnable, LOW);	// enable driver	

	// compute steps to travel
	long stepsToTravel = abs(_desiredPositionSubsteps - _currentPositionSubsteps);

	// Clamps cruise speed and compute delay in microseconds
	setSpeedAndDelay(cruiseSpeed);	// Minimal delay to reach for obtaining desired travel speed

	/*
	Setting up the speed and acceleration profile
	This is the speed profile, from which are calculated delays

						cruisePoint       decelPoint
						  |					|
	maxSpeed	 ---	   _ _ _ _ _ _ _ _ _	-- Minimal delay applied to microsteps
						 /|				    |\
					    / |				    | \
	initialSpeed ---   /  |				    |  \	--- Final Speed (Delay ~ 1000 to simulate 0 mm/s)
		
					  accel     cruise       decel
	*/


	Serial.println("Steps to travel: ");
	Serial.println(stepsToTravel);
	Serial.println("Variation: ");
	Serial.println(delayVariation);
	Serial.println("Desired position: ");
	Serial.println(_desiredPositionSubsteps);

	long cruisePoint = (float)(_initialDelay-_cruisingSpeedDelay)/(delayVariation/ rampDivider);	//(float)0.1*stepsToTravel;

	long decelPoint = stepsToTravel - cruisePoint;

	int currentDelay = _initialDelay;	

	Serial.println("Direction: ");
	Serial.println(movementDirection);
	Serial.println("Initial delay: ");
	Serial.println(_initialDelay);
	Serial.println("cruising speed : ");
	Serial.println(cruiseSpeed);
	Serial.println("cruising speed delay: ");
	Serial.println(_cruisingSpeedDelay);

	Serial.println("Cruise point: ");
	Serial.println(cruisePoint);
	Serial.println("Deceleration point: ");
	Serial.println(decelPoint);


	while (microStepCounter < stepsToTravel)
	{
		digitalWrite(_pinStep, HIGH);
		delayMicroseconds(2);
		digitalWrite(_pinStep, LOW);		

		if (microStepCounter >= 0 && microStepCounter < cruisePoint)
		{
			if (microStepCounter % rampDivider == 0)
			{
				currentDelay = currentDelay - delayVariation;
				delayMicroseconds(currentDelay);
			}
		}
		else if (microStepCounter >= cruisePoint && microStepCounter < decelPoint)
		{
			delayMicroseconds(_cruisingSpeedDelay);
		}
		else
		{
			if (microStepCounter%rampDivider == 0)
			{
				currentDelay = currentDelay + delayVariation;
				delayMicroseconds(currentDelay);
			}			
		}

		if (movementDirection == _leavingHomePinState)
		{
			_currentPositionSubsteps += 1;
		}
		else
		{
			_currentPositionSubsteps = _currentPositionSubsteps - 1;
		}
		microStepCounter += 1;

	}

	if (_currentPositionSubsteps == _desiredPositionSubsteps)
	{
		Serial.println("Destination reached");
	}
	digitalWrite(_pinEnable, HIGH);	// Disable driver
}

void Motor::stop(void)
{
	// disables "enable pin"
	digitalWrite(_pinEnable, HIGH);
}

void Motor::goHome(void)
{
	int direction = 0;
	if (_invertedDirection == false)
	{
		direction = LOW;
	}
	else
	{
		direction = HIGH;
	}

	digitalWrite(_pinEnable, LOW);// enable

	while (digitalRead(_pinLimitSwitch))
	{
		digitalWrite(_pinDir, direction);  // Towards home direction
		digitalWrite(_pinStep, HIGH);
		delayMicroseconds(2);
		digitalWrite(_pinStep, LOW);
		delayMicroseconds(_homingSpeedDelay);
	}
	for (int i = 0; i < 2000; i++)	// Will move back 3000 micro steps (1/16 of step)
	{
		digitalWrite(_pinDir, !direction);  // Away from home direction
		digitalWrite(_pinStep, HIGH);
		delayMicroseconds(2);
		digitalWrite(_pinStep, LOW);
		delayMicroseconds(4 * _homingSpeedDelay);
	}
	while (digitalRead(_pinLimitSwitch))
	{
		digitalWrite(_pinDir, direction);  // Towards home direction
		digitalWrite(_pinStep, HIGH);
		delayMicroseconds(2);
		digitalWrite(_pinStep, LOW);
		delayMicroseconds(4 * _homingSpeedDelay);
	}

	digitalWrite(_pinEnable, HIGH);// enable
	// Set position to 0
	_currentPositionSubsteps = 0;
}

void Motor::moveTo(String desiredPosition, unsigned int speed)
{
	int movementDirection = ((_desiredPositionSubsteps - _currentPositionSubsteps) > 0 ? _leavingHomePinState : _towardHomePinState);
	
	digitalWrite(_pinEnable, LOW);	// enable driver
	digitalWrite(_pinDir, movementDirection);  // Set direction polarity

	long stepCounter = 0;	// Will hold the step count to know when to change delay to affect speed accordingly
	long decelStepCounter = 0;	// Will hold the step count related to deceleration only

	// Clamping the distance to travel
	setDesiredPositionSubsteps(desiredPosition);

	// compute steps to travel
	long stepsToTravel = abs(_desiredPositionSubsteps - _currentPositionSubsteps);

	// Define speed to use
	setSpeedAndDelay(speed);

	// Compute delay at cruise speed (defined by user)
	float cruiseSpeedDelay = (float)(((float)(_mm_per_step/1000)*_microstepsPerStep)/ _desiredCruisingSpeed);	// This is the minimal delay between steps
	Serial.println("cruise speed delay: ");
	Serial.println(cruiseSpeedDelay);
	Serial.println("current speed ");
	Serial.println(_desiredCruisingSpeed);
	Serial.println("mm per step: ");
	Serial.println(_mm_per_step);
	Serial.println("microsteps per step ");
	Serial.println(_microstepsPerStep);
	Serial.println("mm per step/100 ");
	Serial.println((float)((float)(_mm_per_step / 1000)*_microstepsPerStep));

	/*
		Setting up the speed and acceleration profile
		The first 10% of the trajectory will have a maximal acceleration while the last 20% will decelerate down to almost 0mm/s before stopping
	
						cruisePoint       decelPoint
	maxSpeedDelay ---	  _ _ _ _ _ _ _ _ _ 
						/|				  | \
					   / |				  |   \
	initialDelay ---  /	 |				  |     \
						 |				  |		  \	--- Final Delay (which will be greater than starting delay to further slow down
					 accel   cruise         decel
	*/
	long cruisePoint = (float)0.05*stepsToTravel;
	long decelPoint = (float)0.8*stepsToTravel;
	float accelIncrement = (_initialDelay - cruiseSpeedDelay) / (cruisePoint);	//(100000 * diff) / divider;
	float decelerationIncrement = (_finalDelay - cruiseSpeedDelay) / (decelPoint);

	while (stepCounter <= stepsToTravel)
	{
		digitalWrite(_pinStep, HIGH);
		delayMicroseconds(4);
		digitalWrite(_pinStep, LOW);

		if (stepCounter >= 0 && stepCounter < cruisePoint)
		{
			delayMicroseconds(_initialDelay - 1);
		}
		else if (stepCounter >= cruisePoint && stepCounter < decelPoint)
		{
			delayMicroseconds(cruiseSpeedDelay);
		}
		else
		{
			delayMicroseconds((int)(_cruisingSpeedDelay + (decelerationIncrement*decelStepCounter)));
			decelStepCounter += 1;
		}
		stepCounter += 1;
	}

	digitalWrite(_pinEnable, HIGH);	// Disable driver

	/*while (stepCounter <= stepsToTravel)
	{
		digitalWrite(_pinStep, HIGH);
		delayMicroseconds(4);
		digitalWrite(_pinStep, LOW);

		if (stepCounter>=0 && stepCounter < cruisePoint)
		{
			delayMicroseconds((int)(_initialDelay - (accelIncrement*stepCounter)));
		}
		else if (stepCounter >= cruisePoint && stepCounter < decelPoint)
		{
			delayMicroseconds(cruiseSpeedDelay);
		}
		else
		{
			delayMicroseconds((int)(_cruisingSpeedDelay + (decelerationIncrement*decelStepCounter)));
			decelStepCounter += 1;
		}
		stepCounter += 1;
	}*/
}

void Motor::setMaxPositionSubsteps(String maximum)
{
	int maximumInint = maximum.toInt();
	float maximumFloat = (float)maximumInint / 10;
	double millimeterPerStep = (double)_mm_per_step;	//TODO verify if it works / 1000;
	double InvertedMillimeterPerStep = (double)(1 / millimeterPerStep);
	double maximumSubsteps = (double)(maximumFloat*InvertedMillimeterPerStep*_microstepsPerStep);
	/*_maxPositionSubsteps = maximumSubsteps;*/
}

void Motor::moveToWithTriangularSpeedProfile(String desiredPosition)
{
	int initialDelay = 200;	// Default value
	int maxSpeedDelay = 100;	// Default value

	// Set desired position
	setDesiredPositionSubsteps(desiredPosition);

	int movementDirection = ((_desiredPositionSubsteps - _currentPositionSubsteps) > 0 ? _leavingHomePinState : _towardHomePinState);

	long stepCounter = 0;	// Will hold the step count to know when to change delay to affect speed accordingly
	long stepsToTravel = abs(_desiredPositionSubsteps - _currentPositionSubsteps);

	// Setting delays depending on travel distance (if less than 100mm, make a slower move)
	if (stepsToTravel <= 12600)
	{
		initialDelay = 300;
		maxSpeedDelay = 100;	// Smaller is faster
	}
	else
	{
		initialDelay = 80;
		maxSpeedDelay = 16;	// Smaller is faster
	}

	long diff = initialDelay - maxSpeedDelay;
	long divider = (long)stepsToTravel / 2;
	float increment = (100000 * diff) / divider;

	//long increment = long(100000*((initialDelay - maxSpeedDelay) / (stepsToTravel/2)));
	bool firstHalf = true;
	bool plateau = false;
	bool decelerationPhase = false;

	/*Serial.print("Desired position in substeps: ");
	Serial.println(_desiredPositionSubsteps);
	Serial.print("Steps to travel: ");
	Serial.println(stepsToTravel);
	Serial.print("Diff: ");
	Serial.println(diff);
	Serial.print("Divider: ");
	Serial.println(divider);
	Serial.print("Increment: ");
	Serial.println(increment);
	Serial.println("Current position : ");
	Serial.print(_currentPositionSubsteps);
	Serial.println("Desired position : ");
	Serial.print(_desiredPositionSubsteps);
	Serial.println();*/

	_lastPositionSubsteps = _currentPositionSubsteps;
	digitalWrite(_pinEnable, LOW);// enable
	digitalWrite(_pinDir, movementDirection);  // Set direction

	while (_currentPositionSubsteps != _desiredPositionSubsteps)
	{
		digitalWrite(_pinStep, HIGH);
		delayMicroseconds(4);
		digitalWrite(_pinStep, LOW);

		// Calculating delay according to required speed (really approximate and pretty hardcoded method)
		if (firstHalf == true)//stepCounter <= stepsToTravel / 2)
		{
			int delayToSend = int(initialDelay - ((increment / 100000)*stepCounter));
			delayMicroseconds(delayToSend);

			// Verify if we have passed halfway
			if (stepCounter >= (stepsToTravel / 2))
			{
				stepCounter = 0;
				firstHalf = false;
			}
		}
		else
		{
			delayMicroseconds(int(maxSpeedDelay + increment / 100000 * stepCounter));
		}

		if (movementDirection == _leavingHomePinState)
		{
			_currentPositionSubsteps += 1;
		}
		else
		{
			_currentPositionSubsteps = _currentPositionSubsteps - 1;
		}
		stepCounter += 1;
	}
	digitalWrite(_pinEnable, HIGH);// disable
}

// Specific to X axis to slow it down on small travels to avoid problems of acceleration
void Motor::xAxisMoveWithTriangularSpeed(String desiredPosition)
{
	int initialDelay = 200;
	int maxSpeedDelay = 100;
	// Set desired position
	setDesiredPositionSubsteps(desiredPosition);

	int movementDirection = ((_desiredPositionSubsteps - _currentPositionSubsteps) > 0 ? _leavingHomePinState : _towardHomePinState);

	long stepCounter = 0;	// Will hold the step count to know when to change delay to affect speed accordingly
	long stepsToTravel = abs(_desiredPositionSubsteps - _currentPositionSubsteps);
	
	if (stepsToTravel <= 12600)
	{
		initialDelay = 300;
		maxSpeedDelay = 100;	// Smaller is faster
	}
	else
	{
		initialDelay = 80;
		maxSpeedDelay = 20;	// Smaller is faster
	}
	
	long diff = initialDelay - maxSpeedDelay;
	long divider = (long)stepsToTravel / 2;
	float increment = (100000 * diff) / divider;

	//long increment = long(100000*((initialDelay - maxSpeedDelay) / (stepsToTravel/2)));
	bool firstHalf = true;
	bool plateau = false;
	bool decelerationPhase = false;

	Serial.print("Steps to travel: ");
	Serial.println(stepsToTravel);

	/*Serial.print("Desired position in substeps: ");
	Serial.println(_desiredPositionSubsteps);
	Serial.print("Steps to travel: ");
	Serial.println(stepsToTravel);
	Serial.print("Diff: ");
	Serial.println(diff);
	Serial.print("Divider: ");
	Serial.println(divider);
	Serial.print("Increment: ");
	Serial.println(increment);
	Serial.println("Current position : ");
	Serial.print(_currentPositionSubsteps);
	Serial.println("Desired position : ");
	Serial.print(_desiredPositionSubsteps);
	Serial.println();*/

	_lastPositionSubsteps = _currentPositionSubsteps;
	digitalWrite(_pinEnable, LOW);// enable
	digitalWrite(_pinDir, movementDirection);  // Set direction

	while (_currentPositionSubsteps != _desiredPositionSubsteps)
	{
		digitalWrite(_pinStep, HIGH);
		delayMicroseconds(4);
		digitalWrite(_pinStep, LOW);

		// Calculating delay according to required speed (really approximate and pretty hardcoded method)
		if (firstHalf == true)//stepCounter <= stepsToTravel / 2)
		{
			int delayToSend = int(initialDelay - ((increment / 100000)*stepCounter));
			delayMicroseconds(delayToSend);

			// Verify if we have passed halfway
			if (stepCounter >= (stepsToTravel / 2))
			{
				stepCounter = 0;
				firstHalf = false;
			}
		}
		else
		{
			delayMicroseconds(int(maxSpeedDelay + increment / 100000 * stepCounter));
		}

		if (movementDirection == _leavingHomePinState)
		{
			_currentPositionSubsteps += 1;
		}
		else
		{
			_currentPositionSubsteps = _currentPositionSubsteps - 1;
		}
		stepCounter += 1;
	}
	Serial.println("Disabling motor");
	digitalWrite(_pinEnable, HIGH);// disable
}
void Motor::setDesiredPositionSubsteps(String desiredPos)
{
	char temp[32] = { 0 };
	desiredPos.toCharArray(temp, sizeof(temp), 0);

	long desiredPositionLong = atol(temp);

	// clamping the value:
	if (desiredPositionLong > _maxPosition)
	{
		desiredPositionLong = _maxPosition;
	}

	float desiredPositionFloat = (float)desiredPositionLong / 10;	// Value input in mm*10, so we need to divide to get the real mm measure
	float InvertedMillimeterPerStep = (float)(1 / _mm_per_step);
	float desiredPositionSubstepsFloat = (float)(desiredPositionFloat*InvertedMillimeterPerStep*_microstepsPerStep);

	// Setting the object's property
	_desiredPositionSubsteps = desiredPositionSubstepsFloat;

	////TODO remove from code when done testing
#ifdef VERBOSE
	Serial.println("Desired position in mmm (float):");
	Serial.println(desiredPositionFloat);
	Serial.println("millimeterPerStep: :");
	Serial.println(_mm_per_step*1000);
	Serial.println("1/mmPerStep:");
	Serial.println(InvertedMillimeterPerStep);
	Serial.println("Desired position substeps float:");
	Serial.println(desiredPositionSubstepsFloat);
	Serial.println("Desired position substeps:");
	Serial.println(_desiredPositionSubsteps);
#endif

	
}