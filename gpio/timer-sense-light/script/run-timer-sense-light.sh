#!/bin/bash
# A script to start, restart or kill the timer_sense_light progarm.
# Usage example:
# (1) start:    ./run-timer-sense-light.sh
# (2) restart:  ./run-timer-sense-light.sh restart
# (3) kill:     ./run-timer-sense-light.sh kill

CURRENT_DIR=`dirname "$0"`
export TIMER_SENSE_LIGHT_HOME=`cd "$CURRENT_DIR/.."; pwd`

. $TIMER_SENSE_LIGHT_HOME/script/common-config.sh
cd $TIMER_SENSE_LIGHT_HOME

EXEC_FILE_NAME=timer_sense_light

function process_count() {
    ps -ef | grep $EXEC_FILE_NAME | grep -v grep | wc -l
}

function kill_process() {
    PID=`ps -ef | grep $EXEC_FILE_NAME | grep -v grep | awk '{print $2}'`
    if [ -z $PID ]; then  # no process found
	echo "Process $EXEC_FILE_NAME not found, nothing to kill"
	return
    fi

    KILL_RESULT=0
    for i in `seq 1 20`
    do
	# send signal SIGINT to the prgram, which will make it exit gracefully
	kill -s SIGINT $PID
	sleep 4s
	if [ `process_count` -eq 0 ]; then
	    KILL_RESULT=1
	    echo "Successfully killed $EXEC_FILE_NAME"
	    break
	fi
    done
    if [ $KILL_RESULT -eq 0 ]; then
	echo "Failed to kill $EXEC_FILE_NAME"
	exit 1
    fi
}

COMMAND=
if [ $# -ge 1 ]; then
    COMMAND=$1
    if [[ $COMMAND == 'restart' ]]; then
	echo "Try to restart $EXEC_FILE_NAME, please wait..."
	kill_process
    elif [[ $COMMAND == 'kill' ]]; then
	echo "Try to kill $EXEC_FILE_NAME, please wait..."
	kill_process
	exit
    else
	echo "Unrecognized command: $COMMAND"
	exit 1
    fi
fi

if [ `process_count` -gt 0 ]; then
    echo "You can only start one $EXEC_FILE_NAME"
    exit 1
fi

if [ ! -f $TIME_RANGE_CONFIG_FILE ]; then
    echo "Config file $TIME_RANGE_CONFIG_FILE not exists, create an empty one"
    touch $TIME_RANGE_CONFIG_FILE
    exit 1
fi

# config glog by setting some environment variables
export GOOGLE_LOG_DIR=$LOG_DIR  # log dir
export GLOG_max_log_size=5      # create a new log file every N MB

# run it
echo "Running $EXEC_FILE_NAME..."
$TIMER_SENSE_LIGHT_HOME/$EXEC_FILE_NAME $MAIN_CONFIG_FILE $TIME_RANGE_CONFIG_FILE
