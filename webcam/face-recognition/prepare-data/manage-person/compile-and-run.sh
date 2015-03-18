#!/bin/bash
# A script to create/delete a Face++ Person.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

SOURCE_FILE=$WORKING_HOME/manage_person.cpp
COMPILED_BIN=$WORKING_HOME/manage_person

# replace the API key & API Secret with your own
API_KEY=XXXXXXXXXXXXXXXXXXXXXXXX
API_SECRET=XXXXXXXXXXXXXXXXXXXXXXXX

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
OPERATION_TYPE=create    # should be "create" or "delete"
echo "Invoking Face++ service to $OPERATION_TYPE Person $PERSON_NAME ..."
$COMPILED_BIN $API_KEY $API_SECRET $OPERATION_TYPE $PERSON_NAME
