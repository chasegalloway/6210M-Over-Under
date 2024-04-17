// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// RFDrive              motor         11
// LFDrive              motor         20
// Controller1          controller
// RMDrive              motor         12
// LMDrive              motor         19
// Inertial15           inertial      15
// RBDrive              motor         13
// LBDrive              motor         17
// Puncher              motor         8
// Intake               motor         21
// Kick_Arm             digital_out   B
// LeftWing             digital_out   D
// Hang_PTO             digital_out   E
// Hang_Release         digital_out   F
// LeftBackWing         digital_out   A
// DistanceSensor       distance      16
// RightWing            digital_out   C
// HangPosition         rotation      3
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
using namespace vex;
vex::competition Competition;

///////////////////////////////////////////////////////////////
////////                                              /////////
////////               GLOBAL VARIABLES               /////////
////////                                              /////////
///////////////////////////////////////////////////////////////

// Controller Variables
int FCState = 0;
int ControllerAxis1;
int ControllerAxis2;
int ControllerAxis3;
int ControllerAxis4;
int EndgameButtonCount;

// Sensor Variables
//double AvgDriveMtrDist;
double AvgDriveMtrSpeed;
// double Gyro1;
int Clock;
int SlowestDrive;
int FastestDrive;
float TriBallThere;
int HangToggle = 1;
int HangDriveSpeedToggle = 0;
// Drive Train Variables
// int LDSpeed = 0;
// int RDSpeed = 0;
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
// bool AutonisRunning = false;
int AutonStep = 0;
int AutonSide = 1;
int AutoGryoCorr = 0;
int AutonNumber = 1;
bool AutoHappen = false;
int ToggleAuton = 0;

///////////////////////////////////////////////////////////////
//                                                           //
//           ##########                                      //
//           ##                                              //
//           ##                                              //
//           #########                                       //
//           ##                                              //
//           ##                                              //
//           ##                                              //
//                                                           //
///////////////////////////////////////////////////////////////
////////                                              /////////
////////              FUNCTION TO SLEEP               /////////
////////                                              /////////
///////////////////////////////////////////////////////////////
void sleep(int sleepmsec) { task::sleep(sleepmsec); }

///////////////////////////////////////////////////////////////
////////                                              /////////
////////           FUNCTION FOR TIMER RESET           /////////
////////                                              /////////
///////////////////////////////////////////////////////////////
void ResetTimer() {
  Brain.resetTimer();
  sleep(5);
}

///////////////////////////////////////////////////////////////
////////                                              /////////
////////           FUNCTION FOR GYRO RESET            /////////
////////                                              /////////
///////////////////////////////////////////////////////////////
void ResetGyro() {
  Inertial15.setRotation(0, deg);
  sleep(5);
}
void SetGyro(int Heading) {
  Inertial15.setRotation(Heading, deg);
  sleep(5);
}

///////////////////////////////////////////////////////////////
////////                                              /////////
////////  FUNCTION FOR RESETING DRIVE MOTOR ENCODERS  /////////
////////                                              /////////
///////////////////////////////////////////////////////////////
void ResetDriveMotors() {
  LFDrive.resetPosition();
  RFDrive.resetPosition();
  LMDrive.resetPosition();
  RMDrive.resetPosition();
  LBDrive.resetPosition();
  RBDrive.resetPosition();
  sleep(5);
}

///////////////////////////////////////////////////////////////
////////                                              /////////
////////      FUNCTION FOR STOPPING DRIVE MOTORS      /////////
////////                                              /////////
///////////////////////////////////////////////////////////////
void StopDriveMotors() {
  LDSpeed = 0;
  RDSpeed = 0;
  LDSpeed = 0;
  RDSpeed = 0;
  sleep(5);
}

///////////////////////////////////////////////////////////////
////////                                              /////////
////////      FUNCTION FOR STOPPING All MOTORS        /////////
////////                                              /////////
///////////////////////////////////////////////////////////////
void StopAllMotors() {
  LFDrive.stop();
  RFDrive.stop();
  LMDrive.stop();
  RMDrive.stop();
  LBDrive.stop();
  RBDrive.stop();
  Intake.stop();
  Puncher.stop();
  sleep(5);
}

///////////////////////////////////////////////////////////////
//                                                           //
//           #########         ###                           //
//           ##      ##      ##   ##                         //
//           ##      ##     ##     ##                        //
//           #########     ###########                       //
//           ##            ##       ##                       //
//           ##            ##       ##                       //
//           ##            ##       ##                       //
//                                                           //
///////////////////////////////////////////////////////////////
////////                                              /////////
////////      FUNCTION FOR PREPARING FOR AUTON        /////////
////////                                              /////////
///////////////////////////////////////////////////////////////
void pre_auton() {
  // PRINT TO BRIAN SCREEN "Pre Auton"
  FCState = 0;
  Brain.Screen.clearScreen();
  Brain.Screen.printAt(320, 200, "Pre Auton");

  // RESET DRIVE MOTORS ROTATION
  ResetDriveMotors();

  sleep(100);
  LeftWing.set(false);
  RightWing.set(false);
  // Intake_Hold.set(false);
  // Low_Hang.set(false);

  Kick_Arm.set(false);
  // High_Hang.set(false);
  Controller1.Screen.clearScreen();
  LeftBackWing.set(false);
  // SET FIELD CONTROL STATE
  FCState = 1;
}

