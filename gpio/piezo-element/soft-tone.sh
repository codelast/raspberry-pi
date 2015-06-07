#!/bin/bash
# A script to control Raspberry Pi to play music through a piezo element.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

GPIO_PORT=0

SOURCE=$WORKING_HOME/soft_tone.c
BIN=$WORKING_HOME/soft_tone

# compile the source code if the executable bin not exists
if [ ! -f $BIN ]; then
    echo "Compiling $SOURCE ..."
    gcc $SOURCE -o $BIN -lwiringPi
    if [ $? -ne 0 ]; then
	echo "Failed to compile $SOURCE"
	exit 1
    fi
fi

# run the program
echo "Running $BIN ..."
$BIN $GPIO_PORT
