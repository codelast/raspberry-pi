#!/bin/bash
# A script to invoke a C program to detect motion in front of an USB camera using OpenCV.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

SOURCE_FILE=$WORKING_HOME/motion-detection.c
BIN_FILE=$WORKING_HOME/motion-detection
MOTION_DETECTION_THRESHOLD=100

# compile the source code if the executable bin not exists
if [ ! -f $BIN_FILE ]; then
    echo "Compiling $SOURCE_FILE ..."
    g++ -I/usr/include/opencv2/  `pkg-config --cflags opencv --libs opencv` $SOURCE_FILE -o $BIN_FILE
    if [ $? -ne 0 ]; then
	echo "Failed to compile $SOURCE_FILE"
	exit 1
    fi
fi

# run the program
$BIN_FILE $MOTION_DETECTION_THRESHOLD
