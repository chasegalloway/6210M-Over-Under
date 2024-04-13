#include "vex.h"

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// RFDrive              motor         5
// LFDrive              motor         6
// RTDrive              motor         3
// LTDrive              motor         4
// RBDrive              motor         1
// LBDrive              motor         2
// PuncherRight         motor         12
// Intake               motor         21
// PuncherLeft          motor         8
// Inertial11           inertial      11
// Kick_Arm             digital_out   C
// LeftWing             digital_out   D
// Low_Hang             digital_out   E
// High_Hang            digital_out   F
// LeftBackWing         digital_out   A
// Distance_Sensor      distance      10
// RightWing            digital_out   G
// ---- END VEXCODE CONFIGURED DEVICES ----

using namespace vex;
competition Competition;

/*---------------------------------------------------------------------------*/
/*                             VEXcode Config                                */
/*                                                                           */
/*  Before you do anything else, start by configuring your motors and        */
/*  sensors using the V5 port icon in the top right of the screen. Doing     */
/*  so will update robot-config.cpp and robot-config.h automatically, so     */
/*  you don't have to. Ensure that your motors are reversed properly. For    */
/*  the drive, spinning all motors forward should drive the robot forward.   */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                             JAR-Template Config                           */
/*                                                                           */
/*  Where all the magic happens. Follow the instructions below to input      */
/*  all the physical constants and values for your robot. You should         */
/*  already have configured your robot manually with the sidebar configurer. */
/*---------------------------------------------------------------------------*/

// Variables(#1)
// Controller Variables
int FCState = 0;
int ControllerAxis1;
int ControllerAxis2;
int ControllerAxis3;
int ControllerAxis4;
int EndgameButtonCount;

// Sensor Variables
double AvgDriveMtrDist;
double AvgDriveMtrSpeed;
double Gyro1;
int Clock;
int SlowestDrive;
int FastestDrive;
float TriBallThere;
int HangToggle = 1;
int HangDriveSpeedToggle = 0;

// Drive Train Variables
int LDSpeed = 0;
int RDSpeed = 0;
int DriveTorque = 100;
bool DriveTrainHold = false;
float TurnConstant = 1;
int HangDriveSpeed = 0;
int HangDriveSpeedGoal = 0;
bool ControllerLock = true;

// Intake Variables
bool IntakeTaskRunning = true;
int IntakeSpeed = 0;
int IntakeTorque = 100;

// Shooter Variables
bool CataSlip = true;
bool CatapultSlipPosition = true;
bool CataMode = false;
int CataState = 3;
float CatapultGoal;
int CatapultState = 3;
int MotorStop = 1;
bool XToggle = true;
int Puncher_Flywheel = 1;
int Flywheel_Speed = 100;
int Flywheel_State = 9;
int CatapultSpeed = 0;
int ShooterSpeed = 0;
bool ShooterToggle = false;

// Auton Variables
bool RedSide = true;
bool AutonisRunning = false;
int AutonStep = 0;
int AutonSide = 1;
int AutoGryoCorr = 0;
int AutonNumber = 1;
bool AutoHappen = false;
int ToggleAuton = 0;

// Basic Functions(#2)
// Sleep
void sleep(int sleepmsec) { task::sleep(sleepmsec); }

// Resetting Timer
void ResetTimer() {
  Brain.resetTimer();
  sleep(5);
}

// Gyro Stuff
void ResetGyro() {
  Inertial11.setRotation(0, deg);
  sleep(5);
}
void SetGyro(int Heading) {
  Inertial11.setRotation(Heading, deg);
  sleep(5);
}

// Resetting Motor Encoder Positions
void ResetDriveMotors() {
  LFDrive.resetPosition();
  RFDrive.resetPosition();
  LTDrive.resetPosition();
  RTDrive.resetPosition();
  LBDrive.resetPosition();
  RBDrive.resetPosition();
  sleep(5);
}

// Stopping Motors
void StopDriveMotors() {
  LDSpeed = 0;
  RDSpeed = 0;
  LDSpeed = 0;
  RDSpeed = 0;
  sleep(5);
}

void StopAllMotors() {
  LFDrive.stop();
  RFDrive.stop();
  LTDrive.stop();
  RTDrive.stop();
  LBDrive.stop();
  RBDrive.stop();
  Intake.stop();
  PuncherLeft.stop();
  PuncherRight.stop();
  sleep(5);
}

