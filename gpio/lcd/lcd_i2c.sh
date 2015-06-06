#!/bin/bash
# A script to control Raspberry Pi to display something on the LCD1602 screen through I2C.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

DISPLAY_MESSAGE="                Raspberry Pi I2C is amazing                "  # the message displayed on the 1st row of the LCD

SOURCE=$WORKING_HOME/lcd_i2c.c
BIN=$WORKING_HOME/lcd_i2c

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
$BIN "$DISPLAY_MESSAGE"
