#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in8,    ballDetect,     sensorLineFollower)
#pragma config(Sensor, dgtl1,  rampSolenoidA,  sensorDigitalOut)
#pragma config(Sensor, dgtl2,  rampSolenoidB,  sensorDigitalOut)
#pragma config(Sensor, dgtl3,  alignSolenoid,  sensorDigitalOut)
#pragma config(Sensor, dgtl4,  shootSolenoid,  sensorDigitalOut)
#pragma config(Sensor, dgtl5,  tournamentJumper, sensorDigitalIn)
#pragma config(Sensor, dgtl6,  autonJumper,    sensorDigitalIn)
#pragma config(Sensor, dgtl12, ShooterReadyLED, sensorLEDtoVCC)
#pragma config(Sensor, I2C_1,  encLeftFront10, sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  encShooterLeft7B, sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_3,  encLeftBack6,   sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_4,  endBackRight5,  sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_5,  encShooterRight2, sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_6,  encRightFront1, sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           mFrontRight,   tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           mShooter2,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           mShooter3,     tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           mShooter4,     tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           mBackRight,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           mBackLeft,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           mShooter7,     tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           mShooter8,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           mShooter9,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          mFrontLeft,    tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#ifdef _MSC_VER
int SensorValue[];
int motor[];
int vexRT[];
int nLCDButtons;
int nImmediateBatteryLevel;
int bLCDBacklight;
typedef int bool;
#define true 1
#define false 0
typedef char* string;
typedef void task;

int bumpLeft;
int bumpRight;
int mShooter2;
int mShooter3;
int mShooter4;
int mShooter7;
int mShooter8;
int mShooter9;
int mFrontLeft;
int mFrontRight;
int mBackLeft;
int mBackRight;
int ShooterReadyLED;
int encShooterRight2;
int rampSolenoidA;
int rampSolenoidB;
int Ch1;
int Ch2;
int Ch3;
int Ch4;
int Btn5U;
int Btn6D;
int Btn5D;
int Btn6U;
int Btn7U;
int Btn7D;
int Btn7L;
int Btn7R;
int Btn8U;
int Btn8D;
int Btn8L;
int Btn8R;
int alignSolenoid;
int shootSolenoid;
int bIfiAutonomousMode;
int bIfiRobotDisabled;
int time1[];
int T1;
int T2;
int T4;
int autonJumper;
int nSysTime;
#endif

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(15)
#pragma userControlDuration(120)

#define USE_BUTTON_PLACEHOLDER
//#include "../CustomLCD.c"

#ifdef _MSC_VER
#include "C:\Program Files (x86)\Robomatter Inc\ROBOTC Development Environment 4.X\Includes\Vex_Competition_Includes.c"   //Main competition background code...do not modify!
#else
#include "Vex_Competition_Includes.c"
#endif

//--------------------Button Mapping--------------------//
//Drive axis
#define joyDriveA Ch2 //First axis for the drive joysticks
#define joyDriveB Ch4 //Second axis for the drive joysticks
#define joyDriveC Ch1 //Third axis for the drive joysticks
#define joyTurnRight Btn6U
#define joyTurnLeft Btn5U
#define joyTurnRightSlow Btn6D
#define joyTurnLeftSlow Btn5D

//Shooter buttons
#define joyShooterZero Btn7L //Set the shooter speed to zero (E-STOP)
#define joyShooterIncU Btn7U //Increment shooter speed
#define joyShooterIncD Btn7D //Decrement the shooter speed
#define joyShooterFull Btn7R //Set the shooter speed to max (127)

//Pneumatics Buttons
#define joyRampActivate  Btn8U //Activates the ramp solenoids
#define joyAlignActivate Btn8D //Toggles the alignment solenoids
#define JoyShooterManual Btn8R //Manual trigger for the shooter solenoid
#define joyShooterAuto   Btn8L //Toggles automatic triggering of the shooter solenoid

//--------------------Constants--------------------//
float optimalShooterSpd = 36.8; //Optimal speed for firing 36.8
float shooterIncrement = 0.2; //How much to increment or decrement speed each tick
int shooterSmoothTrigger = 6; //how long the shooter must be stable for in order to trigger a shot
int rampSecondsRemaining = 20; //Time from end of match that ramp will be triggerable
int ballDetectThreshold = 2525;

//--------------------Variables--------------------//
int lastSysTime = 0;
int lastSpeed = 0;
int power = 0;
float shooterMotorRaw = 0; //stores the current set speed for the shooter motors
int lastSpeedValue = 0;    //The previous speed of the shooter
int lastError = 0;
int currentSpeedValue = 0; //The current speed of the shooter
float shooterAverage = 0;  //Average speed of the shooter
float shooterTarget = 0;    //The target for the shooter PID loop
bool isShooterReady = false;
int shooterSmooth = 0; //used to smooth out the readiness detection for the shooter
bool alignState = false;
bool alignReady = true;

