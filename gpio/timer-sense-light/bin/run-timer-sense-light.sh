#!/bin/bash

bin=`dirname "$0"`
export TIMER_SENSE_LIGHT_HOME=`cd "$bin/.."; pwd`

. $TIMER_SENSE_LIGHT_HOME/bin/common-config.sh
cd $TIMER_SENSE_LIGHT_HOME

if [ ! -f $TIME_RANGE_CONFIG_FILE ]; then
    echo "Config file $TIME_RANGE_CONFIG_FILE not exists, quit"
    exit 1
fi

# input mode port
PYROELECTRIC_MODULE_GPIO_PORT=0

# output mode ports, if the start index is 1 & the LED number is 4, then it means we'll use GPIO 1, 2, 3, 4 as the LED GPIO port
LED_GPIO_PORT_START=1
LED_NUMBER=4

# set glog dir by setting this environment variable
export GOOGLE_LOG_DIR=$LOG_DIR

# run it
echo "Running program..."
$TIMER_SENSE_LIGHT_HOME/timer_sense_light $TIME_RANGE_CONFIG_FILE $PYROELECTRIC_MODULE_GPIO_PORT $LED_GPIO_PORT_START $LED_NUMBER