///////////////////////////////////////////////////////////////
//                                                           //
//          ############     ########                        //
//               ##          ##      ##                      //
//               ##          ##      ##                      //
//               ##          ########                        //
//               ##          ##      ##                      //
//               ##          ##      ##                      //
//               ##          ########                        //
//                                                           //
///////////////////////////////////////////////////////////////
////////                                              /////////
////////     TASK TO PRINT DATA TO BRAIN'S SCREEN     /////////
////////                                              /////////
///////////////////////////////////////////////////////////////
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
    Brain.Screen.printAt(1, 170, "       ");
    Brain.Screen.printAt(370, 20, "Axis1: %d", ControllerAxis1);
    Brain.Screen.printAt(370, 40, "Axis2: %d", ControllerAxis2);
    Brain.Screen.printAt(370, 60, "Axis3: %d", ControllerAxis3);
    Brain.Screen.printAt(370, 80, "Axis4: %d", ControllerAxis4);
    Brain.Screen.printAt(320, 110, "Reflect: %3.2f  ");
    Brain.Screen.printAt(320, 150, "PotR: %3.2f   ");
    // Brain.Screen.printAt(320, 170, "RingDumperROT: %5.2f   ");

    if (AutonNumber == 1) {
      Brain.Screen.printAt(1, 210, "Auton: Close Side WP");
      Brain.Screen.setFillColor(red);
    } else if (AutonNumber == 2) {
      Brain.Screen.printAt(1, 210, "Auton: Close Side Elims");
      Brain.Screen.setFillColor(blue);
    } else if (AutonNumber == 3) {
      Brain.Screen.printAt(1, 210, "Auton: Far Side Safe");
      Brain.Screen.setFillColor("#008000");
    } else if (AutonNumber == 4) {
      Brain.Screen.printAt(1, 210, "Auton: Far Side WP");
      Brain.Screen.setFillColor("#403e39");
    } else if (AutonNumber == 5) {
      Brain.Screen.printAt(1, 210, "Auton: Far Side Elims");
      Brain.Screen.setFillColor(purple);
    } else if (AutonNumber == 6) {
      Brain.Screen.printAt(1, 210, "Auton: Skills");
      Brain.Screen.setFillColor("#fc9e05");
    } else if (AutonNumber == 7) {
      Brain.Screen.printAt(1, 210, "Auton: None");
      Brain.Screen.setFillColor(black);
    }
  }

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

///////////////////////////////////////////////////////////////
//                                                           //
//          ############      #######                        //
//               ##         ##                               //
//               ##         ##                               //
//               ##         ##                               //
//               ##         ##                               //
//               ##         ##                               //
//               ##           #######                        //
//                                                           //
///////////////////////////////////////////////////////////////
////////                                              /////////
////////  TASK TO PRINT DATA TO Controller'S SCREEN   /////////
////////                                              /////////
///////////////////////////////////////////////////////////////
int CntrlrScreenTask() {

  Controller1.Screen.clearScreen();
  while (1) {
    sleep(50);
    // Prints Initrial Heading to Controller
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Gyro: %3.0f  ", Inertial15.heading());
    // Prints Catapult State to Controller
    Controller1.Screen.setCursor(2, 1);
    Controller1.Screen.clearLine(2);
    Controller1.Screen.print(ToggleAuton);
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
    Controller1.Screen.print("%2.0f ", LMDrive.temperature(fahrenheit) / 10);
    Controller1.Screen.setCursor(3, 19);
    Controller1.Screen.print("%2.0f ", RMDrive.temperature(fahrenheit) / 10);
    Controller1.Screen.setCursor(3, 22);
    Controller1.Screen.print("%2.0f  ", Puncher.temperature(fahrenheit) / 10);
  }
}

