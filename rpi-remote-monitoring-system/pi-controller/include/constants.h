#ifndef __CONSTANTS_H
#define __CONSTANTS_H

#include <string>

/**
 * Constants definition.
 *
 * @author Darran Zhang @ codelast.com
 */

const int CLOCKWISE = 1;
const int COUNTER_CLOCKWISE = -1;

const int COMMAND_TAKE_PHOTO = 1;
const int COMMAND_MOTOR_ROTATE_CLOCKWISE = 2;
const int COMMAND_MOTOR_ROTATE_COUNTER_CLOCKWISE = 3;

const int RPC_BUFFER_SIZE = 524288;  // 512KB

const std::string CFG_FILE_SECTION_SERVER = "Server";
const std::string CFG_FILE_SECTION_LOCAL = "Local";

const std::string RPC_METHOD_NAME_ROTATE_MOTOR = "rotateMotor";
const std::string RPC_METHOD_NAME_CAPTURE_IMAGE = "captureImage";

const std::string JSON_FIELD_ID = "id";
const std::string JSON_FIELD_DIRECTION = "direction";
const std::string JSON_FIELD_ROUND = "round";
const std::string JSON_FIELD_VALIDATE = "validate";

#endif
