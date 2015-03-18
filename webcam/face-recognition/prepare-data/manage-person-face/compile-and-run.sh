#!/bin/bash
# A script to add/remove face for a Face++ Person.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

SOURCE_FILE=$WORKING_HOME/manage_person_face.cpp
COMPILED_BIN=$WORKING_HOME/manage_person_face

# replace the API key & API Secret with your own
API_KEY=XXX
API_SECRET=XXX

# compile the source code if the executable bin not exists
if [ ! -f $COMPILED_BIN ]; then
    echo "Compiling $SOURCE_FILE ..."
    g++ $SOURCE_FILE -lcurl -o $COMPILED_BIN
    if [ $? -ne 0 ]; then
	echo "Failed to compile $SOURCE_FILE"
	exit 1
    fi
fi

# run the program
PERSON_NAME=jinyan
OPERATION_TYPE=add    # should be "add" or "remove"
FACE_ID_FILE=$WORKING_HOME/../get-image-face-ids/output-face-ids.txt
unset ALL_FACE_ID
while read LINE
do
    ALL_FACE_ID=$ALL_FACE_ID","$LINE
done < $FACE_ID_FILE
ALL_FACE_ID=${ALL_FACE_ID:1}  # remove the first ","
echo "All face_id : $ALL_FACE_ID"

echo "Invoking Face++ service to $OPERATION_TYPE face for Person $PERSON_NAME ..."
$COMPILED_BIN $API_KEY $API_SECRET $OPERATION_TYPE $PERSON_NAME $ALL_FACE_ID
