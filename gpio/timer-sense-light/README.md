# A time-controllable human sense light based on Raspberry Pi.
## 一个基于树莓派的、(开关)时间可控的人体感应灯——可以在感应到人接近的时候自动点亮灯，也可以定义在什么时间段禁止灯亮(例如白天)。
![](https://raw.githubusercontent.com/codelast/raspberry-pi/master/gpio/timer-sense-light/demo/finished_product.jpg)

I only tested this program on Raspberry Pi Model B+(Revision 1.2) & Model B(Revision 2), but it should work on other models of RPi too, except that the hardware wiring should be some difference, and the code should need no modification.

Environment:

**Raspberry Pi model: B+/B**

**OS: Arch Linux ARM**

****

You can use this project to build a human sense light, in short, when a person gets close to the light, the pyroelectric module connected to Raspberry Pi will detect the human movement & ouput a high level to one of the GPIO port on RPi, then the program runs on RPi will light up some LEDs connected to it.
Also, you can set the time range of day, only in those time range the LEDs can be lighten up, and in other time range the LEDs will be turned off.

****

## Hardware model
You should prepare some specific hardware components for this project, including:
#### A pyroelectric module(HC-SR501)
![](https://raw.githubusercontent.com/codelast/raspberry-pi/master/gpio/timer-sense-light/demo/pyroelectric_module_1.png)
![](https://raw.githubusercontent.com/codelast/raspberry-pi/master/gpio/timer-sense-light/demo/pyroelectric_module_2.png)

#### Some LEDs
![](https://raw.githubusercontent.com/codelast/raspberry-pi/master/gpio/timer-sense-light/demo/led.jpg)

****

## Dependencies installation
Before compiling this project, you should have some software packages installed on your Arch Linux ARM(besides the basic development tools such as gcc, etc.):
#### WiringPi(a GPIO access library for Raspberry Pi)
git clone git://git.drogon.net/wiringPi && cd wiringPi && ./build

#### glog(Google logging module):
pacman -S google-glog

****

## How to know your Raspberry Pi revision
Install WiringPi, then execute command "gpio -v" to get RPi's revision, the output info looks like:

    gpio version: 2.25
    Copyright (c) 2012-2015 Gordon Henderson
    This is free software with ABSOLUTELY NO WARRANTY.
    For details type: gpio -warranty
    
    Raspberry Pi Details:
      Type: Model B+, Revision: 1.2, Memory: 512MB, Maker: Sony

****

## Future work
Add an UI for this project(may be web-based?).
