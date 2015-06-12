#!/bin/bash
# A script to blink the LED using the [gpio] program shipped with WiringPi.

VALUE=1
GPIO_PORT=0

# set the specified GPIO port to OUTPUT mode
gpio mode $GPIO_PORT out

while true
do
    if [ $VALUE -eq 1 ]; then
	VALUE=0
    else
	VALUE=1
    fi
    gpio write $GPIO_PORT $VALUE
    sleep 1s
done