///////////////////////////////////////////////////////////////
//                                                           //
//          ############     ########                        //
//               ##         ##                               //
//               ##         ##                               //
//               ##          ########                        //
//               ##                 ##                       //
//               ##                 ##                       //
//               ##          ########                        //
//                                                           //
///////////////////////////////////////////////////////////////
////////                                              /////////
////////       TASK TO READ VALUES FROM SENSORS       /////////
////////                                              /////////
///////////////////////////////////////////////////////////////
int SensorsTask() {
  int x = 100;
  while (1) {
    sleep(5);
    // GET MOTOR ENCODERS AND SCALE THEM TO DISTANCE IN INCHES(450 RPM)
    AvgDriveMtrDist = (LFDrive.position(deg) + RMDrive.position(deg)) * 0.0134;

    // GET AVERAGE MOTOR SPEED PERCENTAGE
    AvgDriveMtrSpeed = (LFDrive.velocity(pct) + RMDrive.velocity(pct)) * .5;

    // GET GYRO VALUE
    Gyro1 = Inertial15.rotation(deg);

    // GET SLOWEST DRIVE MOTOR SPEED
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
    if (x > fabs(LMDrive.velocity(pct))) {
      x = fabs(LMDrive.velocity(pct));
    }
    if (x > fabs(RMDrive.velocity(pct))) {
      x = fabs(RMDrive.velocity(pct));
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
    if (x < fabs(LMDrive.velocity(pct))) {
      x = fabs(LMDrive.velocity(pct));
    }
    if (x < fabs(RMDrive.velocity(pct))) {
      x = fabs(RMDrive.velocity(pct));
    }
    FastestDrive = abs(x);

    // GET BRAIN CLOCK MSEC
    Clock = Brain.timer(msec);
  }
}

/////////////////////////////////////////////////////////////
//                                                         //
//          ############      #######                      //
//               ##         ##                             //
//               ##         ##                             //
//               ##         ##                             //
//               ##         ##                             //
//               ##         ##                             //
//               ##          ########                      //
//                                                         //
/////////////////////////////////////////////////////////////
////////                                            /////////
////////            TASK FOR Shooter                /////////
////////                                            /////////
/////////////////////////////////////////////////////////////

/*int ShooterTask() {
  if(ShooterToggle){
  Puncher.spin(forward);
  PuncherRight.spin(forward);
  Puncher.setVelocity(60, pct);
  PuncherRight.setVelocity(60, pct);
  }
  else{
    Puncher.stop();
    PuncherRight.stop();
  }
  return(0);
}*/

/////////////////////////////////////////////////////////////
//                                                         //
//              ############         ##                    //
//                   ##              ##                    //
//                   ##              ##                    //
//                   ##              ##                    //
//                   ##              ##                    //
//                   ##              ##                    //
//                   ##              ##                    //
//                                                         //
/////////////////////////////////////////////////////////////
////////                                            /////////
////////           TASK FOR INTAKE                  /////////
////////                                            /////////
/////////////////////////////////////////////////////////////
int IntakeTask() {
  Intake.spin(forward);
  while (1) {
    sleep(5);
    Intake.setVelocity(IntakeSpeed, pct);
  }
}

///////////////////////////////////////////////////////////////
//                                                           //
//          ############     ########                        //
//               ##          ##      ##                      //
//               ##          ##       ##                     //
//               ##          ##       ##                     //
//               ##          ##       ##                     //
//               ##          ##      ##                      //
//               ##          ########                        //
//                                                           //
///////////////////////////////////////////////////////////////
////////                                              /////////
////////             TASK FOR DRIVE MOTOR             /////////
////////                                              /////////
///////////////////////////////////////////////////////////////

int DriveTask() {
  while (1) {
    if (DriveTrainHold) {
      LFDrive.setStopping(hold);
      LBDrive.setStopping(hold);
      RBDrive.setStopping(hold);
      RFDrive.setStopping(hold);
      LMDrive.setStopping(hold);
      RMDrive.setStopping(hold);
    } else {
      LFDrive.setStopping(coast);
      LBDrive.setStopping(coast);
      RBDrive.setStopping(coast);
      RFDrive.setStopping(coast);
      LMDrive.setStopping(coast);
      RMDrive.setStopping(coast);
    }

    LFDrive.setMaxTorque(DriveTorque, pct);
    LBDrive.setMaxTorque(DriveTorque, pct);
    RFDrive.setMaxTorque(DriveTorque, pct);
    RBDrive.setMaxTorque(DriveTorque, pct);
    LMDrive.setMaxTorque(DriveTorque, pct);
    RMDrive.setMaxTorque(DriveTorque, pct);

    LFDrive.spin(fwd, LDSpeed, pct);
    LBDrive.spin(fwd, LDSpeed, pct);
    LMDrive.spin(fwd, LDSpeed, pct);
    RFDrive.spin(fwd, RDSpeed, pct);
    RBDrive.spin(fwd, RDSpeed, pct);
    RMDrive.spin(fwd, RDSpeed, pct);

    sleep(6);
  }
}

int HangTask() {
  while (1) {
    sleep(5);
    if (HangDriveSpeedToggle == 1) {

      LDSpeed = (HangDriveSpeedGoal - Inertial15.orientation(roll, degrees));
      RDSpeed = (HangDriveSpeedGoal - Inertial15.orientation(roll, degrees));
    }
  }
}

/*int SafeGuard() {
  if(Lift){
    WingsIn();
    Kick_Arm = false;
    LeftBackWing = false;
  }
  return(0);
}*/
/////////////////////////////////////////////////////////////
//                                                         //
//           #########        ###                          //
//           ##             ##   ##                        //
//           ##            ##     ##                       //
//           ########     ###########                      //
//           ##           ##       ##                      //
//           ##           ##       ##                      //
//           ##           ##       ##                      //
//                                                         //
/////////////////////////////////////////////////////////////
////////                                            /////////
////////          FUNCTION FOR AUTO DISTANCE        /////////
////////                                            /////////
/////////////////////////////////////////////////////////////
/* If you want to keep spinning after drive do 1xxx = Speed (xxx being
the speed you want) */
void AutoDistance(int Speed, double Distance, double Heading) {
  ResetDriveMotors();
  sleep(10);
  int RightTurnDiff;
  while ((fabs(AvgDriveMtrDist) < Distance) && AutonisRunning) {
    RightTurnDiff = (Heading - Gyro1) * .65;
    LDSpeed = Speed + RightTurnDiff;
    RDSpeed = Speed - RightTurnDiff;
    sleep(10);
  }
  StopDriveMotors();
}

void drive_distance(int Speed, double Distance, double Heading) {
  double kP = 0.2;
  double kI = 0.06;
  double kD = 0.0;
  double integral = 0;
  double previous_error = 0;

  ResetDriveMotors();
  sleep(10);
  double RightTurnDiff;
  while ((fabs(AvgDriveMtrDist) < Distance) && AutonisRunning) {
    double error = Heading - Gyro1;
    integral += error;
    double derivative = error - previous_error;
    RightTurnDiff = kP * error + kI * integral + kD * derivative;
    LDSpeed = Speed + RightTurnDiff;
    RDSpeed = Speed - RightTurnDiff;
    previous_error = error;
    sleep(10);
  }
  StopDriveMotors();
}

void AutoTillStop(int Speed, double Heading) {
  ResetDriveMotors();
  ResetTimer();
  sleep(10);
  int RightTurnDiff;
  while ((SlowestDrive > 3 || Clock < 500) && AutonisRunning) {
    RightTurnDiff = (Heading - Gyro1) * .65;
    LDSpeed = Speed + RightTurnDiff;
    RDSpeed = Speed - RightTurnDiff;
    sleep(10);
  }
  StopDriveMotors();
}

void AutoTillUnderBar(int Speed, double Heading) {
  ResetDriveMotors();
  ResetTimer();
  sleep(10);
  int RightTurnDiff;
  while (((DistanceSensor.objectDistance(mm)) >= 60) && AutonisRunning) {
    RightTurnDiff = (Heading - Gyro1) * .65;
    LDSpeed = Speed + RightTurnDiff;
    RDSpeed = Speed - RightTurnDiff;
    sleep(10);
  }

  StopDriveMotors();
}

void AutoTillHop(int Speed, double Heading) {
  ResetDriveMotors();
  ResetTimer();
  sleep(10);
  int RightTurnDiff;
  while ((Inertial15.roll() > -10) && AutonisRunning) {
    RightTurnDiff = (Heading - Gyro1) * .65;
    LDSpeed = Speed + RightTurnDiff;
    RDSpeed = Speed - RightTurnDiff;
    sleep(10);
  }
  StopDriveMotors();
}

/////////////////////////////////////////////////////////////
////////                                            /////////
////////          FUNCTION FOR AUTO TURN            /////////
////////                                            /////////
/////////////////////////////////////////////////////////////
void AutoTurn(int Speed, int Heading, int Accuracy) {
  float lsp;
  float rsp;
  float scaling = 0.37;
  while ((fabs(Heading - Gyro1) > Accuracy ||
          (fabs(LFDrive.velocity(pct)) > 2.5)) &&
         AutonisRunning) {
    lsp = +(Heading - Gyro1) * scaling;
    if (fabs(lsp) < 2) {
      lsp = 2 * fabs(lsp) / lsp;
    }
    LDSpeed = lsp;
    rsp = -(Heading - Gyro1) * scaling;
    if (fabs(rsp) < 2) {
      rsp = 2 * fabs(rsp) / rsp;
    }
    RDSpeed = rsp;
    sleep(5);
  }

  StopDriveMotors();
  sleep(10);
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
    double derivative_turn = error - previous_error;
    double output = kP * error + kI * integral + kD * derivative_turn;
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
    sleep(5);
  }

  StopDriveMotors();
  sleep(10);
}
///////////////////////////////////////////////////////////////
////////                                            ///////////
////////           FUNCTION FOR AUTO-DRIVE          ///////////
////////                                            ///////////
///////////////////////////////////////////////////////////////
void AutoDrive(int Forward, int RightTurn) {
  LDSpeed = Forward - RightTurn;
  RDSpeed = Forward + RightTurn;
  sleep(5);
}

