#!/bin/bash
# A script to control some LEDs through an pyroelectric module.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

# input mode port
PYROELECTRIC_MODULE_GPIO_PORT=7

# output mode ports
LED_GPIO_PORT_1=0
LED_GPIO_PORT_2=1

SRC_FILE=$WORKING_HOME/sense-light.c
BIN_FILE=$WORKING_HOME/sense-light

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
$BIN_FILE $PYROELECTRIC_MODULE_GPIO_PORT $LED_GPIO_PORT_1 $LED_GPIO_PORT_2
