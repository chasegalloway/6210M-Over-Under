using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor RFDrive;
extern motor LFDrive;
extern motor RTDrive;
extern motor LTDrive;
extern motor RBDrive;
extern motor LBDrive;
extern motor PuncherRight;
extern motor Intake;
extern motor PuncherLeft;
extern inertial Inertial11;
extern digital_out Kick_Arm;
extern digital_out LeftWing;
extern digital_out Low_Hang;
extern digital_out High_Hang;
extern digital_out LeftBackWing;
extern distance Distance_Sensor;
extern digital_out RightWing;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );