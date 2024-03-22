#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor RFDrive = motor(PORT5, ratio6_1, false);
motor LFDrive = motor(PORT6, ratio6_1, true);
controller Controller1 = controller(primary);
motor RTDrive = motor(PORT3, ratio6_1, true);
motor LTDrive = motor(PORT4, ratio18_1, false);
inertial Inertial11 = inertial(PORT11);
motor RBDrive = motor(PORT1, ratio6_1, false);
motor LBDrive = motor(PORT2, ratio6_1, true);
motor PuncherRight = motor(PORT12, ratio18_1, false);
motor Intake = motor(PORT21, ratio18_1, true);
digital_out Kick_Arm = digital_out(Brain.ThreeWirePort.C);
digital_out LeftWing = digital_out(Brain.ThreeWirePort.D);
digital_out Low_Hang = digital_out(Brain.ThreeWirePort.E);
digital_out High_Hang = digital_out(Brain.ThreeWirePort.F);
motor PuncherLeft = motor(PORT8, ratio18_1, true);
digital_out LeftBackWing = digital_out(Brain.ThreeWirePort.A);
distance DistanceSensor = distance(PORT10);
digital_out RightWing = digital_out(Brain.ThreeWirePort.G);

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