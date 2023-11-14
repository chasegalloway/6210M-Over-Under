#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
triport Exp = triport(PORT10);
motor RFDrive = motor(PORT5, ratio18_1, true);
motor LFDrive = motor(PORT14, ratio18_1, false);
controller Controller1 = controller(primary);
motor RRDrive = motor(PORT2, ratio18_1, false);
motor LRDrive = motor(PORT12, ratio18_1, false);
inertial Inertial1 = inertial(PORT4);
motor RCDrive = motor(PORT3, ratio18_1, true);
motor LCDrive = motor(PORT13, ratio18_1, true);
motor Catapult = motor(PORT21, ratio36_1, true);
motor Intake = motor(PORT1, ratio6_1, false);
digital_out Wings = digital_out(Exp.A);
optical TriBallDetect = optical(PORT19);
rotation CataRotate = rotation(PORT6, false);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}