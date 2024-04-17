#pragma once
#include "vex.h"

extern PID PID;

double Gyro1;

void ResetDriveMotors();
void StopDriveMotors();

double AvgDriveMtrDist;

bool AutonisRunning = false;

int LDSpeed = 0;
int RDSpeed = 0;