//--------------------Helper Functions-------------//
//Helper function for setting all drive motors in one command
void setDriveMotors(int fL, int fR, int bL, int bR) {
	motor[mFrontLeft] = fL;
	motor[mFrontRight] = fR;
	motor[mBackLeft] = bL;
	motor[mBackRight] = bR;
}

//Helper function for setting all shooter motors in one command, with correct polarity
void setShooterMotors(int power) {
	motor[mShooter2] = power;
	motor[mShooter3] = power;
	motor[mShooter4] = power;
	motor[mShooter7] = power;
	motor[mShooter8] = power;
	motor[mShooter9] = power;
}

void calculateShooter() {
	wait1Msec(50);
	lastSpeedValue = currentSpeedValue; //Get the prevoius speed of the shooter

	currentSpeedValue = SensorValue[encShooterRight2];    //Get the current speed of the shooter
	int currSysTime;

	int speed = (lastSpeed + ((currentSpeedValue - lastSpeedValue) * 50 / ((currSysTime = nSysTime) - lastSysTime))) / 2;  //Calculate the change in position between the last cycle
	lastSpeed = speed;
	lastSysTime = currSysTime;
	if (speed > 50) { speed = 50; }                      //Clamp the speed to make sure it doesn't go over 20/s
	else if (speed < -50) { speed = -50; }               // (sometimes it generates erronously high values)

	shooterAverage = (shooterAverage + speed) / 2.0; //Get an average
	float error = shooterTarget - speed;            //Calculate an error based on the target

	shooterMotorRaw = shooterMotorRaw + (error*0.05) - ((lastError - error) * 0.5);
	writeDebugStreamLine("%-4f, %-4f, %-4f, %-4f", shooterMotorRaw, speed,error, lastError - error);
	if (shooterMotorRaw > 127) { shooterMotorRaw = 127; }      //Clamp the motor output so it doesn't go above 127 or below -127
	else if (shooterMotorRaw < -127) { shooterMotorRaw = -127; }

	bool ready = (shooterAverage > shooterTarget - 2.0&& shooterAverage < shooterTarget + 2.0);
	if (ready) { shooterSmooth += 1; } //Smooth out the okay detection
	else { shooterSmooth *= 0.5; }
	isShooterReady = (ready && shooterSmooth >= shooterSmoothTrigger);
	SensorValue[ShooterReadyLED] = isShooterReady;
	bLCDBacklight = isShooterReady;
	if (isShooterReady) {
		clearLCDLine(1);
		displayLCDCenteredString(1, "READY!!!");
	}
	else {
		clearLCDLine(1);
		displayLCDCenteredString(1, "wait...");
	}
	//Debug output!
	//writeDebugStreamLine("target: %-4f speed: %-4f Motors: %i Battery: %f", shooterTarget, shooterAverage, shooterMotorRaw, nImmediateBatteryLevel/1000.0);
	//writeDebugStreamLine("%i",nTimeXX);
	lastError = error;
}

void calculateShooter2() {
	wait1Msec(50);
	lastSpeedValue = currentSpeedValue; //Get the prevoius speed of the shooter

	currentSpeedValue = SensorValue[encShooterRight2];    //Get the current speed of the shooter
	int currSysTime;

	int speed = (lastSpeed + ((currentSpeedValue - lastSpeedValue) * 50 / ((currSysTime = nSysTime) - lastSysTime))) / 2;  //Calculate the change in position between the last cycle
	lastSpeed = speed;
	lastSysTime = currSysTime;
	if (speed > 50) { speed = 50; }                      //Clamp the speed to make sure it doesn't go over 20/s
	else if (speed < -50) { speed = -50; }               // (sometimes it generates erronously high values)

	shooterAverage = (shooterAverage + speed) / 2.0; //Get an average
	float error = shooterTarget - speed;            //Calculate an error based on the target

	if (error > 8) {
		shooterMotorRaw = 127;
	}
	else {
		shooterMotorRaw = power;//shooterMotorRaw + (error*0.05) - ((lastError - error) * 0.5);
	}

	if (shooterMotorRaw > 127) { shooterMotorRaw = 127; }      //Clamp the motor output so it doesn't go above 127 or below -127
	else if (shooterMotorRaw < -127) { shooterMotorRaw = -127; }

	bool ready = (shooterAverage > shooterTarget - 2.5&& shooterAverage < shooterTarget + 2.5);
	if (ready) { shooterSmooth += 1; } //Smooth out the okay detection
	else { shooterSmooth *= 0.5; }
	isShooterReady = (ready && shooterSmooth >= shooterSmoothTrigger);
	SensorValue[ShooterReadyLED] = isShooterReady;
	bLCDBacklight = isShooterReady;
	if (isShooterReady) {
		clearLCDLine(1);
		displayLCDCenteredString(1, "READY!!!");
	}
	else {
		clearLCDLine(1);
		displayLCDCenteredString(1, "wait...");
	}
	clearLCDLine(0);
	string str;
	stringFormat(str, "%i", power);
	displayLCDCenteredString(0, str);
	//Debug output!
	//writeDebugStreamLine("target: %-4f speed: %-4f Motors: %i Battery: %f", shooterTarget, shooterAverage, shooterMotorRaw, nImmediateBatteryLevel/1000.0);
	//writeDebugStreamLine("%i",nTimeXX);
	lastError = error;
}

