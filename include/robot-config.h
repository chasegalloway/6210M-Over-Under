using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor RFDrive;
extern motor LFDrive;
extern controller Controller1;
extern motor RMDrive;
extern motor LMDrive;
extern inertial Inertial15;
extern motor RBDrive;
extern motor LBDrive;
extern motor Puncher;
extern motor Intake;
extern digital_out Kick_Arm;
extern digital_out LeftWing;
extern digital_out Hang_PTO;
extern digital_out Hang_Release;
extern digital_out LeftBackWing;
extern distance DistanceSensor;
extern digital_out RightWing;
extern rotation HangPosition;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );