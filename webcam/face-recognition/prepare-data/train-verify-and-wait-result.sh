#!/bin/bash
# A script to train a Face++ Person.

CURRENT_DIR=`dirname "$0"`
export FACE_RECOGNITION_HOME=`cd "$CURRENT_DIR/.."; pwd`

. $FACE_RECOGNITION_HOME/prepare-data/common-config.sh

echo "Invoking Face++ service to train Person $PERSON_NAME ..."
RESULT=`curl --silent "https://apicn.faceplusplus.com/v2/train/verify?api_secret=$API_SECRET&api_key=$API_KEY&person_name=$PERSON_NAME"`    # e.g. "{ "session_id": "38ef09473b3d4a979a32b044397dc3c1" }"
SESSION_ID=`echo $RESULT | grep -Po '(?<= \"session_id\"\:\ \").*(?=\")'`

# wait until the train succeed or timeout
echo "Waiting for the training result ..."
for i in `seq 1 100`
do
    RESULT=`curl --silent "https://apicn.faceplusplus.com/v2/info/get_session?api_secret=$API_SECRET&api_key=$API_KEY&session_id=$SESSION_ID"`
    SUCCESS_COUNT=`echo $RESULT | grep SUCC | wc -l`
    if [ $SUCCESS_COUNT -eq 1 ]; then
	echo "Successfully finished the training"
	break
    fi
    sleep 1s
done
