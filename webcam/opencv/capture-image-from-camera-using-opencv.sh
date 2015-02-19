#!/bin/bash
# A script to invoke a C program to capture an image from USB camera using OpenCV.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

WEBCAM_SOURCE=$WORKING_HOME/webcam.c
WEBCAM_BIN=$WORKING_HOME/webcam
OUTPUT_IMAGE_FILE=$WORKING_HOME/webcam.jpg

rm -f $OUTPUT_IMAGE_FILE

# compile the source code if the executable bin not exists
if [ ! -f $WEBCAM_BIN ]; then
    echo "Compiling $WEBCAM_SOURCE ..."
    g++ -I/usr/include/opencv2/  `pkg-config --cflags opencv --libs opencv` $WEBCAM_SOURCE -o $WEBCAM_BIN
    if [ $? -ne 0 ]; then
	echo "Failed to compile $WEBCAM_SOURCE"
	exit 1
    fi
fi

$WEBCAM_BIN $OUTPUT_IMAGE_FILE
if [ $? -ne 0 ]; then
    echo "Failed to capture image and output to file $OUTPUT_IMAGE_FILE"
    exit 1
fi
