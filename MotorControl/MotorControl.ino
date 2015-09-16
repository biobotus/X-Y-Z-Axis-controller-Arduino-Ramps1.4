#include <Stepper.h>
#include "Defines.h"
#include "MotorStructure.h"
#include <Arduino.h>

using namespace std;

///////////////////////   Arduino ///////////////////////

//******* POT to control a stepper speed and direction ******//

String readString;
String Echo;
String OneTwo;
String ThreeFourFiveSix;

//char strValue[MAX_LEN - 1];
int index = 0;
int intThreeFourFiveSix = 0;

String strValue = "";

int position_X1 = 0;
int position_Y1 = 0;
int position_Z1 = 0;
int position_Z2 = 0;
int position_Z3 = 0;

int actual_position_X1 = 0;
int actual_position_Y1 = 0;
int actual_position_Z1 = 0;
int actual_position_Z2 = 0;
int actual_position_Z3 = 0;

//declare values
int Homing_Speed_X1 = 100;       //step speed (delay between steps)
int Homing_Speed_Y1 = 100;       //step speed (delay between steps)  
int Homing_Speed_Z1 = 100;       //step speed (delay between steps)  
int Homing_Speed_Z2 = 100;       //step speed (delay between steps)  
int Homing_Speed_Z3 = 100;       //step speed (delay between steps)  

int Speed_X1 = 5000;
int Speed_Y1 = 10000;
int Speed_Z1 = 1500;
int Speed_Z2 = 1500;
int Speed_Z3 = 1500;

int val = 0;
int j = 0;

// Initializing motors
Motor Motor_X = {0,0,0,0,HOMING_SPEED_X,PIN_STEP_X,PIN_DIR_X,PIN_ENABLE_X,PIN_LMSWITCH_X,0,MAX_POSITION_X,MM_PER_STEP_X,SUB_STEP_NUMBER_X,LOW, HIGH,MAX_DELAY_X,MAX_RAMP_X,CRUISE_SPEED_DELAY_X };
Motor Motor_Y = { 0,0,0,0,HOMING_SPEED_Y,PIN_STEP_Y,PIN_DIR_Y,PIN_ENABLE_Y,PIN_LMSWITCH_Y,0,MAX_POSITION_Y,MM_PER_STEP_Y,SUB_STEP_NUMBER_Y,LOW, HIGH,MAX_DELAY_Y,MAX_RAMP_Y,CRUISE_SPEED_DELAY_Y };
Motor Motor_Z1 = { 0,0,0,0,HOMING_SPEED_Z1,PIN_STEP_Z1,PIN_DIR_Z1,PIN_ENABLE_Z1,PIN_LMSWITCH_Z1,0,MAX_POSITION_X,MM_PER_STEP_Z1,SUB_STEP_NUMBER_Z1,HIGH,LOW,MAX_DELAY_Z1,MAX_RAMP_Z1,CRUISE_SPEED_DELAY_Z1 };
Motor Motor_Z2 = { 0,0,0,0,HOMING_SPEED_Z2,PIN_STEP_Z2,PIN_DIR_Z2,PIN_ENABLE_Z2,PIN_LMSWITCH_Z2,0,MAX_POSITION_Z2,MM_PER_STEP_Z2,SUB_STEP_NUMBER_Z2,HIGH,LOW,MAX_DELAY_Z2,MAX_RAMP_Z2,CRUISE_SPEED_DELAY_Z2 };
Motor Motor_Z3 = { 0,0,0,0,HOMING_SPEED_Z3,PIN_STEP_Z3,PIN_DIR_Z3,PIN_ENABLE_Z3,PIN_LMSWITCH_Z3,0,MAX_POSITION_Z3,MM_PER_STEP_Z3,SUB_STEP_NUMBER_Z3,HIGH,LOW,MAX_DELAY_Z3,MAX_RAMP_Z3,CRUISE_SPEED_DELAY_Z3 };

