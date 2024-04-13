#include "vex.h"

class Chase
{
private:
public:
  double drive_kp_chase = 0;
  double drive_ki_chase = 0;
  double drive_kd_chase = 0;

  void reset_drive_motors() {
    LFDrive.resetPosition();
    RFDrive.resetPosition();
    LTDrive.resetPosition();
    RTDrive.resetPosition();
    LBDrive.resetPosition();
    RBDrive.resetPosition();
    task::sleep(5);
  }

  void stop_drive_motors() {
    LFDrive.stop();
    RFDrive.stop();
    LTDrive.stop();
    RTDrive.stop();
    LBDrive.stop();
    RBDrive.stop();
    task::sleep(5);
  }

  int lspeed = 0;
  int rspeed = 0;
  
  double integral = 0;
  double previous_error = 0;

  double gyro = Inertial11.rotation(deg);

  double average_distance = (LFDrive.position(deg) + RTDrive.position(deg)) * 0.0095 * 21.5 / 20;

  void drive_distance_chase(int velocity, double distance, double heading);
  void drive_distance_chase(int velocity, double distance, double heading, double drive_kp_chase, double drive_ki_chase, double drive_kd_chase);

};
