#!/bin/bash

bin=`dirname "$0"`
export PI_CONTROLLER_HOME=`cd "$bin/.."; pwd`

. $PI_CONTROLLER_HOME/bin/common-config.sh
cd $PI_CONTROLLER_HOME

# run it
$PI_CONTROLLER_HOME/pi_controller $LOG_DIR $CONFIG_FILE
