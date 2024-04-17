#pragma once

class PID {
  public:
    void drive_distance(int Speed, double Distance, double Heading);
    void turn(int Speed, int Heading, int Accuracy);
};