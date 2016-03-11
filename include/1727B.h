/*
 * 1727B.h
 *
 *  Created on: Jan 29, 2016
 *      Author: Anton
 */

#ifndef _1727B_H_
#define _1727B_H_

#include <API.h>
#include "rexAPI.h"
#include "math.h"

#define RF				3
#define LF				5
#define RB				8
#define LB				10
#define RC				4
#define LC				2
#define LOWER_INTAKE	9
#define UPPER_INTAKE	1



#define FLYWHEEL_CIRCUMFERENCE (5*3.1415926535)


typedef struct pidVars
{
	float velocity;
	float velocityRaw;
	float power;
	float powerRaw;
}pidVars;

typedef struct flywheel
{
	pidVars variables;
	pidParams parameters;
}flywheel;

void flywheelInit(flywheel aFlywheel,int (*input)(), int (*target)(), float kP, float kI, float kD, int outputs[4]);

void velocityReader(void *ignore);

void powerListener(void *params);

void driveControl(void *params);

int getPower();

int getVel();

int getEWMA();

bool twoJoysticks;

flywheel shooter;

TaskHandle shooter_task;

TaskHandle leftFlywheel_task;

TaskHandle powerListener_task;

TaskHandle velocity_task;

TaskHandle drive_task;

TaskHandle joystick_task;

Encoder shooterEncoder;
#endif /* _1727B_H_ */
