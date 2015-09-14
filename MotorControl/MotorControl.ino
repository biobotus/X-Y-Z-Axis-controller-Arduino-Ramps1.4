#include <Stepper.h>

///////////////////////   Arduino ///////////////////////

//******* POT to control a stepper speed and direction ******//

//declare pins
#define BAUD_RATE 115200
#define MAX_LEN 128

String readString;
String Echo;
String OneTwo;
String ThreeFourFiveSix;

char strValue[MAX_LEN - 1];
int index = 0;
int intThreeFourFiveSix = 0;

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

int max_position_X1 = 10000;
int max_position_Y1 = 10000;
int max_position_Z1 = 10000;
int max_position_Z2 = 10000;
int max_position_Z3 = 10000;

int Step_X1 = 54;
int Dir_X1 = 55;
int Enable_X1 = 38;
int Limit_switch_X1 = 3;

int Step_Y1 = 60;
int Dir_Y1 = 61;
int Enable_Y1 = 56;
int Limit_switch_Y1 = 14;

int Step_Z1 = 46;
int Dir_Z1 = 48;
int Enable_Z1 = 62;
int Limit_switch_Z1 = 18;

int Step_Z2 = 26;
int Dir_Z2 = 28;
int Enable_Z2 = 24;
int Limit_switch_Z2 = 15;

int Step_Z3 = 36;
int Dir_Z3 = 34;
int Enable_Z3 = 30;
int Limit_switch_Z3 = 2;

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

float mm_per_step_X1 = 0.127;
float mm_per_step_Y1 = 0.127;
float mm_per_step_Z1 = 200;
float mm_per_step_Z2 = 200;
float mm_per_step_Z3 = 200;

int val = 0;
int j = 0;

void setup() {

	//Stepper(200,Step_X1,Dir_X1);
	Serial.begin(BAUD_RATE);

	pinMode(Step_X1, OUTPUT);
	pinMode(Dir_X1, OUTPUT);
	pinMode(Enable_X1, OUTPUT);
	pinMode(Limit_switch_X1, INPUT);

	pinMode(Step_Y1, OUTPUT);
	pinMode(Dir_Y1, OUTPUT);
	pinMode(Enable_Y1, OUTPUT);
	pinMode(Limit_switch_Y1, INPUT);

	pinMode(Step_Z1, OUTPUT);
	pinMode(Dir_Z1, OUTPUT);
	pinMode(Enable_Z1, OUTPUT);
	pinMode(Limit_switch_Z1, INPUT);

	pinMode(Step_Z2, OUTPUT);
	pinMode(Dir_Z2, OUTPUT);
	pinMode(Enable_Z2, OUTPUT);
	pinMode(Limit_switch_Z2, INPUT);

	pinMode(Step_Z3, OUTPUT);
	pinMode(Dir_Z3, OUTPUT);
	pinMode(Enable_Z3, OUTPUT);
	pinMode(Limit_switch_Z3, INPUT);

}

