#!/bin/bash
# A script to turn on/off a LED through a button(the interrupt way, with software switch debouncing).

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

BUTTON_GPIO_PORT=0  # the input signal comes from this GPIO port
LED_GPIO_PORT=7     # the output signal to turn on/off the LED comes from this GPIO port

SRC_FILE=$WORKING_HOME/input_control_interrupt_v2.c
BIN_FILE=$WORKING_HOME/input_control_interrupt_v2

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
