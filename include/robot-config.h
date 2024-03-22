using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor RFDrive;
extern motor LFDrive;
extern controller Controller1;
extern motor RTDrive;
extern motor LTDrive;
extern inertial Inertial11;
extern motor RBDrive;
extern motor LBDrive;
extern motor PuncherRight;
extern motor Intake;
extern digital_out Kick_Arm;
extern digital_out LeftWing;
extern digital_out Low_Hang;
extern digital_out High_Hang;
extern motor PuncherLeft;
extern digital_out LeftBackWing;
extern distance DistanceSensor;
extern digital_out RightWing;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );