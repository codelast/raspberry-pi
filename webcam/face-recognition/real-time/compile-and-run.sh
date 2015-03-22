#!/bin/bash
# A script to verify the face in front of an USB camera when motion is detected.

CURRENT_DIR=`dirname "$0"`
export FACE_RECOGNITION_HOME=`cd "$CURRENT_DIR/.."; pwd`

. $FACE_RECOGNITION_HOME/prepare-data/common-config.sh

SOURCE_FILE=$REAL_TIME_DIR/real_time_face_recognition.cpp
COMPILED_BIN=$REAL_TIME_DIR/real_time_face_recognition
MOTION_DETECTION_THRESHOLD=100

CAPTURED_IMAGE_FILE_NAME=captured.jpg
CAPTURED_IMAGE_FILE_PATH=$REAL_TIME_DIR/$CAPTURED_IMAGE_FILE_NAME

rm -f $CAPTURED_IMAGE_FILE_NAME

# compile the source code if the executable bin not exists
if [ ! -f $COMPILED_BIN ]; then
    echo "Compiling $SOURCE_FILE ..."
    g++ -I/usr/include/opencv2/ `pkg-config --cflags opencv --libs opencv` -lcurl $SOURCE_FILE -o $COMPILED_BIN
    if [ $? -ne 0 ]; then
	echo "Failed to compile $SOURCE_FILE"
	exit 1
    fi
fi

# run the program
$COMPILED_BIN $MOTION_DETECTION_THRESHOLD $API_KEY $API_SECRET $PERSON_NAME $CAPTURED_IMAGE_FILE_PATH $CAPTURED_IMAGE_FILE_NAME
