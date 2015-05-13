#!/bin/bash
# A script to control a stepper motor through the GPIO ports on Raspberry Pi.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

GPIO_PORT_1=0
GPIO_PORT_2=1
GPIO_PORT_3=2
GPIO_PORT_4=3

SRC_FILE=$WORKING_HOME/motor.c
BIN_FILE=$WORKING_HOME/motor

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
$BIN_FILE $GPIO_PORT_1 $GPIO_PORT_2 $GPIO_PORT_3 $GPIO_PORT_4
