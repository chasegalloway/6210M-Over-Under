using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor RFDrive;
extern motor LFDrive;
extern controller Controller1;
extern motor RRDrive;
extern motor LRDrive;
extern inertial Inertial1;
extern motor RCDrive;
extern motor LCDrive;
extern motor Catapult;
extern motor Intake;
extern triport Exp;
extern digital_out Wings;
extern optical TriBallDetect;
extern rotation CataRotate;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );