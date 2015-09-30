#include <glog/logging.h>
#include "motor_control.h"

/**
 * A class to control the motion of a stepper motor, e.g. rotation, stop, etc.
 *
 * @author Darran Zhang @ codelast.com
 */

MotorControl::MotorControl() {
  pins = NULL;
}

MotorControl::~MotorControl() {
  pins = NULL;
}

/**
 * Initialize the stepper motor controller.
 *
 * @param pins  A pointer which points to the pins number array.
 * @return true for successfully initialize the motor controller, false otherwise.
 */
bool MotorControl::init(unsigned int* pins) {
  if (NULL == pins) {
    LOG(ERROR) << "Can't initialize motor controller!";
    return false;
  }
  this->pins = pins;
  return true;
}

/** 
 * Rotate the stepper motor.
 *
 * @param direction  CLOCKWISE for clockwise rotation, COUNTER_CLOCKWISE for counter clockwise rotation.
 * @param delay	     The time intervals(in ms) to delay, and if the value is smaller, the motor rotates faster.
 */
void MotorControl::rotate(int direction, int delayMS) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int pinIndex = (CLOCKWISE == direction) ? (3 -j) : j;
      if (j == i) {
	digitalWrite(pins[pinIndex], 1); // output a high level
      } else {
	digitalWrite(pins[pinIndex], 0); // output a low level
      }
      delay(delayMS);
    }
  }
}

/** 
 * Stop the stepper motor by setting all of its inputs to low level.
 *
 */
void MotorControl::stop() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(pins[i], 0); // output a low level 
  }
}