Drive chassis(

    // Specify your drive setup below. There are eight options:
    // ZERO_TRACKER_NO_ODOM, ZERO_TRACKER_ODOM, TANK_ONE_ENCODER,
    // TANK_ONE_ROTATION, TANK_TWO_ENCODER, TANK_TWO_ROTATION,
    // HOLONOMIC_TWO_ENCODER, and HOLONOMIC_TWO_ROTATION For example, if you are
    // not using odometry, put ZERO_TRACKER_NO_ODOM below:
    ZERO_TRACKER_ODOM,

    // Add the names of your Drive motors into the motor groups below, separated
    // by commas, i.e. motor_group(Motor1,Motor2,Motor3). You will input whatever
    // motor names you chose when you configured your robot using the sidebar
    // configurer, they don't have to be "Motor1" and "Motor2".

    // Left Motors:
    motor_group(LFDrive, LTDrive, LBDrive),

    // Right Motors:
    motor_group(RFDrive, RTDrive, RBDrive),

    // Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e.
    // "PORT1", not simply "1"):
    PORT11,

    // Input your wheel diameter. (4" omnis are actually closer to 4.125"):
    3.25,

    // External ratio, must be in decimal, in the format of input teeth/output
    // teeth. If your motor has an 84-tooth gear and your wheel has a 60-tooth
    // gear, this value will be 1.4. If the motor drives the wheel directly, this
    // value is 1:
    0.65,

    // Gyro scale, this is what your gyro reads when you spin the robot 360
    // degrees. For most cases 360 will do fine here, but this scale factor can
    // be very helpful when precision is necessary.
    360,

    /*---------------------------------------------------------------------------*/
    /*                                  PAUSE! */
    /*                                                                           */
    /*  The rest of the drive constructor is for robots using POSITION TRACKING.
     */
    /*  If you are not using position tracking, leave the rest of the values as
     */
    /*  they are. */
    /*---------------------------------------------------------------------------*/

    // If you are using ZERO_TRACKER_ODOM, you ONLY need to adjust the FORWARD
    // TRACKER CENTER DISTANCE.

    // FOR HOLONOMIC DRIVES ONLY: Input your drive motors by position. This is
    // only necessary for holonomic drives, otherwise this section can be left
    // alone. LF:      //RF:
    PORT1, -PORT2,

    // LB:      //RB:
    PORT3, -PORT4,

    // If you are using position tracking, this is the Forward Tracker port (the
    // tracker which runs parallel to the direction of the chassis). If this is a
    // rotation sensor, enter it in "PORT1" format, inputting the port below. If
    // this is an encoder, enter the port as an integer. Triport A will be a "1",
    // Triport B will be a "2", etc.
    3,

    // Input the Forward Tracker diameter (reverse it to make the direction
    // switch):
    2.75,

    // Input Forward Tracker center distance (a positive distance corresponds to
    // a tracker on the right side of the robot, negative is left.) For a zero
    // tracker tank drive with odom, put the positive distance from the center of
    // the robot to the right side of the drive. This distance is in inches:
    -2,

    // Input the Sideways Tracker Port, following the same steps as the Forward
    // Tracker Port:
    1,

    // Sideways tracker diameter (reverse to make the direction switch):
    -2.75,

    // Sideways tracker center distance (positive distance is behind the center
    // of the robot, negative is in front):
    5.5

);

int current_auton_selection = 0;
bool auto_started = false;

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  default_constants();

  FCState = 0;
  Brain.Screen.clearScreen();
  Brain.Screen.printAt(320, 200, "Pre Auton");

  // RESET DRIVE MOTORS ROTATION
  ResetDriveMotors();

  sleep(100);
  LeftWing.set(false);
  RightWing.set(false);
  // Intake_Hold.set(false);
  Low_Hang.set(false);

  Kick_Arm.set(false);
  High_Hang.set(false);
  Controller1.Screen.clearScreen();
  LeftBackWing.set(false);
  // SET FIELD CONTROL STATE
  FCState = 1;
}

