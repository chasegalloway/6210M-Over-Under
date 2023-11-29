// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// RFDrive              motor         13
// LFDrive              motor         14
// Controller1          controller
// RRDrive              motor         11
// LRDrive              motor         18
// Inertial1            inertial      1
// RCDrive              motor         12
// LCDrive              motor         20
// Catapult             motor         19
// Intake               motor         16
// Exp                  triport       21
// WingsLeft            digital_out   A
// CataRotate           rotation      15
// Blocker              digital_out   D
// Hang                 digital_out   C
// WingsRight           digital_out   B
// LowHang              digital_out   F
// ButtHang             digital_out   G
// TriballDetect        distance      2
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
double AvgDriveMtrDist;
double AvgDriveMtrSpeed;
double Gyro1;
int Clock;
int SlowestDrive;
int FastestDrive;
float TriBallThere;

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
bool CataSlip = true;
bool CatapultSlipPosition = true;
bool CataMode = false;
int CataState = 3;
float CatapultGoal;
int CatapultState = 3;
int MotorStop = 1;
bool XToggle = true;

// Color Sensor Variables
int colorNumber = 0;
bool isRed = false;
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

  WingsLeft.set(false);
  WingsRight.set(false);
 // Blocker.set(false);
  Hang.set(false);
  LowHang.set(false);
  sleep(500);
  Intake.spin(fwd);
  sleep(200);
  CataRotate.setPosition(0, deg);
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
    // Brain.Screen.printAt(320, 170, "RingDumperROT: %5.2f   ");

    if (AutonNumber == 1) {
      Brain.Screen.printAt(1, 210, "Auton: Close Side WP");
      Brain.Screen.setFillColor(red);
    } else if (AutonNumber == 2) {
      Brain.Screen.printAt(1, 210, "Auton: Close Side Elims");
      Brain.Screen.setFillColor(blue);
    } else if (AutonNumber == 3) {
      Brain.Screen.printAt(1, 210, "Auton: Close Side Blocking");
      Brain.Screen.setFillColor( "#008000");
    } else if (AutonNumber == 4) {
      Brain.Screen.printAt(1, 210, "Auton: Far Side WP");
      Brain.Screen.setFillColor( "#403e39");
    } else if (AutonNumber == 5) {
      Brain.Screen.printAt(1, 210, "Auton: Far Side Elims");
      Brain.Screen.setFillColor(purple);
    } else if (AutonNumber == 6) {
      Brain.Screen.printAt(1, 210, "Auton: Skills");
      Brain.Screen.setFillColor( "#fc9e05");
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
    sleep(100);
    //Prints Initrial Heading to Controller
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Gyro: %3.0f  ", Gyro1);
    //Prints Catapult State to Controller
    Controller1.Screen.setCursor(2, 1);
    if (CatapultState == 3){
      Controller1.Screen.print("Cata: PreMatch");
    }
    else if(CatapultState == 2){
      Controller1.Screen.print("Cata: Intake Only");
    }
    else{
     Controller1.Screen.clearLine(2);
     Controller1.Screen.print("Cata: Shoot");
    }
    //Prints Motor Temps to Controller
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
    AvgDriveMtrDist = (LFDrive.position(deg) + RRDrive.position(deg)) * 0.025 *
                      21.5 / 20; // SCALED TO INCHES
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

//Shoots the Catapult
void Shoot() { 
  if (CatapultSlipPosition) {
    CataSlip = false;
    Catapult.setVelocity(100, pct);
    sleep(200);
    CataSlip = true;
    MotorStop = 1;
  }
}

//Task To Change Between Intake and shoot on the catapult
void CataModeState() {
  if (CataMode) {
    CatapultSlipPosition = false;
    CataSlip = false;
    CataMode = false;
    Catapult.setVelocity(100, pct);
    sleep(400);
    CataSlip = true;
    CatapultState = 2;
    MotorStop = 1;
  } else {
    CatapultState = 1;
    MotorStop = 1;
    CatapultSlipPosition = true;
    CataSlip = true;
  }
}

//Catapult Rotation points
int CatapultTask() {
  Catapult.spin(forward);
  while (1) {
    if (CataSlip) {
      Catapult.setVelocity(
          (((CatapultGoal - CataRotate.position(degrees)) * 1.9) * MotorStop),
          percent);
    }
    if (CatapultState == 1) {
      CatapultGoal = 57.5;
    }
    if (CatapultState == 2) {
      CatapultGoal = 50.0;
    }
    if (CatapultState == 3) {
      CatapultGoal = 20.0;
    }
    if ((CataRotate.position(degrees) > 57.0) && CatapultState == 1) {
      MotorStop = 0;
    }
    if ((CataRotate.position(degrees) > 49.0) && CatapultState == 2) {
      MotorStop = 0;
    }
    if ((CataRotate.position(degrees) > 19.0) && CatapultState == 3) {
      MotorStop = 0;
    }
    wait(0.08, seconds);
    wait(5, msec);
  }
  return 0;
}

int TriballDetectTask() {
  while (1) {
    TriBallThere = (TriballDetect.objectDistance(mm));
    if (TriBallThere <= 15) {
      wait(100, msec);
      Shoot();
    }
    sleep(5);
  }
  return 0;
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
    RightTurnDiff = (Heading - Gyro1) * .65;
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
  while (Inertial1.roll() > -10) {
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
  while (fabs((abs(Heading) - fabs(Gyro1))) > Accuracy ||
         fabs(LFDrive.velocity(pct)) > 2.5) {
    lsp = +(Heading - Gyro1) * .5;
    if (fabs(lsp) < 2) {
      lsp = 2 * fabs(lsp) / lsp;
    }
    LDSpeed = lsp;
    rsp = -(Heading - Gyro1) * .5;
    if (fabs(rsp) < 2) {
      rsp = 2 * fabs(rsp) / rsp;
    }
    RDSpeed = rsp;
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

void ToggleButtHang() { ButtHang.set(!ButtHang); }
void ToggleLowHang() { LowHang.set(!LowHang); }
void ToggleLeftWing() { WingsLeft.set(!WingsLeft); }
void ToggleRightWing() {WingsRight.set(!WingsRight); }
void ToggleBlocker() { Blocker.set(!Blocker); }
void ToggleHang() { Hang.set(!Hang); }
void ToggleWingsOut() {
  WingsLeft = true;
  WingsRight = true;
}
void ToggleWingsIn() {
  WingsLeft = false;
  WingsRight = false;
}
// L-UP =
void buttonLup_pressed() { Shoot(); }

// L-DOWN =
void buttonLdown_pressed() { ToggleBlocker(); }

// L-UP RELEASED =
void buttonLup_released() {}

// L-DOWN RELEASED =
void buttonLdown_released() {}

// R-UP = Intake UP
void buttonRup_pressed() { IntakeSpeed = 100; }

// R-DOWN = Intake DOWN
void buttonRdown_pressed() { IntakeSpeed = -100; }

// R-UP RELEASED = Intake STOP
void buttonRup_released() { IntakeSpeed = 0; }

// R-DOWN RELEASED = Intake STOP
void buttonRdown_released() { IntakeSpeed = 0; }

// UP = Wings OUT
void buttonUP_pressed() { ToggleWingsOut(); }

// DOWN = Wings IN
void buttonDOWN_pressed() { ToggleWingsIn(); }

// RIGHT =
void buttonRIGHT_pressed() {
  (Hang = false), (WingsLeft = false), (WingsRight = false);
}

// LEFT =
void buttonLEFT_pressed() {
  if (Clock > 75000 || EndgameButtonCount == 4) {
    ToggleHang(), ToggleBlocker();
  } else {
    EndgameButtonCount += 1;
  }
}
// Brain Pressed =
void brain_pressed() {
  if (AutonNumber == 1) {
    //close side WP
    AutonNumber = 2;
  } else if (AutonNumber == 2) {
    // close side Elims
    AutonNumber = 3;
  } else if (AutonNumber == 3) {
    //close side Blocking
    AutonNumber = 4;
  } else if (AutonNumber == 4) {
    //far side WP
    AutonNumber = 5;
  } else if (AutonNumber == 5) {
    //far side Elims
    AutonNumber = 6;
  }
  else if (AutonNumber == 6) {
    //skills
    AutonNumber = 7;
  }
  else if (AutonNumber == 7) {
    //none
    AutonNumber = 1;
  }
}

// B =
void buttonB_pressed() {
  if (Clock > 75000 || EndgameButtonCount == 4) {
    ToggleLowHang();
  } else {
    EndgameButtonCount += 1;
  }
}

// X =
void buttonX_pressed() {
  CataModeState();
  if (XToggle) {
    CataMode = true;
    XToggle = false;
  } else {
    (CataMode = false), (XToggle = true);
  }
}

// Y = Butt Boost
void buttonY_pressed() {
  if (Clock > 75000 || EndgameButtonCount == 4) {
    ToggleButtHang();
  } else {
    EndgameButtonCount += 1;
  }
}

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

void CloseSideWP() {
  SetGyro(-40);
  ToggleWingsOut();
  sleep(200);
  AutoTurn(80, -90, 10);
  ToggleWingsIn();
  sleep(500);
  AutoTurn(50, -260, 3);
  CatapultState = 2;
  MotorStop = 1;
  IntakeSpeed = -100;
  AutoDistance(60, 20, -260);
  AutoDistance(60, 15, -270);

}

void CloseSideElims() {
  SetGyro(-40);
  ToggleLeftWing();
  sleep(200);
  AutoTurn(80, -90, 10);
  ToggleLeftWing();
  CatapultState = 2;
  MotorStop = 1;
  AutoTurn(80, 25, 2);
  IntakeSpeed = 100;
  AutoDistance(80, 47, 17);
  sleep(400);
  IntakeSpeed = 0;
  AutoDistance(-60, 10, 17);
  AutoTurn(80, 90, 5);
  AutoTillHop(60, 90);
  IntakeSpeed = -100;
  sleep(750);
  IntakeSpeed = 0;
  DriveTorque = 100;
  AutoDistance(-50, 13, 90);
  AutoTurn(50, 220, 3);
  DriveTorque = 20;
  AutoDistance(50, 20, 220);
  AutoTillStop(40, 220);
  DriveTorque = 100;
  AutoTurn(50, 150, 3);
  AutoDistance(50, 6, 145);
  IntakeSpeed = -100;
  AutoDistance(60, 28, 80);
  //ToggleLeftWing();
  //AutoTurn(50, 125, 2);
  sleep(500);
  IntakeSpeed = 0;
  AutoDistance(-60, 20, 80);
  AutoTurn(60, -100, 2);
  DriveTorque = 25;
  AutoTillStop(40, -110);
  MotorStop = 1;
  CatapultState = 1;

}

void AutonSkills() {
  SetGyro(0);
  MotorStop = 1;
  CatapultState = 1;
  AutoTurn(60, -15, 2);
  DriveTorque = 20;
  AutoDistance(50, 3, -20);
  sleep(2); 
  DriveTorque = 100;
  AutoDistance(-80, 3, -20);
  AutoTurn(50, 25, 2);
  AutoDistance(-80, 15, 20);
  AutoDistance(-80, 50, 0);
  AutoDistance(-60, 25, 0);
  AutoTurn(70, -45, 2);
  DriveTorque = 20;
  AutoDistance(-80, 30, -75);
  sleep(100);
  AutoDistance(80, 6, -75);
  AutoDistance(-80, 4, -75);
  sleep(100);
  AutoDistance(80, 8, -90);
  /*AutoTurn(70, -90, 2);
  AutoDistance(-80, -40, -100);
  AutoTurn(70, 180, 2);
  ToggleWingsOut();*/


}

void CloseSideBlock() { 
  SetGyro(0);
  ToggleLeftWing();
  sleep(500);
  ToggleLeftWing();
  CatapultState = 2;
  MotorStop = 1;
  AutoDistance(80, 45, 0);
  IntakeSpeed = 100;
  sleep(2000);
  IntakeSpeed  = 0;
}

void FarSideWP() {
  SetGyro(90);
  MotorStop = 1;
  CatapultState = 2;
  sleep(300);
  IntakeSpeed = 100;
  //ToggleRightWing();
  AutoTurn(60, 45, 3);
  AutoDistance(70, 5, 45);
  AutoTurn(60, 15, 2);
  IntakeSpeed = -100;
  //ToggleRightWing();
  sleep(100);
  AutoDistance(80, 20, 20);
  //ToggleRightWing();
  AutoDistance(-80, 10, 0);
  AutoTurn(60, -65, 1);
  IntakeSpeed = 100;
  AutoDistance(80, 45, -65);
  sleep(100);
  AutoTurn(60, 10, 3);
  //IntakeSpeed = 0;
  AutoDistance(70, 2, 10);
  AutoTurn(60, 90, 3);
  ToggleLeftWing();
  IntakeSpeed = 0;
  /*sleep(50);
  AutoDistance(70, 15, 90);
  IntakeSpeed = -100;
  AutoDistance(70, 10, 90);
  sleep(50);
  AutoDistance(70, 20, 90);
  AutoTurn(60, -20, 3);
  IntakeSpeed = 100;
  AutoDistance(70, 15, 20);
  sleep (100);
  AutoDistance(-70, 15, 20);
  AutoTurn(60, 90, 3);
  AutoDistance(70, 10, 90);
  IntakeSpeed = -100;
  AutoDistance(70, 10, 90);*/




  
}

void FarSideElims() {

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
    CloseSideWP();
  } else if (AutonNumber == 2) {
    CloseSideElims();
  } else if (AutonNumber == 3) {
    CloseSideBlock();
  } else if (AutonNumber == 4) {
    FarSideWP();
  } else if (AutonNumber == 5) {
    FarSideElims();
  } else if (AutonNumber == 6) {
    AutonSkills();
  }
    
}
 void buttonA_pressed() { autonomous(); }
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
   MotorStop = 1;
  ResetTimer();
  IntakeTaskRunning = true;
  AutonRunning = false;
  DriveTrainHold = false;
  WingsLeft = false;
  WingsRight = false;
  DriveTorque = 100;
  FCState = 4;
  IntakeSpeed = 0;

  if (Clock >= 140000) {
    ButtHang = false;
  }

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

      LDSpeed = ControllerAxis3 + ControllerAxis1;
      RDSpeed = ControllerAxis3 - ControllerAxis1;
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
  task taskTriballDetect(TriballDetectTask);
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