/////////////////////////////////////////////////////////////
//                                                         //
//                ########                                 //
//                ##      ##                               //
//                ##      ##                               //
//                ########                                 //
//                ##      ##                               //
//                ##      ##                               //
//                ########                                 //
//                                                         //
/////////////////////////////////////////////////////////////
////////                                            /////////
////////            FUNCTION FOR BUTTONS            /////////
////////                                            /////////
/////////////////////////////////////////////////////////////

void ToggleKick_Arm() { Kick_Arm.set(!Kick_Arm); }
void ToggleHigh_Hang() { /*High_Hang.set(!High_Hang)*/
  ;
}
void ToggleLowHang() { /*Low_Hang.set(!Low_Hang)*/
  ;
}
void ToggleLeftWing() { LeftWing.set(!LeftWing); }
void ToggleRightWing() { RightWing.set(!RightWing); }
void WingsOut() { LeftWing.set(true), RightWing.set(true); }
void WingsIn() { LeftWing.set(false), RightWing.set(false); }
void BackWingsOut() { LeftWing.set(true), Kick_Arm.set(true); }
void BackWingsIn() { LeftBackWing.set(false), Kick_Arm.set(false); }
void ToggleBackWings() {
  LeftBackWing.set(!LeftBackWing), Kick_Arm.set(!Kick_Arm);
}