// Print Data to the Brain Screen
int BrainScreenTask() {
  while (1) {
    sleep(100);
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(1, 20, "LFMotor: %5.2f   ", Gyro1);
    Brain.Screen.printAt(188, 20, "RFMotor: %5.2f   ", Gyro1);
    Brain.Screen.printAt(1, 40, "LTMotor: %5.2f   ", Gyro1);
    Brain.Screen.printAt(188, 40, "RTMotor: %5.2f   ", Gyro1);
    Brain.Screen.printAt(1, 60, "Avg Motor Dist: %4.2f   ", AvgDriveMtrDist);
    Brain.Screen.printAt(1, 100, "Gyro: %5.2f    ", Gyro1);
    Brain.Screen.printAt(1, 140, "Clock: %d    ", Clock);
    Brain.Screen.printAt(370, 20, "Axis1: %d", ControllerAxis1);
    Brain.Screen.printAt(370, 40, "Axis2: %d", ControllerAxis2);
    Brain.Screen.printAt(370, 60, "Axis3: %d", ControllerAxis3);
    Brain.Screen.printAt(370, 80, "Axis4: %d", ControllerAxis4);

    if (FCState == 0) {
      Brain.Screen.printAt(320, 200, "Pre Auton");
    }
    if (FCState == 1) {
      Brain.Screen.printAt(320, 200, "Pre Auton Done");
    }
    if (FCState == 2) {
      Brain.Screen.printAt(320, 200, "Autonomous");
      Brain.Screen.printAt(320, 220, "Step %d", AutonStep);
    }
    if (FCState == 3) {
      Brain.Screen.printAt(320, 200, "Autonomous Done");
    }
    if (FCState == 4) {
      Brain.Screen.printAt(320, 200, "Driver");
    }
  }
}

// Print Data to the Controller
int CntrlrScreenTask() {

  Controller1.Screen.clearScreen();
  while (1) {
    sleep(50);
    // Prints Initrial Heading to Controller
    Controller1.Screen.setCursor(1, 1);
    // Controller1.Screen.print("Gyro: %3.0f  ", Inertial11.heading());
    Controller1.Screen.print(chassis.leftVelocity); // temp debugging display
    Controller1.Screen.setCursor(2, 1);                     // |
    Controller1.Screen.print(chassis.rightVelocity);//\/
    Controller1.Screen.setCursor(2, 5);                     
    Controller1.Screen.print(chassis.drive_output);
    // Prints Motor Temps to Controller
    Controller1.Screen.clearLine(3);
    Controller1.Screen.setCursor(3, 1);
    Controller1.Screen.print("%2.0f ", Intake.temperature(fahrenheit) / 10);
    Controller1.Screen.setCursor(3, 4);
    Controller1.Screen.print("%2.0f ", LFDrive.temperature(fahrenheit) / 10);
    Controller1.Screen.setCursor(3, 7);
    Controller1.Screen.print("%2.0f ", RFDrive.temperature(fahrenheit) / 10);
    Controller1.Screen.setCursor(3, 10);
    Controller1.Screen.print("%2.0f ", LBDrive.temperature(fahrenheit) / 10);
    Controller1.Screen.setCursor(3, 13);
    Controller1.Screen.print("%2.0f ", RBDrive.temperature(fahrenheit) / 10);
    Controller1.Screen.setCursor(3, 16);
    Controller1.Screen.print("%2.0f ", LTDrive.temperature(fahrenheit) / 10);
    Controller1.Screen.setCursor(3, 19);
    Controller1.Screen.print("%2.0f ", RTDrive.temperature(fahrenheit) / 10);
    Controller1.Screen.setCursor(3, 22);
    Controller1.Screen.print("%2.0f  ", PuncherLeft.temperature(fahrenheit) / 10);
  }
}

