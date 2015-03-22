#!/bin/bash
# A script to add/remove faces for a Face++ Person.

CURRENT_DIR=`dirname "$0"`
export FACE_RECOGNITION_HOME=`cd "$CURRENT_DIR/.."; pwd`

. $FACE_RECOGNITION_HOME/prepare-data/common-config.sh


OPERATION_TYPE=add    # should be "add" or "remove"
FACE_ID_FILE=$GET_IMAGE_FACE_IDS_DIR/output-face-ids.txt

unset ALL_FACE_ID
while read LINE
do
    ALL_FACE_ID=$ALL_FACE_ID","$LINE
done < $FACE_ID_FILE
ALL_FACE_ID=${ALL_FACE_ID:1}  # remove the first ","
echo "All face_id : $ALL_FACE_ID"

echo "Invoking Face++ service to $OPERATION_TYPE face for Person $PERSON_NAME ..."
curl --silent "https://apicn.faceplusplus.com/v2/person/$OPERATION_TYPE"_face"?api_secret=$API_SECRET&api_key=$API_KEY&face_id=$ALL_FACE_ID&person_name=$PERSON_NAME"
