#include "vex.h"

void default_constants(){
  // Each constant set is in the form of (maxVoltage, kP, kI, kD, startI).
  chassis.set_drive_constants(100, 1.5, 100, 100, 0);
  chassis.set_heading_constants(6, .4, 0, 1, 0);
  chassis.set_turn_constants(12, .4, .03, 3, 15);
  chassis.set_swing_constants(12, .3, .001, 2, 15);

  // Each exit condition set is in the form (settle_error, settle_time, timeout).
  chassis.set_drive_exit_conditions(1.5, 300, 5000);
  chassis.set_turn_exit_conditions(1, 300, 3000);
  chassis.set_swing_exit_conditions(1, 300, 3000);
}

void odom_constants(){
  default_constants();
  chassis.drive_max_voltage = 8;
  chassis.drive_settle_error = 3;
}

void CloseSideWP() {
  default_constants();
  // SetGyro(30);
  // IntakeSpeed = 100;
  Kick_Arm = true;
  // sleep(300);
  // IntakeSpeed = 0;
  // chassis.turn_to_angle(-40);
  // sleep(100);
  // Kick_Arm = false;
  // chassis.turn_to_angle(-10);
  // IntakeSpeed = -100;
  //chassis.drive_distance(24, 0, 12, 1.5, 0, 10, 0);
 // chassis.drive_distance(float distance, float heading, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti)
  chassis.turn_to_angle(5);
  // AutoTillDistanceSensor(40, 0);
}


void drive_test(){
  chassis.drive_distance_chase(100, 30, 0);
  // chassis.drive_distance(12);
  // chassis.drive_distance(18);
  // chassis.drive_distance(-36);
}

void turn_test(){
  // chassis.turn_to_angle(5);
  // chassis.turn_to_angle(30);
  chassis.turn_to_angle(90);
  // chassis.turn_to_angle(225);
  // chassis.turn_to_angle(0);
}

void swing_test(){
  chassis.left_swing_to_angle(90);
  chassis.right_swing_to_angle(0);
}

void full_test(){
  // chassis.drive_distance(24);
  // chassis.turn_to_angle(-45);
  // chassis.drive_distance(-36);
  // chassis.right_swing_to_angle(-90);
  // chassis.drive_distance(24);
  // chassis.turn_to_angle(0);
}

void odom_test(){
  chassis.set_coordinates(0, 0, 0);
  while(1){
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(0,50, "X: %f", chassis.get_X_position());
    Brain.Screen.printAt(0,70, "Y: %f", chassis.get_Y_position());
    Brain.Screen.printAt(0,90, "Heading: %f", chassis.get_absolute_heading());
    Brain.Screen.printAt(0,110, "ForwardTracker: %f", chassis.get_ForwardTracker_position());
    Brain.Screen.printAt(0,130, "SidewaysTracker: %f", chassis.get_SidewaysTracker_position());
    task::sleep(20);
  }
}

void tank_odom_test(){
  odom_constants();
  chassis.set_coordinates(0, 0, 0);
  chassis.turn_to_point(24, 24);
  chassis.drive_to_point(24,24);
  chassis.drive_to_point(0,0);
  chassis.turn_to_angle(0);
}

void holonomic_odom_test(){
  odom_constants();
  chassis.set_coordinates(0, 0, 0);
  chassis.holonomic_drive_to_point(0, 18, 90);
  chassis.holonomic_drive_to_point(18, 0, 180);
  chassis.holonomic_drive_to_point(0, 18, 270);
  chassis.holonomic_drive_to_point(0, 0, 0);
}