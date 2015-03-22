#!/bin/bash
# A script to create/delete a Face++ Person.

CURRENT_DIR=`dirname "$0"`
export FACE_RECOGNITION_HOME=`cd "$CURRENT_DIR/.."; pwd`

. $FACE_RECOGNITION_HOME/prepare-data/common-config.sh

OPERATION_TYPE=create    # should be "create" or "delete"
echo "Invoking Face++ service to $OPERATION_TYPE Person $PERSON_NAME ..."
curl --silent "https://apicn.faceplusplus.com/v2/person/$OPERATION_TYPE?api_secret=$API_SECRET&api_key=$API_KEY&person_name=$PERSON_NAME"
