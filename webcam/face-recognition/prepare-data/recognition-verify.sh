#!/bin/bash
# A script to verify a Face++ Person.

CURRENT_DIR=`dirname "$0"`
export FACE_RECOGNITION_HOME=`cd "$CURRENT_DIR/.."; pwd`

. $FACE_RECOGNITION_HOME/prepare-data/common-config.sh


# first, we should detect an image file to get the face_id
BIN_TO_GET_FACE_ID=$GET_IMAGE_FACE_IDS_DIR/get_face_id
if [ ! -f $BIN_TO_GET_FACE_ID ]; then
    echo "Program to get face_id not exits: [ $BIN_TO_GET_FACE_ID ], quit"
    exit 1
fi

IMAGE_FILE_NAME=21.jpg
LOCAL_IMAGE_FILE_TO_VERIFY=$IMAGE_FILE_BASE_DIR/$PERSON_NAME/verify/$IMAGE_FILE_NAME
if [ ! -f $LOCAL_IMAGE_FILE_TO_VERIFY ]; then
    echo "Image file $LOCAL_IMAGE_FILE_TO_VERIFY not exists, quit"
    exit 1
fi
FACE_ID=`$BIN_TO_GET_FACE_ID $API_KEY $API_SECRET $LOCAL_IMAGE_FILE_TO_VERIFY $IMAGE_FILE_NAME`
if [ -z $FACE_ID ]; then
    echo "Failed to get face_id, quit"
    exit 1
fi

# then, we should verify the face_id we just got
echo "Invoking Face++ service to verify Person $PERSON_NAME ..."
RESULT=`curl --silent "https://apicn.faceplusplus.com/v2/recognition/verify?api_secret=$API_SECRET&api_key=$API_KEY&face_id=$FACE_ID&person_name=$PERSON_NAME"`    # e.g. "{ "confidence": 55.039787, "is_same_person": true, "session_id": "a58bbfc672abce074111166dd9961163" }"

CONFIDENCE=`echo $RESULT | grep -Po '(?<= \"confidence\"\:\ ).*(?=, \"is_same_person)'`
IS_SAME_PERSON=`echo $RESULT | grep -Po '(?<= \"is_same_person\"\:\ ).*(?=,)'`

if [[ "$IS_SAME_PERSON"x = "true"x ]]; then
    echo "YES! Confidence[$CONFIDENCE]"
else
    echo "NO! Confidence[$CONFIDENCE]"
fi
