#!/bin/bash
# A script to control a LED through an button using WiringPi.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

BUTTON_GPIO_PORT=0
LED_GPIO_PORT=7

SRC_FILE=$WORKING_HOME/input_control.c
BIN_FILE=$WORKING_HOME/input_control

# compile the source code if the executable bin not exists
if [ ! -f $BIN_FILE ]; then
    echo "Compiling $SRC_FILE ..."
    gcc $SRC_FILE -o $BIN_FILE -lwiringPi
    if [ $? -ne 0 ]; then
	echo "Failed to compile $SRC_FILE"
	exit 1
    fi
fi

# run the program
echo "Running $BIN_FILE ..."
$BIN_FILE $BUTTON_GPIO_PORT $LED_GPIO_PORT