//Takes manual joystick inputs to control solenoids
void solenoidsManual() {
	if (vexRT[joyRampActivate] && !alignState) {
		SensorValue[rampSolenoidA] = 1; //Set the state of the ramp
		SensorValue[rampSolenoidB] = 1; //Set the state of the ramp
	}
	else if (!vexRT[joyRampActivate]) {
		SensorValue[rampSolenoidA] = 0; //Set the state of the ramp
		SensorValue[rampSolenoidB] = 0; //Set the state of the ramp
	}
	if (vexRT[joyAlignActivate]) {
		if (alignReady)
			alignState = !alignState;
		alignReady = false;
	}
	else { alignReady = true; }
	SensorValue[alignSolenoid] = alignState;
	//SensorValue[shootSolenoid] = vexRT[JoyShooterManual];
}

//--------------------Initalization Code--------------------//
void pre_auton() {
	bStopTasksBetweenModes = false; //Set false for user tasks to run between mode switches
	SensorValue[shootSolenoid] = 1;
	SensorValue[rampSolenoidA] = 0;
	SensorValue[rampSolenoidB] = 0;
	SensorValue[alignSolenoid] = 1;

}

//--------------------Autonomous mode--------------------//
task autonomous() {
	shooterTarget = optimalShooterSpd;
	SensorValue[shootSolenoid] = 1;
	if (!SensorValue[autonJumper]) {
		while (true) {
			calculateShooter();                //Calculate the shooter's speed and the motor speed

			if (Sensorvalue[ballDetect] < ballDetectThreshold | !isShooterReady) {
        SensorValue[shootSolenoid] = 1;
      }
			if (isShooterReady){
				SensorValue[shootSolenoid] = 0;
			}

			setShooterMotors(shooterMotorRaw); //set the shooter motor's speed

		}
	}
	shooterTarget = 0;
	setShooterMotors(0);

}

//--------------------Manual Control Loop--------------------//
task usercontrol() {
	//Main operator control loop
	ClearTimer(T4);
	SensorValue[shootSolenoid] = 0; //Set the shooter to open
	shooterTarget = 0;
	alignState = false;
	setShooterMotors(0);
	while (true) {

		int x = vexRT[joyDriveA];
		int y = vexRT[joyDriveB];
		int z = (vexRT[joyTurnRight] - vexRT[joyTurnLeft]) * 127 + (vexRT[joyTurnRightSlow] - vexRT[joyTurnLeftSlow]) * 50;

		//Basic configuration for 4 meccanum wheel drive
		setDriveMotors(x + z + y,
			x - z - y,
			x + z - y,
			x - z + y);

		//bring the shooter to a full stop permanently
		if (vexRT[joyShooterZero] == 1) {
			shooterTarget = 0;
			shooterMotorRaw = 0;
			setShooterMotors(0);
			power = 0;

			//Increment the target
		}
		else if (vexRT[joyShooterIncU] == 1) {
			shooterTarget += shooterIncrement;
			power++;

			//Decrement the target
		}
		else if (vexRT[joyShooterIncD] == 1) {
			shooterTarget -= shooterIncrement;
			power--;
			//Set the shooter speed to the optimal speed
		}
		else if (vexRT[joyShooterFull] == 1) {
			shooterTarget = optimalShooterSpd;
			power = 58;
		} //shooter button if statements

		calculateShooter2();                //Calculate the shooter's speed and the motor speed
		setShooterMotors(shooterMotorRaw); //set the shooter motor's speed

		solenoidsManual(); //Get button innputs for solenoid control

	} //Main Loop


}

void frameUpdate(int frameNumber) {

}
