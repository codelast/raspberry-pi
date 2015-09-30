#ifndef __MOTOR_CONTROL_H
#define __MOTOR_CONTROL_H

#include "constants.h"
#include <wiringPi.h>

/**
 * A class to control the motion of a stepper motor, e.g. rotation, stop, etc.
 *
 * @author Darran Zhang @ codelast.com
 */

class MotorControl
{
 public:
  MotorControl();
  virtual ~MotorControl();

 private:
  unsigned int *pins;

 public:
  bool init(unsigned int *pins);
  void rotate(int direction, int delay);
  void stop();
};

#endif
