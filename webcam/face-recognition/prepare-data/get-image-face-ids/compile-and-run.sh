#!/bin/bash
# A script to batch get the face_ids for some image files under a local dir & save the face_ids returned by Face++ service to a text file.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

SOURCE_FILE=$WORKING_HOME/get_face_id.cpp
COMPILED_BIN=$WORKING_HOME/get_face_id
OUTPUT_FACE_ID_FILE=$WORKING_HOME/output-face-ids.txt
rm -f $OUTPUT_FACE_ID_FILE

# modify the API key & API Secret to your own
API_KEY=XXXXXXXXXXXXXXXXXXXXXXXXX
API_SECRET=XXXXXXXXXXXXXXXXXXXXXXXXX
INPUT_IMAGE_FILE_DIR=$WORKING_HOME/../../image/ceo-of-jikexueyuan

rm -f $OUTPUT_FACE_ID_FILE

# compile the source code if the executable bin not exists
if [ ! -f $COMPILED_BIN ]; then
    echo "Compiling $SOURCE_FILE ..."
    g++ $SOURCE_FILE -lcurl -o $COMPILED_BIN
    if [ $? -ne 0 ]; then
	echo "Failed to compile $SOURCE_FILE"
	exit 1
    fi
fi

# run the program, each image file a time
for IMAGE_FILE_NAME in `ls $INPUT_IMAGE_FILE_DIR`
do
    IMAGE_FILE_PATH=$INPUT_IMAGE_FILE_DIR/$IMAGE_FILE_NAME
    echo "Invoking Face++ service for file $IMAGE_FILE_NAME ..."
    $COMPILED_BIN $API_KEY $API_SECRET $IMAGE_FILE_PATH $IMAGE_FILE_NAME >> $OUTPUT_FACE_ID_FILE
    if [ $? -ne 0 ]; then
	echo "Failed to get face_id for image file $IMAGE_FILE"
    fi
done
