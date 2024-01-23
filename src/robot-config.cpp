#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
triport Exp = triport(PORT5);
motor RFDrive = motor(PORT20, ratio6_1, false);
motor LFDrive = motor(PORT16, ratio6_1, true);
controller Controller1 = controller(primary);
motor RTDrive = motor(PORT17, ratio6_1, true);
motor LTDrive = motor(PORT14, ratio18_1, false);
inertial Inertial13 = inertial(PORT13);
motor RBDrive = motor(PORT19, ratio6_1, false);
motor LBDrive = motor(PORT15, ratio6_1, true);
motor PuncherRight = motor(PORT21, ratio18_1, true);
motor Intake = motor(PORT3, ratio6_1, false);
digital_out Lift = digital_out(Exp.A);
digital_out Kick_Arm = digital_out(Exp.D);
digital_out Wings = digital_out(Exp.C);
digital_out Low_Hang = digital_out(Exp.B);
digital_out Hang_PTO = digital_out(Exp.F);
digital_out Intake_Hold = digital_out(Exp.E);
motor PuncherLeft = motor(PORT6, ratio18_1, false);

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