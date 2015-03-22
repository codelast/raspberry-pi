#!/bin/bash
# A script to get the face_ids for some image files under a local dir & save the face_ids returned by Face++ service to a text file.

CURRENT_DIR=`dirname "$0"`
export FACE_RECOGNITION_HOME=`cd "$CURRENT_DIR/../.."; pwd`

. $FACE_RECOGNITION_HOME/prepare-data/common-config.sh

SOURCE_FILE=$GET_IMAGE_FACE_IDS_DIR/get_face_id.cpp
COMPILED_BIN=$GET_IMAGE_FACE_IDS_DIR/get_face_id

OUTPUT_FACE_ID_FILE=$GET_IMAGE_FACE_IDS_DIR/output-face-ids.txt
rm -f $OUTPUT_FACE_ID_FILE

# the directory which contains the image files to be trained
INPUT_IMAGE_FILE_DIR=$IMAGE_FILE_BASE_DIR/obama/train

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
done