// L-UP =
void buttonLup_pressed() {
  Puncher.spin(forward);
  Puncher.setVelocity(60, pct);
}

// L-DOWN =
void buttonLdown_pressed() { WingsOut(); }
// L-UP RELEASED =
void buttonLup_released() { Puncher.stop(); }

// L-DOWN RELEASED =
void buttonLdown_released() { WingsIn(); }

// R-UP = Intake UP
void buttonRup_pressed() { IntakeSpeed = 100; }

// R-DOWN = Intake DOWN
void buttonRdown_pressed() { IntakeSpeed = -100; }

// R-UP RELEASED = Intake STOP
void buttonRup_released() { IntakeSpeed = 0; }

// R-DOWN RELEASED = Intake STOP
void buttonRdown_released() { IntakeSpeed = 0; }

// UP = Wings OUT
void buttonUP_pressed() {
  if (Clock > 75000 || EndgameButtonCount == 4) {
    ToggleHigh_Hang();
  } else {
    EndgameButtonCount += 1;
  }
}

// RIGHT =
void buttonRIGHT_pressed() { ToggleRightWing(); }

// LEFT = Hang on the Horizontal Bar
void buttonLEFT_pressed() { ToggleLeftWing(); }

// Brain Pressed =
void brain_pressed() {
  if (AutonNumber == 1) {
    // close side WP
    AutonNumber = 2;
  } else if (AutonNumber == 2) {
    // close side Elims
    AutonNumber = 3;
  } else if (AutonNumber == 3) {
    // close side Blocking
    AutonNumber = 4;
  } else if (AutonNumber == 4) {
    // far side WP
    AutonNumber = 5;
  } else if (AutonNumber == 5) {
    // far side Elims
    AutonNumber = 6;
  } else if (AutonNumber == 6) {
    // skills
    AutonNumber = 7;
  } else if (AutonNumber == 7) {
    // none
    AutonNumber = 1;
  }
}

// B =Shooter Select/ Flywheel Speed Up

// X =
void buttonX_pressed() { ToggleBackWings(); }
// Y = Shooter Selector/Low Hang
void buttonY_pressed() {
  if (Clock > 75000 || EndgameButtonCount == 4) {
    ToggleLowHang();
  } else {
    EndgameButtonCount += 1;
  }
}

// A = (found under Auton subfunctions)

void buttonLup_pressed2() {}

void buttonLdown_pressed2() {}

void buttonRup_pressed2() {}

void buttonRdown_pressed2() {}

void buttonRdown_released2() {}

void buttonRup_released2() {}

/////////////////////////////////////////////////////////////
//                                                         //
//                    #####                                //
//                   ##   ##                               //
//                  ##     ##                              //
//                 ###########                             //
//                 ##       ##                             //
//                 ##       ##                             //
//                 ##       ##                             //
//                                                         //
/////////////////////////////////////////////////////////////
////////                                            /////////
////////           AUTONOMOUS SUBFUNCTIONS          /////////
////////                                            /////////
/////////////////////////////////////////////////////////////

void PIDTest() {
  SetGyro(0);
  drive_distance(-100, 50, 0);
  turn(100, 90, 3);
}

void CloseSideSafeWP() {
  SetGyro(30);
  IntakeSpeed = 100;
  ToggleKick_Arm();
  sleep(300);
  IntakeSpeed = 0;
  turn(100, -40, 2);
  sleep(100);
  ToggleKick_Arm();
  turn(100, 15, 2);
  IntakeSpeed = -100;
  drive_distance(80, 27, 14);
  turn(100, 0, 2);
  AutoTillUnderBar(40, 0);
}

void CloseSideWP() {
  SetGyro(17);
  IntakeSpeed = 100;
  drive_distance(100, 54, 17);
  sleep(250);
  drive_distance(-60, 59, 17);
  sleep(100);
  turn(70, 105, 3);
  drive_distance(-60, 10, 105);
  IntakeSpeed = -100;
  Kick_Arm = true;
  turn(100, 65, 2);
  sleep(100);
  Kick_Arm = false;
  turn(100, 80, 2);
  IntakeSpeed = -100;
  drive_distance(80, 27, 80);
  turn(100, 90, 2);
  AutoTillUnderBar(40, 0);
}

