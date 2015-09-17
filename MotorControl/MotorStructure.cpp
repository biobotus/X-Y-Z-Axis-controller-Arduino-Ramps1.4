#include "MotorStructure.h"


Motor::Motor(int pin_step, int pin_dir, int pin_enable, int pin_limitSwitch, long maxPosition, long mmPerStep, long microstepsPerStep, bool invertedDirection, int maxSpeed)
{
	_pinStep = pin_step;
	_pinDir = pin_dir;
	_pinEnable = pin_enable;
	_pinLimitSwitch = pin_limitSwitch;
	_maxPosition = maxPosition;
	_microstepsPerStep = microstepsPerStep;
	_invertedDirection = invertedDirection;
	_mm_per_step = mmPerStep;

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

void Motor::setSpeed(unsigned int desiredSpeed)
{
	if (desiredSpeed > _maxSpeed)
	{
		_currentSpeed = _maxSpeed;
	}
	else if(desiredSpeed > 0)
	{
		_currentSpeed = desiredSpeed;
	}
	else
	{
		// Do nothing, keep previous speed
	}
}

void Motor::testMaxSpeed(String desiredPosition, int speed)
{
	// Clamping the distance to travel
	setDesiredPositionSubsteps(desiredPosition);

	int movementDirection = ((_desiredPositionSubsteps - _currentPositionSubsteps) > 0 ? _leavingHomePinState : _towardHomePinState);

	digitalWrite(_pinEnable, LOW);	// enable driver
	digitalWrite(_pinDir, movementDirection);  // Set direction polarity

	long stepCounter = 0;	// Will hold the step count to know when to change delay to affect speed accordingly
	long decelStepCounter = 0;	// Will hold the step count related to deceleration only

	// compute steps to travel
	long stepsToTravel = abs(_desiredPositionSubsteps - _currentPositionSubsteps);

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
	long cruisePoint = (float)0.1*stepsToTravel;
	long decelPoint = (float)0.9*stepsToTravel;
	float accelIncrement = (float)(_initialDelay - speed) / (cruisePoint);	//(100000 * diff) / divider;
	float decelerationIncrement = (float)(_finalDelay - speed) / (decelPoint);


	Serial.println("Direction: ");
	Serial.println(movementDirection);
	Serial.println("Steps to travel: ");
	Serial.println(stepsToTravel);
	Serial.println("Initial delay: ");
	Serial.println(_initialDelay);
	Serial.println("minimal delay: ");
	Serial.println(speed);

	Serial.println("decelerationIncrement: ");
	Serial.println(decelerationIncrement);
	Serial.println("Cruise point: ");
	Serial.println(cruisePoint);
	Serial.println("Deceleration point: ");
	Serial.println(decelPoint);
	Serial.println("Accel increment: ");
	Serial.println(accelIncrement);

	while (stepCounter <= stepsToTravel)
	{
		digitalWrite(_pinStep, HIGH);
		delayMicroseconds(4);
		digitalWrite(_pinStep, LOW);

		if (stepCounter >= 0 && stepCounter < cruisePoint)
		{
			delayMicroseconds((int)(_initialDelay - (accelIncrement*stepCounter)));
		}
		else if (stepCounter >= cruisePoint && stepCounter < decelPoint)
		{
			delayMicroseconds(speed);
		}
		else
		{
			delayMicroseconds((int)(speed + (decelerationIncrement*decelStepCounter)));
			decelStepCounter += 1;
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
		/*if (stepCounter % 200 == 0)
		{
			Serial.println("Position: ");
			Serial.println(_currentPositionSubsteps);
		}*/
	}
}

void Motor::goHome(void)
{
	while (digitalRead(_pinLimitSwitch))
	{
		digitalWrite(_pinEnable, LOW);// enable
		digitalWrite(_pinDir, LOW);  // Towards home direction
		digitalWrite(_pinStep, HIGH);
		delayMicroseconds(2);
		digitalWrite(_pinStep, LOW);
		delayMicroseconds(_homingSpeedDelay);
	}
	for (int i = 0; i < 3000; i++)
	{
		digitalWrite(_pinEnable, LOW);// enable
		digitalWrite(_pinDir, HIGH);  // Away from home direction
		digitalWrite(_pinStep, HIGH);
		delayMicroseconds(2);
		digitalWrite(_pinStep, LOW);
		delayMicroseconds(2 * _homingSpeedDelay);
	}
	while (digitalRead(_pinLimitSwitch))
	{
		digitalWrite(_pinEnable, LOW);// enable
		digitalWrite(_pinDir, LOW);  // Towards home direction
		digitalWrite(_pinStep, HIGH);
		delayMicroseconds(2);
		digitalWrite(_pinStep, LOW);
		delayMicroseconds(2 * _homingSpeedDelay);
	}
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
	setSpeed(speed);

	// Compute delay at cruise speed (defined by user)
	float cruiseSpeedDelay = (float)(((float)(_mm_per_step/100)*_microstepsPerStep)/ _currentSpeed);	// This is the minimal delay between steps
	Serial.println("cruise speed delay: ");
	Serial.println(cruiseSpeedDelay);
	Serial.println("current speed ");
	Serial.println(_currentSpeed);
	Serial.println("mm per step: ");
	Serial.println(_mm_per_step);
	Serial.println("microsteps per step ");
	Serial.println(_microstepsPerStep);
	Serial.println("mm per step/100 ");
	Serial.println((float)((float)(_mm_per_step / 100)*_microstepsPerStep));

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
	double millimeterPerStep = (double)_mm_per_step / 1000;
	double InvertedMillimeterPerStep = (double)(1 / millimeterPerStep);
	double maximumSubsteps = (double)(maximumFloat*InvertedMillimeterPerStep*_microstepsPerStep);
	_maxPositionSubsteps = maximumSubsteps;
}

void Motor::moveMotorWithKindOfTrajectory()
{
	int movementDirection = ((_desiredPositionSubsteps - _currentPositionSubsteps) > 0 ? _leavingHomePinState : _towardHomePinState);

	long stepCounter = 0;	// Will hold the step count to know when to change delay to affect speed accordingly
	long stepsToTravel = abs(_desiredPositionSubsteps - _currentPositionSubsteps);
	int initialDelay = 20;
	int maxSpeedDelay = 2;	// Smaller is faster
	long diff = initialDelay - maxSpeedDelay;
	long divider = (long)stepsToTravel / 2;
	//long increment = (long)(((100000*diff) / divider));
	float increment = (100000 * diff) / divider;

	//long increment = long(100000*((initialDelay - maxSpeedDelay) / (stepsToTravel/2)));
	bool firstHalf = true;
	bool plateau = false;
	bool decelerationPhase = false;

	Serial.print("Desired position: ");
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
	Serial.println();

	_lastPositionSubsteps = _currentPositionSubsteps;
	digitalWrite(_pinEnable, LOW);// enable
	digitalWrite(_pinDir, movementDirection);  // Set direction

	while (_currentPositionSubsteps != _desiredPositionSubsteps && _currentPositionSubsteps < _maxPositionSubsteps)
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

int Motor::moveMotor()
{
#ifdef DEBUG_PRINT
	Serial.println(F("Desired substeps to do : "));
	Serial.println(motorToMove->desiredPositionSubsteps);
	Serial.print(F("Current position (in substeps): "));
	Serial.println(motorToMove->currentPositionSubsteps);
#endif	

	int movementDirection = ((_desiredPositionSubsteps - _currentPositionSubsteps) > 0 ? _leavingHomePinState : _towardHomePinState);
	_lastPositionSubsteps = _currentPositionSubsteps;
	digitalWrite(_pinEnable, LOW);// enable
	digitalWrite(_pinDir, movementDirection);  // Set direction

	while (_currentPositionSubsteps != _desiredPositionSubsteps && _currentPositionSubsteps < _maxPositionSubsteps)
	{
		digitalWrite(_pinStep, HIGH);
		delayMicroseconds(20);
		digitalWrite(_pinStep, LOW);
		//delayMicroseconds(motorToMove->initialHomingSpeed);

		delayMicroseconds(30);//200);

		if (movementDirection == _leavingHomePinState)
		{
			_currentPositionSubsteps += 1;
		}
		else
		{
			_currentPositionSubsteps = _currentPositionSubsteps - 1;
		}
	}
	digitalWrite(_pinEnable, HIGH);// disable
	return 1;
}


int Motor::moveMotorTrapezoidally()
{
	int movementDirection = ((_desiredPositionSubsteps - _currentPositionSubsteps) > 0 ? _leavingHomePinState : _towardHomePinState);
	_lastPositionSubsteps = _currentPositionSubsteps;
	digitalWrite(_pinEnable, LOW);// enable
	digitalWrite(_pinDir, movementDirection);  // Set direction

	int initialDelay = 200;
	float increment = 0.05;

	long stepsToTravel = abs(_desiredPositionSubsteps - _currentPositionSubsteps);
	long stepOfAcceleration = 0;
	long stepCounter = 0;
	int nextDelaytoSend = initialDelay;

	/*Serial.print("Steps to travel: ");
	Serial.println(stepsToTravel);
	Serial.print("Increment: ");
	Serial.println(increment);*/
	Serial.println("Current position : ");
	Serial.println(_currentPositionSubsteps);
	/*Serial.println("Desired position : ");
	Serial.println(motorToMove->desiredPositionSubsteps);
	Serial.println();*/

	//while (motorToMove->currentPositionSubsteps != motorToMove->desiredPositionSubsteps && motorToMove->currentPositionSubsteps < motorToMove->maxPositionSubsteps)
	while (stepCounter <= stepsToTravel)
	{
		digitalWrite(_pinStep, HIGH);
		delayMicroseconds(4);
		digitalWrite(_pinStep, LOW);

		if (nextDelaytoSend >= _cruisingSpeedDelay)
		{
			//Serial.println("Accelerating");			
			nextDelaytoSend = (int)(nextDelaytoSend - (float)(increment / 100000));
			delayMicroseconds(nextDelaytoSend);
			stepOfAcceleration = stepOfAcceleration + 1;
		}
		else if ((stepsToTravel - stepCounter) < stepOfAcceleration)
		{
			//Serial.println("Decelerating");			
			nextDelaytoSend = (int)(nextDelaytoSend + (float)(increment / 100000));
			delayMicroseconds(nextDelaytoSend);
		}

		if (movementDirection == _leavingHomePinState)
		{
			_currentPositionSubsteps += 1;
		}
		else
		{
			_currentPositionSubsteps = _currentPositionSubsteps - 1;
		}
		//Serial.println(nextDelaytoSend);
		stepCounter++;
	}
	Serial.println("JUST GOT OUT");
	digitalWrite(_pinEnable, HIGH);// disable
	return 1;
}

void Motor::setDesiredPositionSubsteps(String desiredPos)
{
	char temp[128] = { 0 };
	desiredPos.toCharArray(temp, sizeof(temp), 0);

	//int desiredPositionInt = desiredPos.toInt();
	long desiredPositionLong = atol(temp);

	// clamping the value:
	if (desiredPositionLong > _maxPosition)
	{
		desiredPositionLong = _maxPosition;
	}

	float desiredPositionFloat = (float)desiredPositionLong / 10;
	double millimeterPerStep = (double)_mm_per_step / 1000;
	double InvertedMillimeterPerStep = (double)(1 / millimeterPerStep);
	double desiredPositionSubstepsVariable = (double)(desiredPositionFloat*InvertedMillimeterPerStep*_microstepsPerStep);

	// Setting the object's property
	_desiredPositionSubsteps = desiredPositionSubstepsVariable;
}