#!/bin/bash
# A script to invoke a C program to capture an image from USB camera using V4L2.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

WEBCAM_SOURCE="$WORKING_HOME/webcam.c $WORKING_HOME/yuv.c"
WEBCAM_BIN=$WORKING_HOME/webcam
OUTPUT_IMAGE_FILE=$WORKING_HOME/webcam.jpg

rm -f $OUTPUT_IMAGE_FILE

# compile the source code if the executable bin not exists
if [ ! -f $WEBCAM_BIN ]; then
    echo "Compiling $WEBCAM_SOURCE ..."
    gcc $WEBCAM_SOURCE -o $WEBCAM_BIN -Wall -ljpeg -lv4l2 -DIO_READ -DIO_MMAP -DIO_USERPTR
    if [ $? -ne 0 ]; then
	echo "Failed to compile $WEBCAM_SOURCE"
	exit 1
    fi
fi

$WEBCAM_BIN -o $OUTPUT_IMAGE_FILE
if [ $? -ne 0 ]; then
    echo "Failed to capture image and output to file $OUTPUT_IMAGE_FILE"
    exit 1
fi
