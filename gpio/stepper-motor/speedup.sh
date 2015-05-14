#!/bin/bash
# A script to control a stepper motor(speedup) through the GPIO ports on Raspberry Pi.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

CLOCKWISE=1
COUNTER_CLOCKWISE=2

GPIO_PORT_1=0
GPIO_PORT_2=1
GPIO_PORT_3=2
GPIO_PORT_4=3

SRC_FILE=$WORKING_HOME/speedup.c
BIN_FILE=$WORKING_HOME/speedup

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
ROTATE_DIRECTION=$CLOCKWISE
echo "Running $BIN_FILE ..."
$BIN_FILE $ROTATE_DIRECTION $GPIO_PORT_1 $GPIO_PORT_2 $GPIO_PORT_3 $GPIO_PORT_4