// Read values from sensors
int SensorsTask() {
  int x = 100;
  while (1) {
    sleep(5);
    // Get motor encoder values and scale them to rough inches (450 rpm)
    AvgDriveMtrDist = (LFDrive.position(deg) + RTDrive.position(deg)) *
                      0.01235 * 21.5 / 20; // scaled to rough inches
    // Get the average motor speed in percent
    AvgDriveMtrSpeed = (LFDrive.velocity(pct) + RTDrive.velocity(pct)) * .5;

    // Get gyro value
    Gyro1 = Inertial11.rotation(deg);

    // Get slowest drive motor
    x = fabs(RFDrive.velocity(pct));
    if (x > fabs(LFDrive.velocity(pct))) {
      x = fabs(LFDrive.velocity(pct));
    }
    if (x > fabs(LBDrive.velocity(pct))) {
      x = fabs(LBDrive.velocity(pct));
    }
    if (x > fabs(RBDrive.velocity(pct))) {
      x = fabs(RBDrive.velocity(pct));
    }
    if (x > fabs(LTDrive.velocity(pct))) {
      x = fabs(LTDrive.velocity(pct));
    }
    if (x > fabs(RTDrive.velocity(pct))) {
      x = fabs(RTDrive.velocity(pct));
    }
    SlowestDrive = abs(x);

    x = fabs(RFDrive.velocity(pct));
    if (x < fabs(LFDrive.velocity(pct))) {
      x = fabs(LFDrive.velocity(pct));
    }
    if (x < fabs(LBDrive.velocity(pct))) {
      x = fabs(LBDrive.velocity(pct));
    }
    if (x < fabs(RBDrive.velocity(pct))) {
      x = fabs(RBDrive.velocity(pct));
    }
    if (x < fabs(LTDrive.velocity(pct))) {
      x = fabs(LTDrive.velocity(pct));
    }
    if (x < fabs(RTDrive.velocity(pct))) {
      x = fabs(RTDrive.velocity(pct));
    }
    FastestDrive = abs(x);

    // Get brain clock in msec
    Clock = Brain.timer(msec);
  }
}

//Intake Task
int IntakeTask() {
  Intake.spin(forward);
  while (1) {
    sleep(5);
    Intake.setVelocity(IntakeSpeed, pct);
  }
}

// Task for Drive Motors
/*int DriveTask() {
  while (1) {
    if (DriveTrainHold) {
      LFDrive.setStopping(hold);
      LBDrive.setStopping(hold);
      RBDrive.setStopping(hold);
      RFDrive.setStopping(hold);
      LTDrive.setStopping(hold);
      RTDrive.setStopping(hold);
    } else {
      LFDrive.setStopping(coast);
      LBDrive.setStopping(coast);
      RBDrive.setStopping(coast);
      RFDrive.setStopping(coast);
      LTDrive.setStopping(coast);
      RTDrive.setStopping(coast);
    }

    LFDrive.setMaxTorque(DriveTorque, pct);
    LBDrive.setMaxTorque(DriveTorque, pct);
    RFDrive.setMaxTorque(DriveTorque, pct);
    RBDrive.setMaxTorque(DriveTorque, pct);
    LTDrive.setMaxTorque(DriveTorque, pct);
    RTDrive.setMaxTorque(DriveTorque, pct);
  
    LFDrive.spin(fwd, LDSpeed, pct);
    LBDrive.spin(fwd, LDSpeed, pct);
    LTDrive.spin(fwd, LDSpeed, pct);
    RFDrive.spin(fwd, RDSpeed, pct);
    RBDrive.spin(fwd, RDSpeed, pct);
    RTDrive.spin(fwd, RDSpeed, pct);
    
    sleep(6);
  }
}*/


// Buttons
void ToggleKick_Arm() { Kick_Arm.set(!Kick_Arm); }
void ToggleHigh_Hang() { High_Hang.set(!High_Hang); }
void ToggleLowHang() { Low_Hang.set(!Low_Hang); }
void ToggleLeftWing() { LeftWing.set(!LeftWing); }
void ToggleRightWing() { RightWing.set(!RightWing); }
void WingsOut() { LeftWing.set(true), RightWing.set(true); }
void WingsIn() { LeftWing.set(false), RightWing.set(false); }
void BackWingsOut() { LeftWing.set(true), Kick_Arm.set(true); }
void BackWingsIn() { LeftBackWing.set(false), Kick_Arm.set(false); }
void ToggleBackWings() {
  LeftBackWing.set(!LeftBackWing), Kick_Arm.set(!Kick_Arm);
}

// L-UP = Run Puncher
void buttonLup_pressed() {
  PuncherLeft.spin(forward);
  PuncherRight.spin(forward);
  PuncherLeft.setVelocity(60, pct);
  PuncherRight.setVelocity(60, pct);
}

// L-DOWN = Front Wings Out
void buttonLdown_pressed() { WingsOut(); }

