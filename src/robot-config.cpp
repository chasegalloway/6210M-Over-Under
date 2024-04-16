#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor RFDrive = motor(PORT11, ratio6_1, false);
motor LFDrive = motor(PORT20, ratio6_1, true);
controller Controller1 = controller(primary);
motor RMDrive = motor(PORT12, ratio6_1, false);
motor LMDrive = motor(PORT19, ratio18_1, true);
inertial Inertial15 = inertial(PORT15);
motor RBDrive = motor(PORT13, ratio6_1, false);
motor LBDrive = motor(PORT17, ratio6_1, true);
motor Puncher = motor(PORT8, ratio18_1, true);
motor Intake = motor(PORT21, ratio18_1, true);
digital_out Kick_Arm = digital_out(Brain.ThreeWirePort.B);
digital_out LeftWing = digital_out(Brain.ThreeWirePort.D);
digital_out Hang_PTO = digital_out(Brain.ThreeWirePort.E);
digital_out Hang_Release = digital_out(Brain.ThreeWirePort.F);
digital_out LeftBackWing = digital_out(Brain.ThreeWirePort.A);
distance DistanceSensor = distance(PORT16);
digital_out RightWing = digital_out(Brain.ThreeWirePort.C);
rotation HangPosition = rotation(PORT3, false);

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