void homing_all_5_axis() {

	while (digitalRead(Limit_switch_X1))
	{
		digitalWrite(Enable_X1, LOW);// enable
		digitalWrite(Dir_X1, LOW);  // Other direction
		digitalWrite(Step_X1, HIGH);
		delayMicroseconds(2);
		digitalWrite(Step_X1, LOW);
		delayMicroseconds(Homing_Speed_X1);
	}
	digitalWrite(Enable_X1, HIGH);// disable
	position_X1 = 0;

	while (digitalRead(Limit_switch_Y1))
	{
		digitalWrite(Enable_Y1, LOW);// enable
		digitalWrite(Dir_Y1, LOW);  // Other direction
		digitalWrite(Step_Y1, HIGH);
		delayMicroseconds(2);
		digitalWrite(Step_Y1, LOW);
		delayMicroseconds(Homing_Speed_Y1);
	}
	digitalWrite(Enable_Y1, HIGH);// disable
	position_Y1 = 0;

	while (digitalRead(Limit_switch_Z1))
	{
		digitalWrite(Enable_Z1, LOW);// enable
		digitalWrite(Dir_Z1, HIGH);  // Goes Up
		digitalWrite(Step_Z1, HIGH);
		delayMicroseconds(2);
		digitalWrite(Step_Z1, LOW);
		delayMicroseconds(Homing_Speed_Z1);
	}
	digitalWrite(Enable_Z1, HIGH);// disable
	position_Z1 = 0;

	while (digitalRead(Limit_switch_Z2))
	{
		digitalWrite(Enable_Z2, LOW);// enable
		digitalWrite(Dir_Z2, HIGH);  // Goes Up
		digitalWrite(Step_Z2, HIGH);
		delayMicroseconds(2);
		digitalWrite(Step_Z2, LOW);
		delayMicroseconds(Homing_Speed_Z2);
	}
	digitalWrite(Enable_Z2, HIGH);// disable
	position_Z2 = 0;

	while (digitalRead(Limit_switch_Z3))
	{
		digitalWrite(Enable_Z3, LOW);// enable
		digitalWrite(Dir_Z3, HIGH);  // Goes Up
		digitalWrite(Step_Z3, HIGH);
		delayMicroseconds(2);
		digitalWrite(Step_Z3, LOW);
		delayMicroseconds(Homing_Speed_Z3);
	}
	digitalWrite(Enable_Z3, HIGH);// disable
	position_Z3 = 0;

}

void moveX1Motor(int desired_position_X1)
{
	actual_position_X1 = position_X1;
	int displacement_X1 = desired_position_X1 - actual_position_X1;

	Serial.println("Moving X1 axis with an actual position of :");
	Serial.println(actual_position_X1);
	Serial.println("Moving X1 axis with a displacement of :");
	Serial.println(displacement_X1);
	Serial.println("Moving X1 axis with a desired position of :");
	Serial.println(desired_position_X1);

	digitalWrite(Enable_X1, LOW);// enable
	digitalWrite(Step_X1, HIGH);

	Stepper MotorX1 = Stepper(200, Dir_X1, Step_X1);
	MotorX1.setSpeed(Speed_X1);
	MotorX1.step(-displacement_X1 / mm_per_step_X1);

	position_X1 = desired_position_X1;
}

void moveY1Motor(int desired_position_Y1)
{
	actual_position_Y1 = position_Y1;
	int displacement_Y1 = desired_position_Y1 - actual_position_Y1;

	Serial.println("Moving Y1 axis with an actual position of :");
	Serial.println(actual_position_Y1);
	Serial.println("Moving Y1 axis with a displacement of :");
	Serial.println(displacement_Y1);
	Serial.println("Moving Y1 axis with a desired position of :");
	Serial.println(desired_position_Y1);

	digitalWrite(Enable_Y1, LOW);// enable
	digitalWrite(Step_Y1, HIGH);

	Stepper MotorY1 = Stepper(200, Dir_Y1, Step_Y1);
	MotorY1.setSpeed(Speed_Y1);
	Serial.println("deplacement: ");

	Serial.println(-displacement_Y1 / mm_per_step_Y1);
	MotorY1.step(-displacement_Y1 / mm_per_step_Y1);

	position_Y1 = desired_position_Y1;
}

void moveZ1Motor(int desired_position_Z1)
{
	actual_position_Z1 = position_Z1;
	int displacement_Z1 = desired_position_Z1 - actual_position_Z1;

	Serial.println("Moving Z1 axis with an actual position of :");
	Serial.println(actual_position_Z1);
	Serial.println("Moving Z1 axis with a displacement of :");
	Serial.println(displacement_Z1);
	Serial.println("Moving Z1 axis with a desired position of :");
	Serial.println(desired_position_Z1);

	digitalWrite(Enable_Z1, LOW);// enable
	digitalWrite(Step_Z1, HIGH);

	Stepper MotorZ1 = Stepper(200, Dir_Z1, Step_Z1);
	MotorZ1.setSpeed(Speed_Z1);
	MotorZ1.step(-displacement_Z1*mm_per_step_Z1);

	position_Z1 = desired_position_Z1;
}


void moveZ2Motor(int desired_position_Z2)
{
	actual_position_Z2 = position_Z2;
	int displacement_Z2 = desired_position_Z2 - actual_position_Z2;

	Serial.println("Moving Z2 axis with an actual position of :");
	Serial.println(actual_position_Z2);
	Serial.println("Moving Z2 axis with a displacement of :");
	Serial.println(displacement_Z2);
	Serial.println("Moving Z2 axis with a desired position of :");
	Serial.println(desired_position_Z2);

	digitalWrite(Enable_Z2, LOW);// enable
	digitalWrite(Step_Z2, HIGH);

	Stepper MotorZ2 = Stepper(200, Dir_Z2, Step_Z2);
	MotorZ2.setSpeed(Speed_Z2);
	MotorZ2.step(-displacement_Z2*mm_per_step_Z2);

	position_Z2 = desired_position_Z2;
}

void moveZ3Motor(int desired_position_Z3)
{
	actual_position_Z3 = position_Z3;
	int displacement_Z3 = desired_position_Z3 - actual_position_Z3;

	Serial.println("Moving Z3 axis with an actual position of :");
	Serial.println(actual_position_Z3);
	Serial.println("Moving Z3 axis with a displacement of :");
	Serial.println(displacement_Z3);
	Serial.println("Moving Z3 axis with a desired position of :");
	Serial.println(desired_position_Z3);

	digitalWrite(Enable_Z3, LOW);// enable
	digitalWrite(Step_Z3, HIGH);

	Stepper MotorZ3 = Stepper(200, Dir_Z3, Step_Z3);
	MotorZ3.setSpeed(Speed_Z3);
	MotorZ3.step(-displacement_Z3*mm_per_step_Z3);

	position_Z3 = desired_position_Z3;
}

void loop()
{

	if (Serial.available())
	{
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
	}





	Echo = strValue;
	OneTwo = Echo.substring(1, 3);
	ThreeFourFiveSix = Echo.substring(3, 8);
	intThreeFourFiveSix = ThreeFourFiveSix.toInt();

	if (OneTwo == "X1")
	{
		if (intThreeFourFiveSix >= 0 && intThreeFourFiveSix <= max_position_X1)
		{
			Serial.println("Moving X1 axis");
			moveX1Motor(intThreeFourFiveSix);
		}
		else
		{
			Serial.println("Out of bondaries of X1 axis");
		}
	}

	else if (OneTwo == "Y1")
	{
		if (intThreeFourFiveSix >= 0 && intThreeFourFiveSix <= max_position_Y1)
		{
			Serial.println("Moving Y1 axis");
			moveY1Motor(intThreeFourFiveSix);
		}
		else
		{
			Serial.println("Out of bondaries of Y1 axis");
		}
	}

	else if (OneTwo == "Z1")
	{
		if (intThreeFourFiveSix >= 0 && intThreeFourFiveSix <= max_position_Z1)
		{
			Serial.println("Moving Z1 axis");
			moveZ1Motor(intThreeFourFiveSix);
		}

		else
		{
			Serial.println("Out of bondaries of Z1 axis");
		}
	}


	else if (OneTwo == "Z2")
	{
		if (intThreeFourFiveSix >= 0 && intThreeFourFiveSix <= max_position_Z2)
		{
			Serial.println("Moving Z2 axis");
			moveZ2Motor(intThreeFourFiveSix);
		}
		else
		{
			Serial.println("Out of bondaries of Z2 axis");
		}
	}


	else if (OneTwo == "Z3")
	{
		if (intThreeFourFiveSix >= 0 && intThreeFourFiveSix <= max_position_Z3)
		{
			Serial.println("Moving Z3 axis");
			moveZ3Motor(intThreeFourFiveSix);
		}
		else
		{
			Serial.println("Out of bondaries of Z3 axis");
		}
	}
	else if (OneTwo == "H5")
	{
		Serial.println("Homing all 5 axis");
		homing_all_5_axis();
	}
	else
	{
		Serial.println("This is not a valid command");
		Serial.println(strValue);
		Serial.println(Echo);
		Serial.println(OneTwo);
		Serial.println(intThreeFourFiveSix);
	}
}