void setup() {

	// Initializing motor maximum positions in substeps
	Motor_X.setMaxPositionSubsteps((String)MAX_POSITION_X);
	Motor_Y.setMaxPositionSubsteps((String)MAX_POSITION_Y);
	Motor_Z1.setMaxPositionSubsteps((String)MAX_POSITION_Z1);
	Motor_Z2.setMaxPositionSubsteps((String)MAX_POSITION_Z2);
	Motor_Z3.setMaxPositionSubsteps((String)MAX_POSITION_Z3);

	//Stepper(200,Step_X1,Dir_X1);
	Serial.begin(BAUD_RATE);	

	pinMode(PIN_STEP_X, OUTPUT);
	pinMode(PIN_DIR_X, OUTPUT);
	pinMode(PIN_ENABLE_X, OUTPUT);
	pinMode(PIN_LMSWITCH_X, INPUT);

	pinMode(PIN_STEP_Y, OUTPUT);
	pinMode(PIN_DIR_Y, OUTPUT);
	pinMode(PIN_ENABLE_Y, OUTPUT);
	pinMode(PIN_LMSWITCH_Y, INPUT);

	pinMode(PIN_STEP_Z1, OUTPUT);
	pinMode(PIN_DIR_Z1, OUTPUT);
	pinMode(PIN_ENABLE_Z1, OUTPUT);
	pinMode(PIN_LMSWITCH_Z1, INPUT);

	pinMode(PIN_STEP_Z2, OUTPUT);
	pinMode(PIN_DIR_Z2, OUTPUT);
	pinMode(PIN_ENABLE_Z2, OUTPUT);
	pinMode(PIN_LMSWITCH_Z2, INPUT);

	pinMode(PIN_STEP_Z3, OUTPUT);
	pinMode(PIN_DIR_Z3, OUTPUT);
	pinMode(PIN_ENABLE_Z3, OUTPUT);
	pinMode(PIN_LMSWITCH_Z3, INPUT);
	// Reserving space for string
	strValue.reserve(128);
}

void homing_all_5_axis() {

	while (digitalRead(PIN_LMSWITCH_Z1))
	{
		digitalWrite(PIN_ENABLE_Z1, LOW);// enable
		digitalWrite(PIN_DIR_Z1, HIGH);  // Goes Up
		digitalWrite(PIN_STEP_Z1, HIGH);
		delayMicroseconds(2);
		digitalWrite(PIN_STEP_Z1, LOW);
		delayMicroseconds(Homing_Speed_Z1);
	}
	digitalWrite(PIN_ENABLE_Z1, HIGH);// disable
	Motor_Z1.currentPositionSubsteps = 0;
	position_Z1 = 0;

	while (digitalRead(PIN_LMSWITCH_Z2))
	{
		digitalWrite(PIN_ENABLE_Z2, LOW);// enable
		digitalWrite(PIN_DIR_Z2, HIGH);  // Goes Up
		digitalWrite(PIN_STEP_Z2, HIGH);
		delayMicroseconds(2);
		digitalWrite(PIN_STEP_Z2, LOW);
		delayMicroseconds(Homing_Speed_Z2);
	}
	digitalWrite(PIN_ENABLE_Z2, HIGH);// disable
	Motor_Z2.currentPositionSubsteps = 0;
	position_Z2 = 0;

	while (digitalRead(PIN_LMSWITCH_Z3))
	{
		digitalWrite(PIN_ENABLE_Z3, LOW);// enable
		digitalWrite(PIN_DIR_Z3, HIGH);  // Goes Up
		digitalWrite(PIN_STEP_Z3, HIGH);
		delayMicroseconds(2);
		digitalWrite(PIN_STEP_Z3, LOW);
		delayMicroseconds(Homing_Speed_Z3);
	}
	digitalWrite(PIN_ENABLE_Z3, HIGH);// disable
	Motor_Z3.currentPositionSubsteps = 0;
	position_Z3 = 0;

	while (digitalRead(PIN_LMSWITCH_X))
	{
		digitalWrite(PIN_ENABLE_X, LOW);// enable
		digitalWrite(PIN_DIR_X, LOW);  // Other direction
		digitalWrite(PIN_STEP_X, HIGH);
		delayMicroseconds(2);
		digitalWrite(PIN_STEP_X, LOW);
		delayMicroseconds(Homing_Speed_X1);
	}
	digitalWrite(PIN_ENABLE_X, HIGH);// disable
	Motor_X.currentPositionSubsteps = 0;
	position_X1 = 0;

	while (digitalRead(PIN_LMSWITCH_Y))
	{
		digitalWrite(PIN_ENABLE_Y, LOW);// enable
		digitalWrite(PIN_DIR_Y, LOW);  // Other direction
		digitalWrite(PIN_STEP_Y, HIGH);
		delayMicroseconds(2);
		digitalWrite(PIN_STEP_Y, LOW);
		delayMicroseconds(Homing_Speed_Y1);
	}
	digitalWrite(PIN_ENABLE_Y, HIGH);// disable
	Motor_Y.currentPositionSubsteps = 0;
	position_Y1 = 0;
}

