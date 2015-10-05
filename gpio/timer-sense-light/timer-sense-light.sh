#!/bin/bash
# A time-controllable human sense light implementation based on Raspberry Pi.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

TIME_RANGE_CONF_FILE=$CURRENT_DIR/conf/time-range.txt

# input mode port
PYROELECTRIC_MODULE_GPIO_PORT=0

# output mode ports, if the start index is 1 & the LED number is 4, then it means we'll use GPIO 1, 2, 3, 4 as the LED GPIO port
LED_GPIO_PORT_START=1
LED_NUMBER=4

SRC_FILE=$WORKING_HOME/timer_sense_light.cpp
BIN_FILE=$WORKING_HOME/timer_sense_light

# compile the source code if the executable bin not exists
if [ ! -f $BIN_FILE ]; then
    echo "Compiling $SRC_FILE ..."
    g++ $SRC_FILE -o $BIN_FILE -lwiringPi
    if [ $? -ne 0 ]; then
	echo "Failed to compile $SRC_FILE"
	exit 1
    fi
fi

# run the program
echo "Running $BIN_FILE ..."
$BIN_FILE $TIME_RANGE_CONF_FILE $PYROELECTRIC_MODULE_GPIO_PORT $LED_GPIO_PORT_START $LED_NUMBER
