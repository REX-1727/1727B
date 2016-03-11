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
float ewma = 0;
float ewmaTPS = 0;

float factor = .5;

float prevTPS[5] = {0, 0, 0, 0, 0};

float prev[5] = {0, 0, 0, 0, 0};
void velocityReader(void *ignore)
{
	encoderReset(shooterEncoder);
	unsigned long startTime = 0;
	while(true)
	{
		startTime = millis();
		shooter.variables.velocityRaw = encoderGet(shooterEncoder)*1000/20;
		shooter.variables.velocity = (shooter.variables.velocityRaw*(FLYWHEEL_CIRCUMFERENCE/12)/360.0);
		encoderReset(shooterEncoder);
		prevTPS[4] = prevTPS[3];
		prevTPS[3] = prevTPS[2];
		prevTPS[2] = prevTPS[1];
		prevTPS[1] = prevTPS[0];
		prevTPS[0] = shooter.variables.velocityRaw;

		prev[4] = prev[3];
		prev[3] = prev[2];
		prev[2] = prev[1];
		prev[1] = prev[0];
		prev[0] = shooter.variables.velocity;

		ewmaTPS = (prevTPS[0] + (prevTPS[1]*pow(factor,1)) + (prevTPS[2]*pow(factor,2)) + (prevTPS[3]*pow(factor,3)) + (prevTPS[4]*pow(factor,4)))/
				(1+ pow(factor,1) + pow(factor,2) + pow(factor,3) + pow(factor,4));
		ewma = (prev[0] + (prev[1]*pow(factor,1)) + (prev[2]*pow(factor,2)) + (prev[3]*pow(factor,3)) + (prev[4]*pow(factor,4)))/
				(1+ pow(factor,1) + pow(factor,2) + pow(factor,3) + pow(factor,4));

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
				shooter.variables.power++;
				taskDelay(100);
			}
			else if(partner.rightDpad.axisValue == JOY_DOWN)
			{
				shooter.variables.power--;
				taskDelay(100);
			}
			else if(partner.rightDpad.axisValue == JOY_RIGHT)
			{
				shooter.variables.power += .5;
				taskDelay(100);
			}
			else if(partner.rightDpad.axisValue == JOY_LEFT)
			{
				shooter.variables.power -= .5;
				taskDelay(100);
			}
			else if(partner.leftDpad.axisValue == JOY_UP)
			{
				shooter.variables.power =40;
				taskDelay(100);
			}
			else if(partner.leftDpad.axisValue == JOY_RIGHT)
			{
				shooter.variables.power =30;
				taskDelay(100);
			}
			else if(partner.leftDpad.axisValue == JOY_LEFT)
			{
				shooter.variables.power =20;
				taskDelay(100);
			}
			else if(partner.leftDpad.axisValue == JOY_DOWN)
			{
				shooter.variables.power =0;
				taskDelay(100);
			}
			if(shooter.variables.power<0)
			{
				shooter.variables.power =0;
				taskDelay(100);
			}

			shooter.variables.powerRaw = ((shooter.variables.power))*(12/FLYWHEEL_CIRCUMFERENCE)*360;

			lcdPrint(uart1,1,"%f",shooter.variables.power);
			lcdPrint(uart1,2,"%f",shooter.variables.velocity);
			printf("%f \n \r", ewma);
			taskDelay(20);

		}
	}
}

void driveControl(void *params)
{
	int rightBack;
	int leftBack;
	int rightFront;
	int leftFront;
	int rightCenter;
	int leftCenter;
	while(true)
	{
		/*rightBack = main.leftVertical.axisValue + main.leftHorizontal.axisValue - main.rightHorizontal.axisValue;
		leftBack = -main.leftVertical.axisValue + main.leftHorizontal.axisValue - main.rightHorizontal.axisValue;
		rightFront = main.leftVertical.axisValue - main.leftHorizontal.axisValue - main.rightHorizontal.axisValue;
		leftFront = main.leftVertical.axisValue + main.leftHorizontal.axisValue + main.rightHorizontal.axisValue;
		rightCenter = main.leftVertical.axisValue;
		leftCenter = main.leftVertical.axisValue;
		motorSet(RB, rightBack);
		motorSet(LB, leftBack);
		motorSet(RF, rightFront);
		motorSet(LF, -leftFront);
		motorSet(RC, rightCenter);
		motorSet(LC, -leftCenter);*/
		motorSet(RB, main.rightVertical.axisValue);
		motorSet(RC, main.rightVertical.axisValue);
		motorSet(RF, main.rightVertical.axisValue);
		motorSet(LB, -main.leftVertical.axisValue);
		motorSet(LC, -main.leftVertical.axisValue);
		motorSet(LF, -main.leftVertical.axisValue);

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



		taskDelay(20);
	}
}

int getPower()
{
	return (int)(shooter.variables.powerRaw);
}


int getVel()
{
	return (int)(shooter.variables.velocityRaw);
}

int getEWMA()
{
	return (int)(ewmaTPS);
}

