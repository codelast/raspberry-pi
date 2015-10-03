#!/bin/bash

bin=`dirname "$0"`
export PI_CONTROLLER_HOME=`cd "$bin/.."; pwd`

. $PI_CONTROLLER_HOME/bin/common-config.sh
cd $PI_CONTROLLER_HOME

# set glog dir by setting this environment variable
export GOOGLE_LOG_DIR=$LOG_DIR

# run it
$PI_CONTROLLER_HOME/pi_controller $CONFIG_FILE