// L-UP RELEASED = Stop Puncher
void buttonLup_released() {
  PuncherLeft.stop();
  PuncherRight.stop();
}

// L-DOWN RELEASED = Front Wings In
void buttonLdown_released() { WingsIn(); }

// R-UP = Intake
void buttonRup_pressed() { IntakeSpeed = 100; }

// R-DOWN = Outtake
void buttonRdown_pressed() { IntakeSpeed = -100; }

// R-UP RELEASED = Stop Intake
void buttonRup_released() { IntakeSpeed = 0; }
void buttonRdown_released() { IntakeSpeed = 0; }

// UP = Toggle High Hang Endgame
void buttonUP_pressed() {
  if (Clock > 75000 || EndgameButtonCount == 4) {
    ToggleHigh_Hang();
  } else {
    EndgameButtonCount += 1;
  }
}

// RIGHT = Toggle Right Wing
void buttonRIGHT_pressed() { ToggleRightWing(); }

// LEFT = Toggle Left Wing
void buttonLEFT_pressed() { ToggleLeftWing(); }

// X = Toggle Back Wings
void buttonX_pressed() { ToggleBackWings(); }

// Y = Toggle Low Hang
void buttonY_pressed(){
 if (Clock > 75000 || EndgameButtonCount == 4) {
    ToggleLowHang();
    } 
  else {
    EndgameButtonCount += 1;
    }
  }

// Unused Buttons
void buttonLup_pressed2() {}

void buttonLdown_pressed2() {}

void buttonRup_pressed2() {}

void buttonRdown_pressed2() {}

void buttonRdown_released2() {}

void buttonRup_released2() {}

void buttonB_pressed() {}

void buttonDOWN_pressed() {}

// Autonomous
void autonomous(void) {
  auto_started = true;
  switch (current_auton_selection) {
  case 0:
    drive_test(); //drive_test(); // This is the default auton, if you don't select from the
                  // brain.
    break; // Change these to be your own auton functions in order to use the
           // auton selector.
  case 1: // Tap the screen to cycle through autons.
    drive_test();
    break;
  case 2:
    turn_test();
    break;
  case 3:
    swing_test();
    break;
  case 4:
    full_test();
    break;
  case 5:
    odom_test();
    break;
  case 6:
    tank_odom_test();
    break;
  case 7:
    holonomic_odom_test();
    break;
  }
}

void buttonA_pressed(){
//  ToggleKick_Arm();
  autonomous();
}
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

// User Control
void usercontrol(void) {
  ResetTimer();
  IntakeTaskRunning = true;
  AutonisRunning = false;
  DriveTrainHold = false;
  LeftWing = false;
  RightWing = false;
  DriveTorque = 100;
  FCState = 4;

//  task taskDrive(DriveTask); 
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    // Replace this line with chassis.control_tank(); for tank drive
    // or chassis.control_holonomic(); for holo drive.
    chassis.control_arcade();

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  //Competition.autonomous(autonomous);
  //Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();
  task taskBrainScreen(BrainScreenTask);
  task taskCntrlrScreen(CntrlrScreenTask);
  task taskSensors(SensorsTask);
  task taskIntake(IntakeTask);
  
  Controller1.ButtonL1.pressed(buttonLup_pressed);
  Controller1.ButtonL2.pressed(buttonLdown_pressed);
  Controller1.ButtonL1.released(buttonLup_released);
  Controller1.ButtonL2.released(buttonLdown_released);
  Controller1.ButtonR1.pressed(buttonRup_pressed);
  Controller1.ButtonR2.pressed(buttonRdown_pressed);
  Controller1.ButtonR1.released(buttonRup_released);
  Controller1.ButtonR2.released(buttonRdown_released);
  Controller1.ButtonUp.pressed(buttonUP_pressed);
  Controller1.ButtonDown.pressed(buttonDOWN_pressed);
  Controller1.ButtonRight.pressed(buttonRIGHT_pressed);
  Controller1.ButtonLeft.pressed(buttonLEFT_pressed);
  Controller1.ButtonA.pressed(buttonA_pressed);
  Controller1.ButtonB.pressed(buttonB_pressed);
  Controller1.ButtonX.pressed(buttonX_pressed);
  Controller1.ButtonY.pressed(buttonY_pressed);


}
