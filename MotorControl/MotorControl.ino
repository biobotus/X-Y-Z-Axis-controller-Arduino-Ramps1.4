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
Motor Motor_X = {0,0,0,0,HOMING_SPEED_X,PIN_STEP_X,PIN_DIR_X,PIN_ENABLE_X,PIN_LMSWITCH_X,MAX_POSITION_X,MM_PER_STEP_X,SUB_STEP_NUMBER_X,LOW, HIGH };
Motor Motor_Y = { 0,0,0,0,HOMING_SPEED_Y,PIN_STEP_Y,PIN_DIR_Y,PIN_ENABLE_Y,PIN_LMSWITCH_Y,MAX_POSITION_Y,MM_PER_STEP_Y,SUB_STEP_NUMBER_Y,LOW, HIGH };
Motor Motor_Z1 = { 0,0,0,0,HOMING_SPEED_Z1,PIN_STEP_Z1,PIN_DIR_Z1,PIN_ENABLE_Z1,PIN_LMSWITCH_Z1,MAX_POSITION_Z1,MM_PER_STEP_Z1,SUB_STEP_NUMBER_Z1,HIGH, LOW };
Motor Motor_Z2 = { 0,0,0,0,HOMING_SPEED_Z2,PIN_STEP_Z2,PIN_DIR_Z2,PIN_ENABLE_Z2,PIN_LMSWITCH_Z2,MAX_POSITION_Z2,MM_PER_STEP_Z2,SUB_STEP_NUMBER_Z2,HIGH, LOW };
Motor Motor_Z3 = { 0,0,0,0,HOMING_SPEED_Z3,PIN_STEP_Z3,PIN_DIR_Z3,PIN_ENABLE_Z3,PIN_LMSWITCH_Z3,MAX_POSITION_Z3,MM_PER_STEP_Z3,SUB_STEP_NUMBER_Z3,HIGH, LOW };

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
		delayMicroseconds(motorToMove->homingSpeed);

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
			moveMotor(&Motor_X);
			break;
		}
		case 'Y':
		{
			String temp = strValue.substring(strValue.indexOf('Y') + 1);
			Motor_Y.setDesiredPositionSubsteps(temp);
			moveMotor(&Motor_Y);
			break;
		}
		case 'Z':
		{
			char zAxisNumber = strValue[1];

			if (zAxisNumber == '1')
			{
				String temp = strValue.substring(strValue.indexOf('1') + 1);
				Motor_Z1.setDesiredPositionSubsteps(temp);
				moveMotor(&Motor_Z1);
			}
			else if (zAxisNumber == '2')
			{
				String temp = strValue.substring(strValue.indexOf('2') + 1);
				Motor_Z2.setDesiredPositionSubsteps(temp);
				moveMotor(&Motor_Z2);
			}
			else if (zAxisNumber == '3')
			{
				String temp = strValue.substring(strValue.indexOf('3') + 1);
				Motor_Z3.setDesiredPositionSubsteps(temp);
				moveMotor(&Motor_Z3);
			}
			break;
		}
		case 'H':
		{
			homing_all_5_axis();
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
void serialEvent() {
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
		if (inChar == '\n') {
			commandReceivedFlag = true;
			/*if (strValue.indexOf("Stop") != 0)
			{
				String motorToStop = strValue.substring(strValue.indexOf(":"));
			}*/
		}
	}
}
