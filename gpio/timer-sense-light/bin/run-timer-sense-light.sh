#!/bin/bash

bin=`dirname "$0"`
export TIMER_SENSE_LIGHT_HOME=`cd "$bin/.."; pwd`

. $TIMER_SENSE_LIGHT_HOME/bin/common-config.sh
cd $TIMER_SENSE_LIGHT_HOME

if [ ! -f $TIME_RANGE_CONFIG_FILE ]; then
    echo "Config file $TIME_RANGE_CONFIG_FILE not exists, create an empty one"
    touch $TIME_RANGE_CONFIG_FILE
    exit 1
fi

# set glog dir by setting this environment variable
export GOOGLE_LOG_DIR=$LOG_DIR

# run it
echo "Running program..."
$TIMER_SENSE_LIGHT_HOME/timer_sense_light $MAIN_CONFIG_FILE $TIME_RANGE_CONFIG_FILE
