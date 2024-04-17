#include "vex.h"

//PIDs written by Chase are stored here
void drive_distance(int Speed, double Distance, double Heading) {
  double kP = 0.2;
  double kI = 0.06;
  double kD = 0.0;
  double integral = 0;
  double previous_error = 0;

  ResetDriveMotors();
  task::sleep(10);
  double RightTurnDiff;
  while ((fabs(AvgDriveMtrDist) < Distance) && AutonisRunning) {
    double error = Heading - Gyro1;
    integral += error;
    double derivative = error - previous_error;
    RightTurnDiff = kP * error + kI * integral + kD * derivative;
    LDSpeed = Speed + RightTurnDiff;
    RDSpeed = Speed - RightTurnDiff;
    previous_error = error;
    task::sleep(10);
  }
  StopDriveMotors();
}

void turn(int Speed, int Heading, int Accuracy) {
  double lsp;
  double rsp;
  int integral = 0;
  int previous_error = 0;
  double kP = 0.36;
  double kI = 0;
  double kD = 0;

  int new_heading = Heading + Accuracy - 1;
  while ((fabs(new_heading - Gyro1) > Accuracy || (fabs(LFDrive.velocity(pct)) > 2.5)) && AutonisRunning) {
    double error = new_heading - Gyro1;
    integral += error;
    double derivative = error - previous_error;
    double output = kP * error + kI * integral + kD * derivative;
    lsp = +output;
    if (fabs(lsp) < 2) {
      lsp = 2 * fabs(lsp) / lsp;
    }
    LDSpeed = lsp;
    rsp = -output;
    if (fabs(rsp) < 2) {
      rsp = 2 * fabs(rsp) / rsp;
    }
    RDSpeed = rsp;
    previous_error = error;
    task::sleep(5);
  }
  StopDriveMotors();
  task::sleep(10);
}