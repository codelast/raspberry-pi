#ifndef __CONSTANTS_H
#define __CONSTANTS_H

#include <string>
#include "3rd/option_parser.h"

/**
 * Constants used by this program.
 *
 * @author Darran Zhang @ codelast.com
 */

const int CLOCKWISE = 1;
const int COUNTER_CLOCKWISE = -1;

const int COMMAND_TAKE_PHOTO = 1;
const int COMMAND_MOTOR_ROTATE_CLOCKWISE = 2;
const int COMMAND_MOTOR_ROTATE_COUNTER_CLOCKWISE = 3;

const std::string CFG_FILE_SECTION_SERVER = "Server";
const std::string CFG_FILE_SECTION_LOCAL = "Local";

const std::string RPC_METHOD_NAME_ROTATE_MOTOR = "rotateMotor";
const std::string RPC_METHOD_NAME_CAPTURE_IMAGE = "captureImage";

const std::string JSON_FIELD_ID = "id";
const std::string JSON_FIELD_DIRECTION = "direction";
const std::string JSON_FIELD_ROUND = "round";
const std::string JSON_FIELD_VALIDATE = "validate";

enum  optionIndex { UNKNOWN, CONFIG_FILE, LOG_DIR };
const option::Descriptor usage[] = {
	{UNKNOWN, 0, "", "", option::Arg::None, "Usage: ./run-pi-client.sh --configFile=<your_config_file>\n"},
	{CONFIG_FILE, 0, "c","configFile", option::Arg::Optional, "-c <your_config_file> \t--configFile=<your_config_file>,  \tProgram configuration file." },
  {LOG_DIR, 0, "l","logDir", option::Arg::Optional, "-l <log_directory> \t--logDir=<log_directory>,  \tProgram log directory." },
	{UNKNOWN, 0, "", "", option::Arg::None, "Unknown argument!"},
	{0,0,0,0,0,0}
};

#endif
