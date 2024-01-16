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
// Shooter              motor         21              
// Intake               motor         3               
// Exp                  triport       5               
// Lift                 digital_out   A               
// Kick_Arm             digital_out   D               
// Wings                digital_out   C               
// Low_Hang             digital_out   B               
// Hang_PTO             digital_out   F               
// Intake_Hold          digital_out   E               
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
// Shooter              motor         21              
// Intake               motor         3               
// Exp                  triport       5               
// Lift                 digital_out   A               
// Kick_Arm             digital_out   D               
// Wings                digital_out   C               
// Low_Hang             digital_out   B               
// Hang_PTO             digital_out   F               
// Intake_Hold          digital_out   E               
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
  Inertial13.setRotation(0, deg);
  sleep(5);
}
void SetGyro(int Heading) {
  Inertial13.setRotation(Heading, deg);
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
  Intake_Hold.set(true);
  Low_Hang.set(false);
  Lift.set(false);
  Kick_Arm.set(false);
  Hang_PTO.set(false);
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1,1);
  Controller1.Screen.print("Flywheel(B)");
  Controller1.Screen.setCursor(2,1);
  Controller1.Screen.print("Puncher(Y)");
  waitUntil(Controller1.ButtonB.pressing()||Controller1.ButtonY.pressing());
  Controller1.Screen.clearScreen();
  // SET FIELD CONTROL STATE
  FCState = 1;

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
      Brain.Screen.printAt(1, 210, "Auton: Close Side Blocking");
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
    Controller1.Screen.print("Gyro: %3.0f  ", Inertial13.roll());
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
    Controller1.Screen.print("%2.0f  ", Shooter.temperature(fahrenheit) / 10);
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
    AvgDriveMtrDist = (LFDrive.position(deg) + RTDrive.position(deg)) * 0.0095 *
                      21.5 / 20; // SCALED TO INCHES
    // GET AVERAGE MOTOR SPEED PERCENTAGE
    AvgDriveMtrSpeed = (LFDrive.velocity(pct) + RTDrive.velocity(pct)) * .5;

    // GET GYRO VALUE
    Gyro1 = Inertial13.rotation(deg);

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
  while (1) {
    //if the shooter is a flywheel
    if (Puncher_Flywheel == 1) {
    Shooter.setVelocity((0.5 * Shooter.velocity(percent) + 0.5 * Flywheel_Speed), percent);
      if (Flywheel_State >= 9 ){
        Flywheel_Speed = 100;
        Flywheel_State = 9;      
        }
      else if (Flywheel_State == 8) {
        Flywheel_Speed = 95;
      }
      else if (Flywheel_State == 7) {
        Flywheel_Speed = 90;
      }
      else if (Flywheel_State == 6) {
        Flywheel_Speed = 85;
      }
      else if (Flywheel_State == 5) {
        Flywheel_Speed = 80;
      }
      else if (Flywheel_State == 4) {
        Flywheel_Speed =75;
      }
      else if (Flywheel_State == 3) {
        Flywheel_Speed = 70;
      }
      else if (Flywheel_State == 2) {
        Flywheel_Speed = 65;
      }
      else {
        Flywheel_Speed = 60;
        Flywheel_State = 1;
      }
    }
     //if puncher is attached 
    else  {
     sleep(5);
     Shooter.setVelocity(CatapultSpeed, pct);
    }
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
    
    LDSpeed = (HangDriveSpeedGoal - Inertial13.orientation(roll, degrees));
    RDSpeed = (HangDriveSpeedGoal - Inertial13.orientation(roll, degrees));
    
   }
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
  while (Inertial13.roll() > -10) {
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
  float scaling = .45;
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

void ToggleLift() { Lift.set(!Lift); }
void ToggleKick_Arm() { Kick_Arm.set(!Kick_Arm); }
void ToggleHangPTO() { Hang_PTO.set(!Hang_PTO); }

// L-UP =
void buttonLup_pressed() {
  Shooter.setVelocity(100, percent);
  Shooter.spin(forward);
}



// L-DOWN =
void buttonLdown_pressed() {
  ToggleKick_Arm();
  sleep(50);
   ToggleLift();
}
// L-UP RELEASED =
void buttonLup_released() {
  Shooter.stop();
}

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
void buttonUP_pressed() {
  Wings = true;
}

// DOWN = Wings IN
void buttonDOWN_pressed() {
  Wings = false;
}
// RIGHT =
//void buttonRIGHT_pressed(); 

// LEFT = Hang on the Horizontal Bar
void buttonLEFT_pressed() {
 if (Clock > 75000 || EndgameButtonCount == 4) {
    if (HangToggle == 1) {
      HangToggle = 2;
      Lift = true;
    } else {
      Lift = false;
      ControllerLock = false;
      Hang_PTO = true;
      LDSpeed = 100;
      RDSpeed = 100;
      sleep(200);
      while(LTDrive.velocity(pct)>2){
        sleep (10);
      }
      Hang_PTO = false;
      LDSpeed = -10;
      RDSpeed = -10;
      sleep (100);
      LDSpeed = 0;
      RDSpeed = 0;
      HangDriveSpeedGoal = 0;
      HangDriveSpeedToggle = 1;


    }
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
  if(FCState == 0) {
  Puncher_Flywheel = 1;
  }
  else {
  Flywheel_State += -1;
  }
}
// X =
void buttonX_pressed(){ 
Flywheel_State += 1;
}

// Y = Shooter Selector/Low Hang
void buttonY_pressed(){
  if(FCState == 0){
    Puncher_Flywheel = 2;
  }
  else {
    if (Clock > 75000 || EndgameButtonCount == 4) {
    Kick_Arm = true;
    sleep(50);
    Low_Hang = true;
    } 
    else {
    EndgameButtonCount += 1;
    }
  }
}

// A =



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
  Intake_Hold = false;
  ToggleKick_Arm();
  sleep(300);
  AutoTurn(60, 5, 2);
  ToggleKick_Arm();
  IntakeSpeed = -100;
  AutoDistance(60, 38, 5);
}

void CloseSideElims() {
  SetGyro(30);
  Intake_Hold = false;
  ToggleKick_Arm();
  sleep(100);
  AutoTurn(60, 0, 2);
  ToggleKick_Arm();
  sleep(100);
  AutoTurn(60, -65, 2);
  AutoDistance(80, 40, -65);
  AutoDistance(60, 9, -65);
  AutoTurn(60, -90, 2);
  IntakeSpeed = 100;
  sleep(500);
  IntakeSpeed = 0;
  AutoDistance(-60, 3, -90);
  AutoTurn(60, 0, 2);
  Wings = true;
  AutoDistance(80, 20, 0);
  AutoDistance(-80, 10, 0);
  Wings = false;
  AutoTurn(60, 120, 3);
  AutoDistance(80, 50, 120);
  DriveTorque = 20;
  AutoTillStop(60, 120);
  DriveTorque = 100;
  AutoTurn(60, 15, 2);
  AutoDistance(80, 15, 0);
  IntakeSpeed = -100;
  AutoDistance(60, 17, 0);
  sleep(500);
  AutoDistance(-80, 40, 0);
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
  SetGyro(0);
  DriveTorque = 100;
  MotorStop = 1;
  CatapultState = 1;
  AutoTurn(60, -10, 2);
  AutoDistance(30, 3, -15);
  sleep(37000);
  AutoDistance(-80, 3, -20);
  AutoTurn(60, 20, 2);
  AutoDistance(-80, 18, 20);
  AutoTurn(60, 0, 2);
  AutoDistance(-80, 50, 0);
  AutoDistance(-60, 19, 0);
  IntakeSpeed = -100;
  AutoTurn(60, -50, 2);
  AutoDistance(-80, 35, -50);
  sleep(50);
  AutoDistance(80, 10, -50);
  /*AutoDistance(-80, 6, -90);
  sleep(100);
  AutoDistance(80, 8, -90);*/
  AutoTurn(60, 25, 2);
  AutoDistance(80, 46, 25);
  AutoTurn(60, 160, 2);
  //ToggleWingsOut();
  DriveTorque = 100;
  AutoDistance(70, 30, 170);
  sleep(100);
  //ToggleWingsIn();
  AutoDistance(-80, 30, 180);
  AutoTurn(60, 90, 2);
  AutoDistance(80, 7, 90);
  AutoTurn(60, 180, 2);
  //ToggleWingsOut();
  AutoDistance(70, 30, 180);
  sleep(100);
  //ToggleWingsIn();
  AutoDistance(-80, 25, 180);
  AutoTurn(60, 90, 3);
  AutoDistance(80, 5, 90);
  AutoTurn(60, 200, 3);
  //ToggleWingsOut();
  AutoDistance(70, 30, 200);
  sleep(100);
  //ToggleWingsIn();
  AutoDistance(-80, 5, 200);
  AutoTurn(60, 90, 2);
  AutoDistance(70, 50, 100);
  AutoTurn(60, 0, 2);
  //ToggleBlocker();
  //ToggleHang();
  DriveTorque = 10;
  AutoTillStop(40, 0);
  //ToggleHang();
}

void CloseSideBlock() {
  SetGyro(0);
  //ToggleLeftWing();
  sleep(500);
  //ToggleLeftWing();
  CatapultState = 2;
  MotorStop = 1;
  AutoDistance(80, 45, 0);
  IntakeSpeed = 100;
  sleep(2000);
  IntakeSpeed = 0;
}

void FarSideWP() {
  SetGyro(90);
  MotorStop = 1;
  CatapultState = 2;
  sleep(300);
  IntakeSpeed = 100;
  // ToggleRightWing();
  AutoTurn(60, 45, 3);
  AutoDistance(70, 5, 45);
  AutoTurn(60, 15, 2);
  IntakeSpeed = -100;
  // ToggleRightWing();
  sleep(100);
  AutoDistance(80, 20, 20);
  // ToggleRightWing();
  AutoDistance(-80, 10, 0);
  AutoTurn(60, -65, 1);
  IntakeSpeed = 100;
  AutoDistance(80, 45, -65);
  sleep(100);
  AutoTurn(60, 10, 3);
  // IntakeSpeed = 0;
  AutoDistance(70, 2, 10);
  AutoTurn(60, 90, 3);
  //ToggleLeftWing();
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

void FarSideElims() {}

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

void buttonA_pressed(){
//  autonomous();
  ToggleKick_Arm();

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
  Intake_Hold = false;
  

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
  task taskShooter(ShooterTask);
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
  //Controller1.ButtonRight.pressed(buttonRIGHT_pressed);
  Controller1.ButtonLeft.pressed(buttonLEFT_pressed);
  Controller1.ButtonA.pressed(buttonA_pressed);
  Controller1.ButtonB.pressed(buttonB_pressed);
  Controller1.ButtonX.pressed(buttonX_pressed);
  Controller1.ButtonY.pressed(buttonY_pressed);
}