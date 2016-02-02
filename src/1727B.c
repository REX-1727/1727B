/*
 * 1727B.c
 *
 *  Created on: Jan 29, 2016
 *      Author: Anton
 */
#include "1727B.h"


void flywheelInit(flywheel aFlywheel,int (*input)(),
		int (*target)(),
		float kP,
		float kI,
		float kD,
		int outputs[4])
{
	aFlywheel.parameters.input = input;
	aFlywheel.parameters.target = target;
	aFlywheel.parameters.timeOut = -1;
	aFlywheel.parameters.kP = kP;
	aFlywheel.parameters.kI = kI;
	aFlywheel.parameters.kD = kD;
}

void velocityReader(void *ignore)
{
	encoderReset(leftFlywheelEncoder);
	encoderReset(rightFlywheelEncoder);
	unsigned long startTime = 0;
	while(true)
	{
		startTime = millis();
		leftFlywheel.variables.velocityRaw = -encoderGet(leftFlywheelEncoder)*1000/20;
		rightFlywheel.variables.velocityRaw = -encoderGet(rightFlywheelEncoder)*1000/20;
		leftFlywheel.variables.velocity = (leftFlywheel.variables.velocityRaw*(FLYWHEEL_CIRCUMFERENCE/12)/360.0)*3;
		rightFlywheel.variables.velocity = (rightFlywheel.variables.velocityRaw*(FLYWHEEL_CIRCUMFERENCE/12)/360.0)*3;
		encoderReset(leftFlywheelEncoder);
		encoderReset(rightFlywheelEncoder);

		printf("%f\n\r",leftFlywheel.variables.velocity);
		taskDelayUntil(&startTime, MOTOR_REFRESH_TIME);
	}
}



void powerListener(void *params)
{

	if(true)
	{
		while(true)
		{
			if(partner.rightDpad.axisValue == JOY_UP)
			{
				leftFlywheel.variables.power++;
				rightFlywheel.variables.power++;
				taskDelay(100);
			}
			else if(partner.rightDpad.axisValue == JOY_DOWN)
			{
				leftFlywheel.variables.power--;
				rightFlywheel.variables.power--;
				taskDelay(100);
			}
			else if(partner.rightDpad.axisValue == JOY_RIGHT)
			{
				leftFlywheel.variables.power += .5;
				rightFlywheel.variables.power += .5;
				taskDelay(100);
			}
			else if(partner.rightDpad.axisValue == JOY_LEFT)
			{
				leftFlywheel.variables.power -= .5;
				rightFlywheel.variables.power -= .5;
				taskDelay(100);
			}
			else if(partner.leftDpad.axisValue == JOY_UP)
			{
				leftFlywheel.variables.power =29.5;
				rightFlywheel.variables.power =29.5;
				taskDelay(100);
			}
			else if(partner.leftDpad.axisValue == JOY_RIGHT)
			{
				leftFlywheel.variables.power =22;
				rightFlywheel.variables.power =22;
				taskDelay(100);
			}
			else if(partner.leftDpad.axisValue == JOY_LEFT)
			{
				leftFlywheel.variables.power =20;
				rightFlywheel.variables.power =20;
				taskDelay(100);
			}
			else if(partner.leftDpad.axisValue == JOY_DOWN)
			{
				leftFlywheel.variables.power =0;
				rightFlywheel.variables.power =0;
				taskDelay(100);
			}
			if(leftFlywheel.variables.power<0)
			{
				leftFlywheel.variables.power =0;
				rightFlywheel.variables.power =0;
				taskDelay(100);
			}

			rightFlywheel.variables.powerRaw = ((rightFlywheel.variables.power)/3.0)*(12/FLYWHEEL_CIRCUMFERENCE)*360;
			leftFlywheel.variables.powerRaw = ((leftFlywheel.variables.power)/3.0)*(12/FLYWHEEL_CIRCUMFERENCE)*360;

			lcdPrint(uart1,1,"%f",leftFlywheel.variables.power);
			//printf("%f /n /r", leftFlywheel.variables.power);
			taskDelay(20);

		}
	}
}

void driveControl(void *params)
{
	while(true)
	{
		motorSet(R,-main.rightVertical.axisValue);
		motorSet(L,main.leftVertical.axisValue);

		if(partner.rightBumper.axisValue == JOY_UP)
		{
			motorSet(LOWER_INTAKE, 127);
		}
		else if(partner.rightBumper.axisValue == JOY_DOWN)
		{
			motorSet(LOWER_INTAKE, -127);
		}
		else
		{
			motorSet(LOWER_INTAKE, 0);
		}
		if(partner.leftBumper.axisValue == JOY_UP)
		{
			motorSet(UPPER_INTAKE, -127);
		}
		else if(partner.leftBumper.axisValue == JOY_DOWN)
		{
			motorSet(UPPER_INTAKE, 127);
		}
		else
		{
			motorSet(UPPER_INTAKE, 0);
		}
		motorSet(LIFT, partner.rightVertical.axisValue);
		motorSet(LIFT_2, partner.rightVertical.axisValue);


		taskDelay(20);
	}
}

int getRPower()
{
	return (int)(rightFlywheel.variables.powerRaw);
}

int getLPower()
{
	return (int)(leftFlywheel.variables.powerRaw);
}

int getRVel()
{
	return (int)(rightFlywheel.variables.velocityRaw);
}
int getLVel()
{
	return (int)(leftFlywheel.variables.velocityRaw);
}
