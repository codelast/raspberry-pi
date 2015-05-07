#!/bin/bash
# A script to blink the LED using a C program which invokes the WiringPi library.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

GPIO_PORT=0

BLINK_SOURCE=$WORKING_HOME/blink.c
BLINK_BIN=$WORKING_HOME/blink

# compile the source code if the executable bin not exists
if [ ! -f $BLINK_BIN ]; then
    echo "Compiling $BLINK_SOURCE ..."
    gcc $BLINK_SOURCE -o $BLINK_BIN -lwiringPi
    if [ $? -ne 0 ]; then
	echo "Failed to compile $BLINK_SOURCE"
	exit 1
    fi
fi

# run the program
echo "Running $BLINK_BIN ..."
$BLINK_BIN $GPIO_PORT
