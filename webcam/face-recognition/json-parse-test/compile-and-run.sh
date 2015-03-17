#!/bin/bash
# before compiling the source, you should have rapidjson library header files under one of the header file searching paths, e.g. /usr/include/

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

SOURCE_FILE1=$WORKING_HOME/parse_json_string.cpp
SOURCE_FILE2=$WORKING_HOME/parse_json_file.cpp
COMPILED_BIN1=$WORKING_HOME/parse_json_string
COMPILED_BIN2=$WORKING_HOME/parse_json_file

JSON_FILE=$WORKING_HOME/json-file/test.json

echo "Compiling $SOURCE_FILE1 ..."
g++ $SOURCE_FILE1 -o $COMPILED_BIN1
if [ $? -ne 0 ]; then
    echo "Failed to compile $SOURCE_FILE1"
    exit 1
fi
echo "Will run $COMPILED_BIN1 ..."
$COMPILED_BIN1

echo "=========================================="
echo "Compiling $SOURCE_FILE2 ..."
g++ $SOURCE_FILE2 -o $COMPILED_BIN2
if [ $? -ne 0 ]; then
    echo "Failed to compile $SOURCE_FILE2"
    exit 1
fi

# check whether the JSON file to parse exists
if [ ! -f $JSON_FILE ]; then
    echo "JSON file $JSON_FILE not exits, quit"
fi

echo "Will run $COMPILED_BIN2 ..."
$COMPILED_BIN2 $JSON_FILE
