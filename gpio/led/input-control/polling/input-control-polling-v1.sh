#!/bin/bash
# A script to control a LED through an button using the [gpio] program shipped with WiringPi - polling way.

CURRENT_DIR=`dirname "$0"`
WORKING_HOME=`cd "$CURRENT_DIR"; pwd`

BUTTON_GPIO_PORT=0
LED_GPIO_PORT=7

gpio mode $BUTTON_GPIO_PORT in
gpio mode $LED_GPIO_PORT out

LEVEL=0
while true
do
    CURRENT_LEVEL=`gpio read $BUTTON_GPIO_PORT`
    if [ $LEVEL -ne $CURRENT_LEVEL ]; then
	gpio write $LED_GPIO_PORT $LEVEL
	LEVEL=$CURRENT_LEVEL
    fi
    sleep 0.01
done
