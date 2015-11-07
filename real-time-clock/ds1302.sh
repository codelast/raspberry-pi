#!/bin/bash
# A script to run the real-time clock manipulation program.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

echo "Usage:./ds1302.sh [slc|sdsc|rtest]"

PARAM=
if [ $# -ge 1 ]; then
    PARAM="-"$1
fi

RASPBERRY_PI_MODEL=b_plus
if [ $# -ge 2 ]; then
    RASPBERRY_PI_MODEL=$2
fi

# set these ports according to your RPi GPIO pinout
SCLK_PORT=14
SDA0_PORT=
CE0_PORT=10

case "$RASPBERRY_PI_MODEL" in
    'b')
	SDA0_PORT=8
	;;
    'b_plus')
	SDA0_PORT=30
	;;
    *)
	echo "Unsupported Raspberry Pi model: $RASPBERRY_PI_MODEL"
	exit 1
esac

SOURCE=$WORKING_HOME/ds1302.c
BIN=$WORKING_HOME/ds1302

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
$BIN $SCLK_PORT $SDA0_PORT $CE0_PORT $PARAM
