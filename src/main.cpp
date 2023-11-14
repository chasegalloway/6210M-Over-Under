// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// RFDrive              motor         5               
// LFDrive              motor         14              
// Controller1          controller                    
// RRDrive              motor         2               
// LRDrive              motor         12              
// Inertial1            inertial      4               
// RCDrive              motor         3               
// LCDrive              motor         13              
// Catapult             motor         21              
// Intake               motor         1               
// Exp                  triport       10              
// Wings                digital_out   A               
// TriBallDetect        optical       19              
// CataRotate           rotation      6               
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

// Sensor Variables
double AvgDriveMtrDist;
double AvgDriveMtrSpeed;
double Gyro1;
int Clock;
int SlowestDrive;
int FastestDrive;

// Drive Train Variables
int LDSpeed = 0;
int RDSpeed = 0;
int DriveTorque = 100;
bool DriveTrainHold = false;
float TurnConstant = 1;

// Intake Variables
bool IntakeTaskRunning = true;
int IntakeSpeed = 0;
int IntakeTorque = 100;

// Catapult Variables
float CatapultSpeed = 60;

// Color Sensor Variables
int colorNumber = 0;
bool isRed = false;
bool TriBallThere = false;
bool startRolling = true;
float CataTensionStorage = true;

// Auton Variables
bool RedSide = true;
bool AutonRunning = false;
int AutonStep = 0;
int AutonSide = 1;
int AutoGryoCorr = 0;
int AutonNumber = 1;
bool AutoHappen = false;
int AutonSelect = 1; // SELECTION OF AUTON ROUTINE 1=AUTON SKILLS , 2=LEFT 2,
                     // 3=LEFT 3, 4=RIGHT 1, 5=RIGHT 2, 6=RIGHT 3,

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
  Inertial1.setRotation(0, deg);
  sleep(5);
}
void SetGyro(int Heading) {
  Inertial1.setRotation(Heading, deg);
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
  LRDrive.resetPosition();
  RRDrive.resetPosition();
  LCDrive.resetPosition();
  RCDrive.resetPosition();
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
  Brain.Screen.clearScreen();
  Brain.Screen.printAt(320, 200, "Pre Auton");

  // RESET DRIVE MOTORS ROTATION
  ResetDriveMotors();

  sleep(100);

  // SET FIELD CONTROL STATE
  FCState = 1;

  Wings.set(false);
  sleep(500);
  Intake.spin(fwd);
  sleep(200);
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
    Brain.Screen.printAt(1, 40, "LRMotor: %5.2f   ", Gyro1);
    Brain.Screen.printAt(188, 40, "RRMotor: %5.2f   ", Gyro1);
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
    Brain.Screen.printAt(320, 170, "RingDumperROT: %5.2f   ");

    if (AutonSelect == 5) {
      Brain.Screen.printAt(1, 210, "AUTON SKILLS");
      Brain.Screen.setFillColor(black);
    } else if (AutonSelect == 1) {
      Brain.Screen.printAt(1, 210, "Auton: Red Square");
      Brain.Screen.setFillColor(red);
    } else if (AutonSelect == 2) {
      Brain.Screen.printAt(1, 210, "Auton: Red Rectangle");
      Brain.Screen.setFillColor(orange);
    } else if (AutonSelect == 3) {
      Brain.Screen.printAt(1, 210, "Auton: Blue Square");
      Brain.Screen.setFillColor(blue);
    } else if (AutonSelect == 4) {
      Brain.Screen.printAt(1, 210, "Auton: Blue Rectangle");
      Brain.Screen.setFillColor(green);
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
    sleep(100);
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Fly: %2.0f ", CatapultSpeed);
    Controller1.Screen.setCursor(1, 12);
    Controller1.Screen.print("Gyro: %3.0f  ", Gyro1);
    Controller1.Screen.setCursor(2, 1);
    Controller1.Screen.print("Auton: %1.0f  ", AutonNumber);
    // Controller1.Screen.setCursor(3, 1);
    // Controller1.Screen.print(ObjThere);
    // Controller1.Screen.setCursor(3, 7);
    // Controller1.Screen.print(isRed);
    Controller1.Screen.clearLine(3);
    Controller1.Screen.setCursor(3, 1);
    Controller1.Screen.print("%2.0f ", Intake.temperature(fahrenheit) / 10);
    Controller1.Screen.setCursor(3, 4);
    Controller1.Screen.print("%2.0f ", LFDrive.temperature(fahrenheit) / 10);
    Controller1.Screen.setCursor(3, 7);
    Controller1.Screen.print("%2.0f ", LCDrive.temperature(fahrenheit) / 10);
    Controller1.Screen.setCursor(3, 10);
    Controller1.Screen.print("%2.0f ", LRDrive.temperature(fahrenheit) / 10);
    Controller1.Screen.setCursor(3, 13);
    Controller1.Screen.print("%2.0f ", RFDrive.temperature(fahrenheit) / 10);
    Controller1.Screen.setCursor(3, 16);
    Controller1.Screen.print("%2.0f ", RCDrive.temperature(fahrenheit) / 10);
    Controller1.Screen.setCursor(3, 19);
    Controller1.Screen.print("%2.0f ", RRDrive.temperature(fahrenheit) / 10);
    Controller1.Screen.setCursor(3, 22);
    Controller1.Screen.print("%2.0f  ", Catapult.temperature(fahrenheit) / 10);
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
    // GET MOTOR ENCODERS AND SCALE THEM TO DISTANCE IN INCHES
    AvgDriveMtrDist = LFDrive.position(deg) +
                      RRDrive.position(deg) * .1 * 14 / 48; // SCALED TO INCHES
    // GET AVERAGE MOTOR SPEED PERCENTAGE
    AvgDriveMtrSpeed = (LFDrive.velocity(pct) + RRDrive.velocity(pct)) * .5;

    // GET GYRO VALUE
    Gyro1 = Inertial1.rotation(deg);

    // GET SLOWEST DRIVE MOTOR SPEED
    x = fabs(RFDrive.velocity(pct));
    if (x > fabs(LFDrive.velocity(pct))) {
      x = fabs(LFDrive.velocity(pct));
    }
    if (x > fabs(LRDrive.velocity(pct))) {
      x = fabs(LRDrive.velocity(pct));
    }
    if (x > fabs(RRDrive.velocity(pct))) {
      x = fabs(RRDrive.velocity(pct));
    }
    if (x > fabs(LCDrive.velocity(pct))) {
      x = fabs(LCDrive.velocity(pct));
    }
    if (x > fabs(RCDrive.velocity(pct))) {
      x = fabs(RCDrive.velocity(pct));
    }
    SlowestDrive = abs(x);

    x = fabs(RFDrive.velocity(pct));
    if (x < fabs(LFDrive.velocity(pct))) {
      x = fabs(LFDrive.velocity(pct));
    }
    if (x < fabs(LRDrive.velocity(pct))) {
      x = fabs(LRDrive.velocity(pct));
    }
    if (x < fabs(RRDrive.velocity(pct))) {
      x = fabs(RRDrive.velocity(pct));
    }
    if (x < fabs(LCDrive.velocity(pct))) {
      x = fabs(LCDrive.velocity(pct));
    }
    if (x < fabs(RCDrive.velocity(pct))) {
      x = fabs(RCDrive.velocity(pct));
    }
    FastestDrive = abs(x);

    // GET BRAIN CLOCK MSEC
    Clock = Brain.timer(msec);

    TriBallThere = TriBallDetect.isNearObject();
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
////////            TASK FOR Catapult               /////////
////////                                            /////////
/////////////////////////////////////////////////////////////
int CatapultTask() {
  while (1) {
    
    sleep(10);
  }
}

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
  ////////           TASK FOR DISK INTAKE             /////////
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
        LRDrive.setStopping(hold);
        RRDrive.setStopping(hold);
        RFDrive.setStopping(hold);
        LCDrive.setStopping(hold);
        RCDrive.setStopping(hold);
      } else {
        LFDrive.setStopping(coast);
        LRDrive.setStopping(coast);
        RRDrive.setStopping(coast);
        RFDrive.setStopping(coast);
        LCDrive.setStopping(coast);
        RCDrive.setStopping(coast);
      }

      LFDrive.setMaxTorque(DriveTorque, pct);
      LRDrive.setMaxTorque(DriveTorque, pct);
      RFDrive.setMaxTorque(DriveTorque, pct);
      RRDrive.setMaxTorque(DriveTorque, pct);
      LCDrive.setMaxTorque(DriveTorque, pct);
      RCDrive.setMaxTorque(DriveTorque, pct);

      LFDrive.spin(fwd, LDSpeed, pct);
      LRDrive.spin(fwd, LDSpeed, pct);
      LCDrive.spin(fwd, LDSpeed, pct);
      RFDrive.spin(fwd, RDSpeed, pct);
      RRDrive.spin(fwd, RDSpeed, pct);
      RCDrive.spin(fwd, RDSpeed, pct);

      sleep(6);
    }
  }
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
    while (fabs(AvgDriveMtrDist) < Distance) {
      RightTurnDiff = (Heading - Gyro1) * .6;
      LDSpeed = Speed + RightTurnDiff;
      RDSpeed = Speed - RightTurnDiff;
      sleep(10);
    }
    StopDriveMotors();
  }

  void AutoTillStop(int Speed, double Heading) {
    ResetDriveMotors();
    ResetTimer();
    sleep(10);
    int RightTurnDiff;
    while (abs(SlowestDrive) > 3 || Clock < 500) {
      RightTurnDiff = (Heading - Gyro1) * .6;
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
    while (fabs((abs(Heading) - fabs(Gyro1))) > Accuracy ||
           fabs(LFDrive.velocity(pct)) > 3.5) {
      LDSpeed = -(Heading - Gyro1);
      RDSpeed = +(Heading - Gyro1);
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

  void ToggleWings() {
    Wings.set(!Wings);
  }

  // L-UP = Intake UP
  void buttonLup_pressed() {  }

  // L-DOWN = Intake DOWN
  void buttonLdown_pressed() { IntakeSpeed = -100; }

  // L-UP RELEASED = Intake STOP
  void buttonLup_released() { IntakeSpeed = 0; }

  // L-DOWN RELEASED = Intake STOP
  void buttonLdown_released() { IntakeSpeed = 0; }

  // R-UP = SHOOT
  void buttonRup_pressed() { IntakeSpeed = 100; }

  // R-DOWN =
  void buttonRdown_pressed() {}

  // R-UP RELEASED =
  void buttonRup_released() {}

  // R-DOWN RELEASED =
  void buttonRdown_released() {}

  // UP =
  void buttonUP_pressed() { }

  // DOWN =
  void buttonDOWN_pressed() { }

  // RIGHT =
  void buttonRIGHT_pressed() { }

  // LEFT =
  void buttonLEFT_pressed() {}

  // A =
  void buttonA_pressed() {
    if (AutonNumber == 1) {
      AutonNumber = 2;
    } else if (AutonNumber == 2) {
      AutonNumber = 3;
    } else if (AutonNumber == 3) {
      AutonNumber = 1;
    } else if (AutonNumber == 4) {
      AutonNumber = 5;
    } else if (AutonNumber == 5) {
      AutonNumber = 1;
    }
  }

  // B =
  void buttonB_pressed() {}

  // X = (JUST ABOVE USERCONTROL)

  // Y = Robot Reset
  void buttonY_pressed() {}

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
 
 void AutonSkills() {
    SetGyro(37);
  }

 void FarSide() {
    SetGyro(37);
  }

 void NearSide() {
    SetGyro(37);
ToggleWings();
sleep(400);
AutoTillStop(-60, 80);
IntakeSpeed = 100;
AutoDistance(60, 10, 40);

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
    AutonRunning = true;
    DriveTrainHold = true;
    if (AutonNumber == 1) {
      FarSide();
    } else if (AutonNumber == 2) {
      NearSide();
    } else if (AutonNumber == 3) {
      AutonSkills();
    } else if (AutonNumber == 4) {

    } else {
    }
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

  void buttonX_pressed() {
    if (!AutoHappen) {
      autonomous();
    }
  }

  void usercontrol() {
    ResetTimer();
    IntakeTaskRunning = true;
    AutonRunning = false;
    DriveTrainHold = false;
    DriveTorque = 100;
    FCState = 4;
    IntakeSpeed = 0;

    while (1) {
      sleep(10);
      if (AutonRunning == false) {
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
      }
    }
  }

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
    task taskCatapult(CatapultTask);
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
