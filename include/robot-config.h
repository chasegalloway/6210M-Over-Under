using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor RFDrive;
extern motor LFDrive;
extern controller Controller1;
extern motor RTDrive;
extern motor LTDrive;
extern inertial Inertial13;
extern motor RBDrive;
extern motor LBDrive;
extern motor PuncherRight;
extern motor Intake;
extern triport Exp;
extern digital_out Lift;
extern digital_out Kick_Arm;
extern digital_out Wings;
extern digital_out Low_Hang;
extern digital_out Hang_PTO;
extern digital_out Intake_Hold;
extern motor PuncherLeft;
extern digital_out LeftBackWing;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );