#!/bin/bash
# A script to control Raspberry Pi to display something on the LCD1602 screen.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

DATA_BITS=8    # data bit, can be 8 or 4
DISPLAY_MESSAGE="                Raspberry Pi is amazing                "  # the message displayed on the 1st row of the LCD

SOURCE=$WORKING_HOME/lcd.c
BIN=$WORKING_HOME/lcd

# compile the source code if the executable bin not exists
if [ ! -f $BIN ]; then
    echo "Compiling $SOURCE ..."
    gcc $SOURCE -o $BIN -lwiringPi -lwiringPiDev
    if [ $? -ne 0 ]; then
	echo "Failed to compile $SOURCE"
	exit 1
    fi
fi

# run the program
echo "Running $BIN ..."
$BIN $DATA_BITS "$DISPLAY_MESSAGE"
