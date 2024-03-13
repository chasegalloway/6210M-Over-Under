// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// RFDrive              motor         5               
// LFDrive              motor         6               
// Controller1          controller                    
// RTDrive              motor         3               
// LTDrive              motor         4               
// Inertial11           inertial      11              
// RBDrive              motor         1               
// LBDrive              motor         2               
// PuncherRight         motor         7               
// Intake               motor         21              
// Exp                  triport       12              
// Kick_Arm             digital_out   C               
// Wings                digital_out   D               
// Low_Hang             digital_out   E               
// High_Hang            digital_out   F               
// Intake_Hold          digital_out   G               
// PuncherLeft          motor         8               
// LeftBackWing         digital_out   A               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// RFDrive              motor         20              
// LFDrive              motor         16              
// Controller1          controller                    
// RTDrive              motor         17              
// LTDrive              motor         14              
// Inertial13           inertial      13              
// RBDrive              motor         19              
// LBDrive              motor         15              
// PuncherRight         motor         21              
// Intake               motor         3               
// Exp                  triport       5               
// Lift                 digital_out   A               
// Kick_Arm             digital_out   D               
// Wings                digital_out   C               
// Low_Hang             digital_out   B               
// High_Hang             digital_out   F               
// Intake_Hold          digital_out   E               
// PuncherLeft          motor         6               
// LeftBackWing         digital_out   G               
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
  Inertial11.setRotation(0, deg);
  sleep(5);
}
void SetGyro(int Heading) {
  Inertial11.setRotation(Heading, deg);
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
  LTDrive.resetPosition();
  RTDrive.resetPosition();
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
  Wings.set(false);
  Intake_Hold.set(false);
  Low_Hang.set(false);
 
  Kick_Arm.set(false);
  High_Hang.set(false);
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
    Controller1.Screen.print("Gyro: %3.0f  ", Inertial11.heading());
    // Prints Catapult State to Controller
    Controller1.Screen.setCursor(2, 1);
    Controller1.Screen.clearLine(2);
    if(Puncher_Flywheel == 1){
      if(Flywheel_State == 9) {
        Controller1.Screen.print ("Flywheel Speed: 100");
      }
      else if(Flywheel_State == 8) {
        Controller1.Screen.print ("Flywheel Speed: 95");
      }
      else if(Flywheel_State == 7) {
        Controller1.Screen.print ("Flywheel Speed: 90");
      }
      else if(Flywheel_State == 6) {
        Controller1.Screen.print ("Flywheel Speed: 85");
      }
      else if(Flywheel_State == 5) {
        Controller1.Screen.print ("Flywheel Speed 80");
      }
      else if(Flywheel_State == 4) {
        Controller1.Screen.print ("Flywheel Speed 75");
      }
      else if(Flywheel_State == 3) {
        Controller1.Screen.print ("Flywheel Speed 70");
      }
      else if(Flywheel_State == 2) {
        Controller1.Screen.print ("Flywheel Speed 65");
      }
      else {
        Controller1.Screen.print ("Flywheel Speed 60");
      }
    }
    else {
      Controller1.Screen.print ("Puncher");
    }
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
    // GET MOTOR ENCODERS AND SCALE THEM TO DISTANCE IN INCHES(400 RPM)
    AvgDriveMtrDist = (LFDrive.position(deg) + RTDrive.position(deg)) * 0.0095 *
                      21.5 / 20; // SCALED TO INCHES
    // GET AVERAGE MOTOR SPEED PERCENTAGE
    AvgDriveMtrSpeed = (LFDrive.velocity(pct) + RTDrive.velocity(pct)) * .5;

    // GET GYRO VALUE
    Gyro1 = Inertial11.rotation(deg);

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

int ShooterTask() {
  if(ShooterToggle){
  PuncherLeft.spin(forward);
  PuncherRight.spin(forward);
  PuncherLeft.setVelocity(60, pct);
  PuncherRight.setVelocity(60, pct);
  }
  else{
    PuncherLeft.stop();
    PuncherRight.stop();
  }
  return(0);
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
}

int HangTask() {
 while(1){
  sleep(5);
    if(HangDriveSpeedToggle == 1) {
    
    LDSpeed = (HangDriveSpeedGoal - Inertial11.orientation(roll, degrees));
    RDSpeed = (HangDriveSpeedGoal - Inertial11.orientation(roll, degrees));
    
   }
  }
}

/*int SafeGuard() {
  if(Lift){
    Wings = false;
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
  while (Inertial11.roll() > -10) {
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
  float scaling = .445;
  while (fabs((abs(Heading) - fabs(Gyro1))) > Accuracy ||
         fabs(LFDrive.velocity(pct)) > 2.5) {
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
void ToggleHigh_Hang() { High_Hang.set(!High_Hang); }
void ToggleLowHang() { Low_Hang.set(!Low_Hang); }
void ToggleShooter() { }

// L-UP =
void buttonLup_pressed(){
  PuncherLeft.spin(forward);
  PuncherRight.spin(forward);
  PuncherLeft.setVelocity(60, pct);
  PuncherRight.setVelocity(60, pct);
//  ShooterToggle = !ShooterToggle;


}



// L-DOWN =
void buttonLdown_pressed() {
  Wings = true;
}
// L-UP RELEASED =
void buttonLup_released() {
  PuncherLeft.stop();
  PuncherRight.stop();
}

// L-DOWN RELEASED =
void buttonLdown_released() {
  Wings = false;
}

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

}

// DOWN = Wings IN
void buttonDOWN_pressed() {

}
// RIGHT =
void buttonRIGHT_pressed() {
  Intake_Hold = false;
}

// LEFT = Hang on the Horizontal Bar
void buttonLEFT_pressed() {
 if (Clock > 75000 || EndgameButtonCount == 4) {
    ToggleHigh_Hang();
  } else {
    EndgameButtonCount += 1;
  }
  
}

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
void buttonB_pressed() {
  LeftBackWing = false;
  Kick_Arm = false;

}
// X =
void buttonX_pressed(){ 
LeftBackWing = true;
Kick_Arm = true;
}
// Y = Shooter Selector/Low Hang
void buttonY_pressed(){
 if (Clock > 75000 || EndgameButtonCount == 4) {
    ToggleLowHang();
    } 
  else {
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

// AutoDistance = Speed,Distance,Heading
// AutoTurn = Speed,Heading,Accuracy(Room for error, if its within x of the
// target, you're good.)
void CloseSideWP() {
  SetGyro(30);
  IntakeSpeed = 100;
  ToggleKick_Arm();
  sleep(300);
  IntakeSpeed = 0;
  AutoTurn(60, -40, 2);
  sleep(100);
  ToggleKick_Arm();
  AutoTurn(60, 14, 2);

  IntakeSpeed = -100;
  AutoDistance(60, 27, 14);
  AutoTurn(60, 5, 2);
  AutoDistance(40, 10, 5);
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
  Wings = true;
  AutoDistance(80, 18, 0);
  AutoDistance(-80, 10, 0);
  Wings = false;
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
   SetGyro(52);
  IntakeSpeed = 100;
  AutoTillStop(-70, 90);
  IntakeSpeed = 0;
  AutoDistance(60, 7, 90);
  AutoTurn(60, -26, 4);
  Kick_Arm = true;
  IntakeSpeed = -100;
  PuncherLeft.spin(forward);
  PuncherRight.spin(forward);
  PuncherLeft.setVelocity(60, percent);
  PuncherRight.setVelocity(60, percent);
  sleep(2000);
  PuncherLeft.setVelocity(0, percent);
  PuncherRight.setVelocity(0, percent);
  Kick_Arm = false;
  AutoDistance(70, 44, 0);
  Wings = true;
  AutoTurn(60, -90, 4);
  AutoDistance(80, 65, -88);
  AutoTurn(60, 0, 4);
  AutoTillHop(60, 0);
  sleep(100);
  Wings = false;
  AutoDistance(-80, 25, 0);
  AutoTurn(60, 28, 3);
  AutoDistance(-80, 20, 28);
  AutoTurn(60, -43, 4);
  AutoDistance(80, 25, -32);
  AutoTurn(60, -3, 3);
  Wings = true;
  AutoDistance(80, 73, 0);
  Wings = false; //Remove later
  AutoTurn(60, 36, 2);
  AutoTillStop(80, 87);
  AutoDistance(-70, 8, 90);
  sleep(150);
  AutoTillStop(80, 90);
  AutoDistance(-70, 8, 90);
  AutoTurn(60, 178, 5);
  AutoDistance(80, 42, 172);
  Wings = true; //Remove later
  AutoTurn(60, 58, 5);
  AutoTillStop(70, 0);
  AutoDistance(-70, 15, 5);
  AutoTillStop(70, 5);
  Wings = false;
  AutoDistance(-70, 38, 45);
  AutoTurn(60, -90, 5);
  Kick_Arm  = true;
  LeftBackWing = true;
  AutoDistance(-70, 29, -90);
  AutoTurn(60,  -185, 3);
  AutoTillStop(-70, -180);
  Kick_Arm  = false;
  LeftBackWing = false;
  AutoDistance(70, 36, -180);
  AutoTurn(60, -90, 3);
  AutoDistance(-70, 18, -90);
  AutoTurn(60, -228, 3);
  AutoDistance(-70, 15, -228);
  AutoDistance(70, 15, -228);


  //Working Auto
  /*IntakeSpeed = 100;
  Intake_Hold = true;
  SetGyro(51);
  AutoDistance(-60, 19, 56);
  IntakeSpeed = 0;
  AutoTurn(60, 90, 5);
  AutoDistance(-60, 8, 90);
  AutoDistance(60, 7, 90);
  AutoTurn(60, -26, 3);
  AutoDistance(-30, 1, -26);
  Kick_Arm = true;
  PuncherLeft.setVelocity(60, percent);
  PuncherRight.setVelocity(60, percent);
  PuncherLeft.spin(forward);
  PuncherRight.spin(forward);
  IntakeSpeed = -100;
  sleep(28000);
  PuncherLeft.stop();
  PuncherRight.stop();
  Kick_Arm = false;
  IntakeSpeed = 0; 
  AutoTurn(60, 32, 5);
  IntakeSpeed = -100;
  AutoDistance(80, 38, 30);
  AutoTurn(60, 0, 5);
  AutoDistance(80, 55, 0);
  AutoTurn(60, -30, 4);
  AutoDistance(80, 28, -30);
  AutoTurn(60, -82, 3);
  AutoTillStop(60, -85);
  sleep(75); 
  AutoDistance(-60, 10, -85);
  sleep(75);
  AutoTillStop(60, -82);
  AutoDistance(-60, 12, -82);
  sleep(75);
  AutoTurn(60, -165, 4);
  AutoDistance(60, 46, -165);
  AutoTurn(60, -90, 3);
  AutoDistance(70, 14, -90);
  Wings = true;
  AutoTurn(60, 0, 3);
  IntakeSpeed = -100;
  AutoTillStop(60, 0);
  sleep(75);
  Wings = false;
  IntakeSpeed = 0;
  AutoDistance(-70, 38, 0);
  AutoTurn(60, -90, 3);
  AutoDistance(60, 3, -90);
  AutoTurn(60, 0, 4);
  Wings = true; 
  AutoTillStop(60, 0);
  IntakeSpeed = -100;
  sleep(75);
  Wings = false;
  AutoDistance(-60, 38, 0);
  AutoTurn(60, -90, 5);
  AutoDistance(60, 8, -90);
  AutoTurn(60, 17, 5);
  Wings = true;
  AutoTillStop(60, 15);
  sleep(75);
  Wings = false;
  AutoDistance(-60, 30, 15);
  AutoTurn(60, -50, 4);
  AutoDistance(70, 48, -50);
  Wings = true;
  AutoTurn(60, 55, 2);
  AutoDistance(70, 20, 55);
  Wings = false;
  AutoTurn(70, 90, 2);
  AutoTillStop(70, 90);
  sleep(150);
  AutoDistance(-70, 10, 90);
  AutoTillStop(70, 90);
  sleep(150);
  AutoDistance(-70, 10, 90);
  AutoTillStop(70, 90);
  sleep(150);
  AutoDistance(-70, 13, 90);
  AutoTurn(60, 55, 2);
  AutoDistance(-70, 12, 55);
  AutoTurn(60, 0, 5);
  AutoDistance(-80, 40, 0);*/

  /*Intake_Hold = true;
  SetGyro(0);
  AutoTurn(60, 45, 2);
  AutoDistance(-70, 2, 45);  
  AutoTurn(60, -15, 2);
  AutoDistance(-30, 2, -30);
  Kick_Arm = true;
  PuncherLeft.setVelocity(60, percent);
  PuncherRight.setVelocity(60, percent);
  PuncherLeft.spin(forward);
  PuncherRight.spin(forward);
  IntakeSpeed = -100;
  sleep(35000);
  PuncherLeft.setVelocity(0, percent);
  PuncherRight.setVelocity(0, percent);
  IntakeSpeed = 0;
  Kick_Arm = false;
  AutoDistance(70, 2, -35);
  AutoTurn(60, 47, 2);
  AutoDistance(70, 16, 45);
  AutoTurn(60, 0, 5);
AutoDistance(80, 73, 0);
AutoTurn(60, 125, 2);
AutoDistance(-80, 30, 120); //Crash into right barricade/
Intake_Hold = true;
AutoDistance(80, 5, 90);
AutoTurn(90, 90, 3);
AutoDistance(-80, 7, 90);

AutoDistance(70, 6, 90);
AutoTurn(-90, -135, 4);
//sleep(200);
AutoDistance(70, 40, -140);//Manuever to front-right goal/
Wings = true;
AutoDistance(70, 41 , 0); //Crash into front-right goal/
sleep(100);
Wings = false;
AutoDistance(-70, 15, 0); //Manuever to front-left goal/
AutoTurn(90, -90, 5); //Turn left to get to front-left goal
AutoDistance(90, 6, -90);
AutoTurn(90, 20, 6); //Turn to face the goal again
Wings = true;
AutoDistance(70, 27, 20);//Crash into front-left goal/
//sleep(200);
AutoDistance(-70, 28, 0); //Manuever to left barricade/
Wings = false;
AutoTurn(60, -52, 5);
AutoDistance(70, 62, -52);
AutoTurn(60, -120, 2);
AutoDistance(-90, 32, -120);
AutoDistance(90, 12, -90);
AutoDistance(70, 12, -90);
AutoDistance(70, 14, -90);
AutoTurn(80, -130, 5);
AutoDistance(60, 30, -145);*/
}

void FarSideWP(){
  SetGyro(0);
  Intake_Hold = true;
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
  //IntakeSpeed = -100;
  AutoTillStop(-60, -80);
  IntakeSpeed = 100;
  AutoDistance(60, 8, -90 );
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
  Wings = true;
  AutoDistance(70, 25, -173);
  AutoTillStop(70, -173);
  AutoDistance(-70, 20, -173);
   Wings = false;


  

}
void FarSideElims() {
  SetGyro(0);
  Intake_Hold = false;
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

void FarSideSafe(){
  SetGyro(0);
  IntakeSpeed = 100;
  sleep(400);
  IntakeSpeed = 0;
  AutoTillStop(60, 0);
  AutoDistance(-60, 10, 0); 
}
void AutonSkill2(){
  SetGyro(0);
  AutoTurn(80, 45, 2);
  AutoDistance(-80, 10, 45);  
  AutoTurn(60, -19, 2);
  Kick_Arm = true;
  ShooterSpeed = 100;
  sleep(1000);
  ShooterSpeed = 0;
  Kick_Arm = false;
  AutoDistance(80, 3, -20);
  AutoTurn(60, 45, 5);
  AutoDistance(-80, 6, 45);
  AutoTurn(60, 90, 5);
  AutoDistance(-100, 4, 90);
  sleep(100);
  AutoDistance(100, 7, 90);
  
  AutoTurn(60, -25, 2);
  AutoDistance(80, 48,-25);
  AutoTurn(60, -90, 5);
  IntakeSpeed = -100;
  Wings = true;
  AutoDistance(80, 70, -90);
  AutoDistance(-80, 5, -90);
  AutoDistance(80, 5, -90);
  sleep(100);
  Wings = false;
  //AutoDistance(-100, 5, -90);
  AutoTurn(60, 175, 5);
  AutoDistance(80, 10, 180);
  AutoTurn(60, 200, 2);
  AutoDistance(80, 8, 200);
  AutoTurn(-60, -45, 5);
  AutoDistance(80, 15, -45);
  AutoTurn(60, 0, 5);
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
  AutonSkills(); //  CloseSideWP();
  } else if (AutonNumber == 2) {
    CloseSideElims();
  } else if (AutonNumber == 3) {
    FarSideSafe();
  } else if (AutonNumber == 4) {
    FarSideWP();
  } else if (AutonNumber == 5) {
    FarSideElims();
  } else if (AutonNumber == 6) {
    AutonSkills();
  }
}

void buttonA_pressed(){
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
  AutonRunning = false;
  DriveTrainHold = false;
  Wings =false;
  DriveTorque = 100;
  FCState = 4;
  IntakeSpeed = 0;
  Intake_Hold = true;
  

  while (1) {
    sleep(10);
    if (AutonRunning == false && ControllerLock == true) {
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
  task taskHang(HangTask);
  task taskIntake(IntakeTask);
  //task taskSafeGuard(SafeGuard);
  //task taskShooter(ShooterTask);
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