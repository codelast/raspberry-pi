#!/bin/bash
# Control the brightness of an LED using a C program which invokes the WiringPi library.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

GPIO_PORT=0

PWM_SOURCE=$WORKING_HOME/pwm_software.c
PWM_BIN=$WORKING_HOME/pwm_software

# compile the source code if the executable bin not exists
if [ ! -f $PWM_BIN ]; then
    echo "Compiling $PWM_SOURCE ..."
    gcc $PWM_SOURCE -o $PWM_BIN -lwiringPi
    if [ $? -ne 0 ]; then
	echo "Failed to compile $PWM_SOURCE"
	exit 1
    fi
fi

# run the program
echo "Running $PWM_BIN ..."
$PWM_BIN $GPIO_PORT
