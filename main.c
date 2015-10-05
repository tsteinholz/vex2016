#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  testEncoder,    sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           mFrontLeft,    tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           mShooter2,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           mShooter3,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           mShooter4,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           mBackLeft,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           mFrontRight,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           mShooter7,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           mShooter8,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           mShooter9,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          mBackRight,    tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)
//*!!Code automatically generated by 'ROBOTC' configuration wizard, cleaned up by me!!*//

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)
#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

//--------------------Button Mapping--------------------\\
//Drive axis
#define joyDriveA Ch1 //First axis for the drive joysticks
#define joyDriveB Ch4 //Second axis for the drive joysticks
#define joyDriveC Ch3 //Third axis for the drive joysticks

//Shooter buttons
#define joyShooterZero Btn5U //Set the shooter speed to zero (E-STOP)
#define joyShooterIncU Btn6D //Increment shooter speed
#define joyShooterIncD Btn5D //Decrement the shooter speed
#define joyShooterFull Btn6U //Set the shooter speed to max (127)


//--------------------Constants--------------------\\
#define targetShooterPos = 127; //Optimal speed for firing

float shooterIncrement = 0.01; //How much to increment or decrement speed each tick

//--------------------Variables--------------------\\
float shooterSpeed = 0; //stores the current set speed for the shooter motors

//--------------------Helper Functions-------------\\
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
   motor[mShooter3] = -power;
   motor[mShooter4] = -power;
   motor[mShooter7] = -power;
   motor[mShooter8] = power;
   motor[mShooter9] = power;
}

//--------------------Initalization Code--------------------\\
void pre_auton() {
  bStopTasksBetweenModes = true; //Set false for user tasks to run between mode switches
}

//--------------------Autonomous mode--------------------\\
task autonomous() {
	AutonomousCodePlaceholderForTesting();  // Remove this function call once you have "real" code.
}

//--------------------Manual Control Loop--------------------\\
task usercontrol() {
	float lastValue = 0;
	float currentValue = 0;
	//Main operator control loop
  while(true){
  	//Test code for the encoders
    wait1Msec(10);
    lastValue = currentValue;
    currentValue = SensorValue[testEncoder];
    writeDebugStreamLine("speed: %-4s value:", (currentValue-lastValue), currentValue);

	  int x = vexRT[joyDriveA];
	  int y = vexRT[joyDriveB];
  	int z = vexRT[joyDriveC];

  	//Basic configuration for 4 meccanum wheel drive
  	setDriveMotors(x + z + y,
	                 x - z - y,
	                 x + z - y,
	                 x - z + y);

	  //bring the shooter to a full stop permanently
  	if (vexRT[joyShooterZero] == 1) {
	  	shooterSpeed = 0;
	  	setShooterMotors(0);

	  //Increment the motor speed by 10
  	}else if (vexRT[joyShooterIncU] == 1) {
	    shooterSpeed += shooterIncrement;
	    setShooterMotors(shooterSpeed);

	  //Decrement the motor speed by 10
	  }else if (vexRT[joyShooterIncD] == 1) {
	    shooterSpeed -= shooterIncrement;
	    setShooterMotors(shooterSpeed);

	  //Set the shooter speed to the maximum temporarily
    }else if (vexRT[joyShooterFull] == 1) {
		  setShooterMotors(127);

    }else{
		  setShooterMotors(shooterSpeed);
    } //End shooter button if statements
  } //End main program loop
}