void CloseSideElims() {
  SetGyro(30);
  IntakeSpeed = 100;
  ToggleKick_Arm();
  sleep(100);
  IntakeSpeed = 0;
  AutoTurn(60, 5, 2);
  ToggleKick_Arm();
  sleep(200);
  AutoTurn(60, -62, 2);
  AutoDistance(80, 38, -62);
  AutoDistance(60, 8, -62);
  AutoTurn(60, -90, 2);
  AutoDistance(60, 4, -90);
  IntakeSpeed = 100;
  sleep(600);
  IntakeSpeed = 25;
  AutoDistance(-40, 3, -90);
  AutoTurn(60, 0, 2);
  WingsOut();
  AutoDistance(80, 18, 0);
  AutoDistance(-80, 10, 0);
  WingsIn();
  sleep(200);
  AutoTurn(60, 120, 3);
  AutoDistance(80, 40, 120);
  DriveTorque = 20;
  AutoTillStop(60, 120);
  DriveTorque = 100;
  AutoTurn(60, 15, 2);
  AutoDistance(80, 15, 0);
  IntakeSpeed = -100;
  AutoDistance(60, 17, 0);
  sleep(500);
  AutoDistance(-80, 35, 0);
  /*SetGyro(30);
  Intake_Hold = false;
  ToggleKick_Arm();
  AutoTurn(60, 0, 2);
  ToggleKick_Arm();
  IntakeSpeed = 100;
  AutoDistance(60, 38, 0);
  AutoTurn(60, -65, 2);
  AutoDistance(60, 50, -65);
  AutoTurn(60, -90, 2);
  IntakeSpeed =-100;
  AutoDistance(60, 4, -85);
  sleep(1000);
  IntakeSpeed = 0;*/
}

void AutonSkills() {
  // Auton Notes
  // Aiden G: Scores very well down center but pushes tri balls far off to the
  // right and de-scores in excess - 3/14/24 4:46 PM
  SetGyro(52);
  IntakeSpeed = 100;
  AutoTillStop(-70, 90);
  IntakeSpeed = 0;
  AutoDistance(60, 7, 90);
  AutoTurn(60, -26, 5);
  Kick_Arm = true;
  IntakeSpeed = -100;
  Puncher.spin(forward);
  Puncher.setVelocity(60, percent);
  sleep(2000); // 20000 Milliseconds = 20 seconds :O
  Puncher.setVelocity(0, percent);
  Kick_Arm = false;
  AutoDistance(80, 40, 0);
  WingsOut();
  AutoTurn(60, -90, 5);
  AutoDistance(90, 57, -88);
  AutoTurn(60, 0, 5);
  AutoTillHop(80, 0); // uncomment later
  // AutoDistance(80, 8, 0); //remove later
  sleep(100);
  WingsIn();
  AutoDistance(-80, 25, 0);
  AutoTurn(60, 28, 5);
  AutoDistance(-80, 20, 28);
  AutoTurn(60, -43, 5);
  AutoDistance(80, 16, -32); //(25) (Consider reducing distance here)
  AutoTurn(60, -3, 5);
  //  WingsOut();
  AutoDistance(80, 78, 0); // Run left lane (73)
  //  WingsIn(); //Remove later
  AutoTurn(60, 36, 2);
  RightWing = true;
  AutoTillStop(80, 78); // Hit left goal 1
  AutoDistance(-80, 8, 90);
  RightWing = false;
  AutoTurn(60, 90, 3);
  sleep(100);
  AutoTillStop(80, 90); // Hit left goal 2
  RightWing = false;
  AutoDistance(-80, 8, 90);
  AutoTurn(60, 178, 5);
  IntakeSpeed = 0;
  AutoDistance(80, 42, 172);
  // Original Position of the wings
  RightWing = true;
  AutoTurn(60, 58, 5);
  LeftWing = true; // Wings that moved (Aiden G: done to prevent tri-balls being
                   // pushed to wrong places)
  IntakeSpeed = -100;
  AutoTillStop(80, 0); // Front left hit
  AutoDistance(-80, 15, 5);
  AutoTillStop(80, 5); // Front left hit 2
  WingsIn();
  AutoDistance(-80, 36, 45);
  AutoTurn(60, 90, 5);
  AutoDistance(80, 30, 90); // Tun and reves to move tri=balls
  IntakeSpeed = -100;
  DriveTorque = 10;
  AutoTillStop(70, 90);
  AutoDistance(-70, 3, 90);
  DriveTorque = 100;
  AutoTurn(60, 180, 3);
  LeftBackWing = true;
  AutoDistance(-70, 5, 180);
  AutoTurn(60, 70, 3);
  Kick_Arm = true;
  AutoDistance(-70, 20, 70);
  LeftBackWing = false;
  Kick_Arm = false;
  AutoDistance(70, 5, 70);
  AutoTurn(60, 180, 5);
  AutoDistance(70, 18, 180);
  AutoTurn(60, 270, 5);
  AutoDistance(70, 23, 270);
  AutoTurn(60, 360, 5);
  WingsOut();
  AutoTillStop(70, 360);
  AutoDistance(-80, 6, 360);
  WingsIn();
  AutoTurn(60, 270, 4);
  AutoDistance(-70, 46,
               270); // Move to far-right side of the goal (Aiden G: Consider
                     // going close to wall to catch more tri-balls)
  //  AutoTurn(60, 0, 5);
  //  AutoDistance(70, 3, 0);
  AutoTurn(60, 180, 4);
  Kick_Arm = true;
  AutoTillStop(-70, 270);
}

