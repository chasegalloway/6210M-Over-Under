#include "vex.h"

void Chase::drive_distance_chase(int velocity, double distance, double heading) {
  drive_distance_chase(velocity, distance, heading, drive_kp_chase, drive_ki_chase, drive_kd_chase);
}

void Chase::drive_distance_chase(int velocity, double distance, double heading, double drive_kp_chase, double drive_ki_chase, double drive_kd_chase) {
  chase.reset_drive_motors();
  task::sleep(10);
  double turn_difference_right;
  while ((fabs(chase.average_distance) < distance) /*&& AutonisRunning*/) { 
    double error = heading -  chase.gyro;
    integral += error;
    double derivative = error - previous_error;
    turn_difference_right = drive_kp_chase * error + drive_ki_chase * integral + drive_kd_chase * derivative;
    chase.lspeed = velocity + turn_difference_right;
    chase.rspeed = velocity - turn_difference_right;
    previous_error = error;
    task::sleep(10);
  }
  chase.stop_drive_motors();
}