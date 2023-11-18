#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
triport Exp = triport(PORT21);
motor RFDrive = motor(PORT13, ratio18_1, false);
motor LFDrive = motor(PORT14, ratio18_1, true);
controller Controller1 = controller(primary);
motor RRDrive = motor(PORT11, ratio18_1, false);
motor LRDrive = motor(PORT18, ratio18_1, true);
inertial Inertial1 = inertial(PORT1);
motor RCDrive = motor(PORT12, ratio18_1, false);
motor LCDrive = motor(PORT20, ratio18_1, true);
motor Catapult = motor(PORT19, ratio36_1, false);
motor Intake = motor(PORT16, ratio6_1, false);
digital_out WingsLeft = digital_out(Exp.A);
rotation CataRotate = rotation(PORT15, false);
digital_out Blocker = digital_out(Exp.D);
digital_out Hang = digital_out(Exp.C);
digital_out WingsRight = digital_out(Exp.B);
digital_out LowHang = digital_out(Exp.F);
digital_out ButtHang = digital_out(Exp.G);
distance TriballDetect = distance(PORT2);

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