void AutonSkills2() {
  // Auton Notes
  // Aiden G: Scores very well down center but pushes tri balls far off to the
  // right and de-scores in excess - 3/14/24 4:46 PM
  SetGyro(52);
  IntakeSpeed = 100;
  AutoTillStop(-70, 90);
  IntakeSpeed = 0;
  AutoDistance(60, 7, 90);
  AutoTurn(60, -26, 3);
  Kick_Arm = true;
  IntakeSpeed = -100;
  Puncher.spin(forward);
  Puncher.setVelocity(60, percent);
  sleep(23000);
  Puncher.setVelocity(0, percent);
  Kick_Arm = false;
  AutoDistance(80, 40, 0);
  WingsOut();
  AutoTurn(60, -90, 5);
  AutoDistance(90, 57, -88);
  AutoTurn(60, 0, 5);
  AutoTillHop(80, 0);
  sleep(100);
  WingsIn();
  AutoDistance(-80, 25, 0);
  AutoTurn(60, 28, 5);
  AutoDistance(-80, 20, 28);
  AutoTurn(60, -43, 5);
  AutoDistance(80, 16, -32);
  AutoTurn(60, -3, 5);
  AutoDistance(80, 78, 0); // Run left lane
  AutoTurn(60, 36, 3);
  RightWing = true;
  AutoTillStop(80, 82); // Hit left goal 1
  AutoDistance(-80, 8, 90);
  sleep(100);
  AutoTurn(70, 90, 5);
  AutoTillStop(80, 90); // Hit left goal 2
  RightWing = false;
  AutoDistance(-80, 8, 90);
  AutoTurn(60, 178, 5);
  IntakeSpeed = 0;
  AutoDistance(80, 45, 172);
  RightWing = true;
  AutoTurn(60, 58, 5);
  LeftWing = true; // Wings that moved (Aiden G: done to prevent tri-balls being
                   // pushed to wrong places)
  AutoTillStop(80, 0); // Front left hit
  AutoDistance(-80, 15, 5);
  AutoTillStop(80, 5); // Front left hit 2
  WingsIn();
  AutoDistance(-80, 36, 43);
  AutoTurn(60, -90, 5);
  LeftBackWing = true;
  AutoDistance(-80, 29, -90); // Turn and reverse to move triballs
  AutoTurn(60, -185, 5);
  Kick_Arm = true;
  AutoTillStop(-80, -180); // Reverse to middle
  Kick_Arm = false;
  LeftBackWing = false;
  AutoDistance(80, 36, -180); // Move turn and reverse to center triballs
  AutoTurn(60, -90, 5);
  AutoDistance(-80, 18, -90); // Move to score center
  AutoTurn(60, -235, 5);
  LeftBackWing = true;
  Kick_Arm = true;
  AutoDistance(-80, 27, -235); // Reverse to center triballs
  LeftBackWing = false;
  Kick_Arm = false;
  AutoDistance(80, 27, -235);
  AutoTurn(60, -90, 5);
  AutoDistance(80, 20, -90);
  AutoTurn(60, 0, 5);
  WingsOut();
  AutoTillStop(80, 0);
  WingsIn();
  AutoDistance(-80, 12, 0);
  AutoTurn(60, -90, 4);
  AutoDistance(-70, 58,
               -90); // Move to far-right side of the goal (Aiden G: Consider
                     // going close to wall to catch more tri-balls)
  //  AutoTurn(60, 0, 5);
  //  AutoDistance(70, 3, 0);
  AutoTurn(60, -225, 4);
  Kick_Arm = true;
  AutoDistance(-70, 9, -225);
  AutoTillStop(-70, -280);
  AutoDistance(-70, 5, -270);
}

void FarSideWP() {
  SetGyro(0);
  IntakeSpeed = 100;
  sleep(300);
  AutoDistance(-70, 35, 0);
  AutoTurn(60, -30, 3);
  LeftBackWing = true;
  AutoDistance(-60, 14, -30);
  AutoTurn(45, -90, 3);
  sleep(75);
  LeftBackWing = false;
  AutoTurn(60, -80, 4);
  // IntakeSpeed = -100;
  AutoTillStop(-60, -80);
  IntakeSpeed = 100;
  AutoDistance(60, 8, -90);
  AutoTurn(60, -252, 3);
  sleep(75);
  IntakeSpeed = -100;
  AutoTillStop(60, -255);
  AutoDistance(-60, 13, -265);
  AutoTurn(60, -340, 4);
  IntakeSpeed = 100;
  AutoDistance(80, 49, -338);
  sleep(250);
  AutoTurn(60, -195, 4);
  AutoDistance(70, 15, -197);
  IntakeSpeed = -100;
  sleep(400);
  AutoTurn(60, -320, 4);
  IntakeSpeed = 100;
  AutoDistance(70, 23, -320);
  sleep(400);
  AutoTurn(60, -173, 4);
  IntakeSpeed = -100;
  WingsOut();
  AutoDistance(70, 25, -173);
  AutoTillStop(70, -173);
  AutoDistance(-70, 20, -173);
  WingsIn();
}
void FarSideElims() {
  SetGyro(0);
  IntakeSpeed = 100;
  sleep(500);
  IntakeSpeed = 0;
  AutoDistance(-70, 34, 0);
  AutoTurn(50, -38, 2);
  LeftBackWing = true;
  AutoDistance(-40, 14, -38);
  AutoTurn(50, -90, 2);
  sleep(200);
  LeftBackWing = false;
  AutoTurn(50, -70, 2);
  AutoDistance(-70, 15, -70);
  AutoTurn(50, -90, 2);
  AutoDistance(-70, 7, -90);
}

void FarSideSafe() {
  SetGyro(0);
  IntakeSpeed = 100;
  sleep(400);
  IntakeSpeed = 0;
  AutoTillStop(60, 0);
  AutoDistance(-60, 10, 0);
}

/////////////////////////////////////////////////////////////
//                                                         //
//                    #####                                //
//                   ##   ##                               //
//                  ##     ##                              //
//                 ###########                             //
//                 ##       ##                             //
//                 ##       ##                             //
//                 ##       ##                             //
//                                                         //
/////////////////////////////////////////////////////////////
////////                                            /////////
////////          FEILD CONTROL AUTONOMOUS          /////////
////////                                            /////////
/////////////////////////////////////////////////////////////

void autonomous() {
  AutoHappen = true;
  AutonisRunning = true;
  DriveTrainHold = true;
  if (AutonNumber == 1) {
    CloseSideWP();
  } else if (AutonNumber == 2) {
    CloseSideElims();
  } else if (AutonNumber == 3) {
    FarSideSafe();
  } else if (AutonNumber == 4) {
    FarSideWP();
  } else if (AutonNumber == 5) {
    FarSideElims();
  } else if (AutonNumber == 6) {
    AutonSkills2();
  }
}

void buttonA_pressed() {
  //  ToggleKick_Arm();
  autonomous();
}
/////////////////////////////////////////////////////////////
//                                                         //
//              ##        ##                               //
//              ##        ##                               //
//              ##        ##                               //
//              ##        ##                               //
//              ##        ##                               //
//               ##      ##                                //
//                 ######                                  //
//                                                         //
/////////////////////////////////////////////////////////////
////////                                            /////////
////////           FUNCTION FOR USERCONTROL         /////////
////////                                            /////////
/////////////////////////////////////////////////////////////

void usercontrol() {
  ResetTimer();
  IntakeTaskRunning = true;
  AutonisRunning = false;
  DriveTrainHold = false;
  LeftWing = false;
  RightWing = false;
  DriveTorque = 100;
  FCState = 4;
  IntakeSpeed = 0;
  StopAllMotors();

  while (1) {
    sleep(10);
    if (AutonisRunning == false) {
      ControllerAxis1 = Controller1.Axis1.value();
      if (abs(ControllerAxis1) < 15) {
        ControllerAxis1 = 0;
      }
      ControllerAxis2 = Controller1.Axis2.value();
      if (abs(ControllerAxis2) < 15) {
        ControllerAxis2 = 0;
      }
      ControllerAxis3 = Controller1.Axis3.value();
      if (abs(ControllerAxis3) < 15) {
        ControllerAxis3 = 0;
      }
      ControllerAxis4 = Controller1.Axis4.value();
      if (abs(ControllerAxis4) < 15) {
        ControllerAxis4 = 0;
      }

      LDSpeed = ControllerAxis3 + ControllerAxis1;
      RDSpeed = ControllerAxis3 - ControllerAxis1;
    }
  }
}

void buttonB_pressed() {}

void buttonDOWN_pressed() {}

/////////////////////////////////////////////////////////////
////////                                            /////////
////////         MAIN PROGRAM (START TASKS)         /////////
////////                                            /////////
/////////////////////////////////////////////////////////////
int main() {
  pre_auton();
  task taskBrainScreen(BrainScreenTask);
  task taskCntrlrScreen(CntrlrScreenTask);
  task taskSensors(SensorsTask);
  task taskDrive(DriveTask);
  task taskHang(HangTask);
  task taskIntake(IntakeTask);
  Brain.Screen.pressed(brain_pressed);
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