void disableAllMotors(void)
{

}

int moveMotor(Motor* motorToMove)
{
#ifdef DEBUG_PRINT
	Serial.println(F("Desired substeps to do : "));
	Serial.println(motorToMove->desiredPositionSubsteps);
	Serial.print(F("Current position (in substeps): "));
	Serial.println(motorToMove->currentPositionSubsteps);
#endif	

	int movementDirection = ((motorToMove->desiredPositionSubsteps - motorToMove->currentPositionSubsteps) > 0 ? motorToMove->leavingHomedirection : motorToMove->towardHomeDirection);
	motorToMove->lastPositionSubsteps = motorToMove->currentPositionSubsteps;
	digitalWrite(motorToMove->pinEnable, LOW);// enable
	digitalWrite(motorToMove->pinDir, movementDirection);  // Set direction
	
	while (motorToMove->currentPositionSubsteps != motorToMove->desiredPositionSubsteps && motorToMove->currentPositionSubsteps < motorToMove->maxPositionSubsteps)
	{
		digitalWrite(motorToMove->pintStep, HIGH);
		delayMicroseconds(20);
		digitalWrite(motorToMove->pintStep, LOW);
		//delayMicroseconds(motorToMove->initialHomingSpeed);

		delayMicroseconds(30);//200);

		if (movementDirection == motorToMove->leavingHomedirection)
		{
			motorToMove->currentPositionSubsteps += 1;
		}			
		else
		{
			motorToMove->currentPositionSubsteps = motorToMove->currentPositionSubsteps - 1;
		}			
	}
	digitalWrite(motorToMove->pinEnable, HIGH);// disable
	return 1;
}

int moveMotorX(Motor* motorToMove, int delay)
{
#ifdef DEBUG_PRINT
	Serial.println(F("Desired substeps to do : "));
	Serial.println(motorToMove->desiredPositionSubsteps);
	Serial.print(F("Current position (in substeps): "));
	Serial.println(motorToMove->currentPositionSubsteps);
#endif	

	int movementDirection = ((motorToMove->desiredPositionSubsteps - motorToMove->currentPositionSubsteps) > 0 ? motorToMove->leavingHomedirection : motorToMove->towardHomeDirection);
	motorToMove->lastPositionSubsteps = motorToMove->currentPositionSubsteps;
	digitalWrite(motorToMove->pinEnable, LOW);// enable
	digitalWrite(motorToMove->pinDir, movementDirection);  // Set direction

	while (motorToMove->currentPositionSubsteps != motorToMove->desiredPositionSubsteps && motorToMove->currentPositionSubsteps < motorToMove->maxPositionSubsteps)
	{
		digitalWrite(motorToMove->pintStep, HIGH);
		delayMicroseconds(20);
		digitalWrite(motorToMove->pintStep, LOW);
		//delayMicroseconds(motorToMove->initialHomingSpeed);

		delayMicroseconds(delay);//200);

		if (movementDirection == motorToMove->leavingHomedirection)
		{
			motorToMove->currentPositionSubsteps += 1;
		}
		else
		{
			motorToMove->currentPositionSubsteps = motorToMove->currentPositionSubsteps - 1;
		}
	}
	digitalWrite(motorToMove->pinEnable, HIGH);// disable
	return 1;
}


int moveMotorWithKindOfTrajectory(Motor* motorToMove)
{
	int movementDirection = ((motorToMove->desiredPositionSubsteps - motorToMove->currentPositionSubsteps) > 0 ? motorToMove->leavingHomedirection : motorToMove->towardHomeDirection);
	
	long stepCounter = 0;	// Will hold the step count to know when to change delay to affect speed accordingly
	long stepsToTravel = abs(motorToMove->desiredPositionSubsteps - motorToMove->currentPositionSubsteps);
	int initialDelay = 30;
	int maxSpeedDelay = 2;	// Smaller is faster
	long diff = initialDelay - maxSpeedDelay;
	long divider = (long)stepsToTravel / 2;
	//long increment = (long)(((100000*diff) / divider));
	float increment = (100000 * diff) / divider;

	//long increment = long(100000*((initialDelay - maxSpeedDelay) / (stepsToTravel/2)));
	bool firstHalf = true;
	bool plateau = false;
	bool decelerationPhase = false;

	Serial.print("Steps to travel: ");
	Serial.println(stepsToTravel);
	Serial.print("Diff: ");
	Serial.println(diff);
	Serial.print("Divider: ");
	Serial.println(divider);
	Serial.print("Increment: ");
	Serial.println(increment);
	Serial.println("Current position : ");
	Serial.print(motorToMove->currentPositionSubsteps);
	Serial.println("Desired position : ");
	Serial.print(motorToMove->desiredPositionSubsteps);
	Serial.println();

	motorToMove->lastPositionSubsteps = motorToMove->currentPositionSubsteps;
	digitalWrite(motorToMove->pinEnable, LOW);// enable
	digitalWrite(motorToMove->pinDir, movementDirection);  // Set direction

	while (motorToMove->currentPositionSubsteps != motorToMove->desiredPositionSubsteps && motorToMove->currentPositionSubsteps < motorToMove->maxPositionSubsteps)
	{
		digitalWrite(motorToMove->pintStep, HIGH);
		delayMicroseconds(4);
		digitalWrite(motorToMove->pintStep, LOW);

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
			delayMicroseconds(int(maxSpeedDelay + increment/100000*stepCounter));
		}

		if (movementDirection == motorToMove->leavingHomedirection)
		{
			motorToMove->currentPositionSubsteps += 1;
		}
		else
		{
			motorToMove->currentPositionSubsteps = motorToMove->currentPositionSubsteps - 1;
		}
		stepCounter += 1;
	}
	digitalWrite(motorToMove->pinEnable, HIGH);// disable
	return 1;
}

int moveMotorTrapezoidally(Motor* motorToMove)
{
	int movementDirection = ((motorToMove->desiredPositionSubsteps - motorToMove->currentPositionSubsteps) > 0 ? motorToMove->leavingHomedirection : motorToMove->towardHomeDirection);
	motorToMove->lastPositionSubsteps = motorToMove->currentPositionSubsteps;
	digitalWrite(motorToMove->pinEnable, LOW);// enable
	digitalWrite(motorToMove->pinDir, movementDirection);  // Set direction
	
	int initialDelay = motorToMove->maxDelay;
	float increment = (motorToMove->maxRamp);

	long stepsToTravel = abs(motorToMove->desiredPositionSubsteps - motorToMove->currentPositionSubsteps);
	long stepOfAcceleration = 0;
	long stepCounter = 0;
	int nextDelaytoSend = initialDelay;

	/*Serial.print("Steps to travel: ");
	Serial.println(stepsToTravel);
	Serial.print("Increment: ");
	Serial.println(increment);*/
	Serial.println("Current position : ");
	Serial.println(motorToMove->currentPositionSubsteps);
	/*Serial.println("Desired position : ");
	Serial.println(motorToMove->desiredPositionSubsteps);
	Serial.println();*/

	//while (motorToMove->currentPositionSubsteps != motorToMove->desiredPositionSubsteps && motorToMove->currentPositionSubsteps < motorToMove->maxPositionSubsteps)
	while (stepCounter <= stepsToTravel)
	{
		digitalWrite(motorToMove->pintStep, HIGH);
		delayMicroseconds(4);
		digitalWrite(motorToMove->pintStep, LOW);

		if (nextDelaytoSend >= motorToMove->cruisingSpeedDelay)
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

		if (movementDirection == motorToMove->leavingHomedirection)
		{
			motorToMove->currentPositionSubsteps += 1;
		}
		else
		{
			motorToMove->currentPositionSubsteps = motorToMove->currentPositionSubsteps - 1;
		}
		//Serial.println(nextDelaytoSend);
		stepCounter++;
	}
	Serial.println("JUST GOT OUT");
	digitalWrite(motorToMove->pinEnable, HIGH);// disable
	return 1;
}

void loop()
{
	if (commandReceivedFlag == 1)
	{
		commandReceivedFlag = 0;
		
		char cmd = strValue.charAt(0);
	
		switch (cmd)//strValue[0])
		{
		case 'X':
		{
			String temp = strValue.substring(strValue.indexOf('X') + 1);
			Motor_X.setDesiredPositionSubsteps(temp);

			moveMotorX(&Motor_X, 150);
			//moveMotorWithKindOfTrajectory(&Motor_X);
			//moveMotorTrapezoidally(&Motor_X);
			break;
		}
		case 'Y':
		{
			String temp = strValue.substring(strValue.indexOf('Y') + 1);
			Motor_Y.setDesiredPositionSubsteps(temp);
			moveMotor(&Motor_Y);
			//moveMotorWithKindOfTrajectory(&Motor_Y);

			break;
		}
		case 'Z':
		{
			char zAxisNumber = strValue[1];

			if (zAxisNumber == '1')
			{
				String temp = strValue.substring(strValue.indexOf('1') + 1);
				Motor_Z1.setDesiredPositionSubsteps(temp);
				//moveMotor(&Motor_Z1);
				moveMotorWithKindOfTrajectory(&Motor_Z1);
				//moveMotorWithKindOfTrajectory(&Motor_Z1);
			}
			else if (zAxisNumber == '2')
			{
				String temp = strValue.substring(strValue.indexOf('2') + 1);
				Motor_Z2.setDesiredPositionSubsteps(temp);
				moveMotor(&Motor_Z2);
				//moveMotorWithKindOfTrajectory(&Motor_Z2);
			}
			else if (zAxisNumber == '3')
			{
				String temp = strValue.substring(strValue.indexOf('3') + 1);
				Motor_Z3.setDesiredPositionSubsteps(temp);
				moveMotor(&Motor_Z3);
				//moveMotorWithKindOfTrajectory(&Motor_Z3);
			}
			break;
		}
		case 'H':
		{
			homing_all_5_axis();
			break;
		}
		case 'B': // Stands for a sequence :s
		{
			String temp = "8000"; // Send robot to 95cm from home on the X axis.
			Motor_X.setDesiredPositionSubsteps(temp);
			moveMotorX(&Motor_X, 160);

			temp = "3000"; // Send robot to 40cm from home on the Y axis.
			Motor_Y.setDesiredPositionSubsteps(temp);
			moveMotor(&Motor_Y);

			temp = "2430"; // Send robot to 95cm from home on the X axis.
			Motor_Z2.setDesiredPositionSubsteps(temp);
			moveMotor(&Motor_Z2);

			/*temp = "1500"; // Send robot to 40cm from home on each of the Z axis.
			Motor_Z1.setDesiredPositionSubsteps(temp);
			moveMotor(&Motor_Z1);
			Motor_Z2.setDesiredPositionSubsteps(temp);
			moveMotor(&Motor_Z2);
			Motor_Z3.setDesiredPositionSubsteps(temp);
			moveMotor(&Motor_Z3);*/

			//homing_all_5_axis();
			break;
		}
		case 'A':
		{
			String temp = "3000"; // Send robot to 95cm from home on the X axis.
			Motor_X.setDesiredPositionSubsteps(temp);
			moveMotorX(&Motor_X, 150);

			temp = "3500"; // Send robot to 40cm from home on the Y axis.
			Motor_Y.setDesiredPositionSubsteps(temp);
			moveMotor(&Motor_Y);

			temp = "3500"; // Send robot to 40cm from home on the Y axis.
			Motor_Z2.setDesiredPositionSubsteps(temp);
			moveMotor(&Motor_Z2);

			break;
		}
		default:
			break;
		}
		strValue = "";	// Clearing string content to reset command analysis
	}

	/*if (Serial.available())
	{
	Serial.println(F("Serial is available"));
	memset(strValue, 0, sizeof(strValue));

	char receivedData = Serial.read();
	while (receivedData != 13) //wait for carriage return
	{
	while (receivedData != 13) // 13 is ascii value of carriage return
	{
	if (index < MAX_LEN && receivedData > 0) //If character is valid, and there is room...
	{
	strValue[index++] = receivedData; // add the ASCII character to the string;
	strValue[index] = '\0';
	}
	receivedData = Serial.read(); // Read another character
	}
	}
	index = 0;
	}*/
}


/*
SerialEvent occurs whenever a new data comes in the
hardware serial RX.  This routine is run between each
time loop() runs, so using delay inside loop can delay
response.  Multiple bytes of data may be available.
*/
void serialEvent() 
{
	while (Serial.available()) {
		// get the new byte:
		char inChar = (char)Serial.read();

		if ((inChar >= 48 && inChar <= 122) || inChar == 10 || inChar == 13)
		{
			// add it to the inputString:
			strValue += inChar;
		}		
		// if the incoming character is a newline, set a flag
		// so the main loop can do something about it:
		if (inChar == '\n') 
		{
			// If the instruction is asking for information, we treat it right away, otherwise we simply buff the instruction.
			char cmd = strValue.charAt(0);

			if (cmd == 'P')
			{
				char pingAxis = strValue[1];

				if (pingAxis == 'X')
				{
					Serial.print(F("Current position of the X axis(in substeps): "));
					Serial.print(Motor_X.currentPositionSubsteps);
					Serial.println();
				}
				else if (pingAxis == 'Y')
				{
					Serial.print(F("Current position of the Y axis (in substeps): "));
					Serial.print(Motor_Y.currentPositionSubsteps);
					Serial.println();
				}
				else if (pingAxis == 'Z')
				{
					char zAxisNumber = strValue[2];

					if (zAxisNumber == '1')
					{
						Serial.print(F("Current position of the Z1 axis (in substeps): "));
						Serial.print(Motor_Z1.currentPositionSubsteps);
						Serial.println();
					}
					else if (zAxisNumber == '2')
					{
						Serial.print(F("Current position of the Z2 axis (in substeps): "));
						Serial.print(Motor_Z2.currentPositionSubsteps);
						Serial.println();
					}
					else if (zAxisNumber == '3')
					{
						Serial.print(F("Current position of the Z3 axis (in substeps): "));
						Serial.print(Motor_Z3.currentPositionSubsteps);
						Serial.println();
					}
				}
				strValue = "";
			}
			else
			{
				commandReceivedFlag = true;
			}	
			
			/*if (strValue.indexOf("Stop") != 0)
			{
				String motorToStop = strValue.substring(strValue.indexOf(":"));
			}*/
		}
	}
}
