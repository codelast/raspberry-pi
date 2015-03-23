#!/bin/bash
# A script to invoke a C program to capture video from USB camera & save to a file using OpenCV.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

SOURCE_FILE=$WORKING_HOME/video_capture.cpp
COMPILED_BIN=$WORKING_HOME/video_capture
OUTPUT_VIDEO_FILE=$WORKING_HOME/webcam.avi

rm -f $OUTPUT_VIDEO_FILE

# compile the source code if the executable bin not exists
if [ ! -f $COMPILED_BIN ]; then
    echo "Compiling $SOURCE_FILE ..."
    g++ -I/usr/include/opencv2/  `pkg-config --cflags opencv --libs opencv` $SOURCE_FILE -o $COMPILED_BIN
    if [ $? -ne 0 ]; then
	echo "Failed to compile $SOURCE_FILE"
	exit 1
    fi
fi

# run the program
echo "Start capturing video & save to file $OUTPUT_VIDEO_FILE ..."
$COMPILED_BIN $OUTPUT_VIDEO_FILE
