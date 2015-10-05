# A time-controllable human sense light based on Raspberry Pi.
I only tested this program on Raspberry Pi Model B, but it should work on other models of RPi too, except that the hardware wiring should be some difference, and the code should not need to be modified.

Environment:

**Raspberry Pi model: B**

**OS: Arch Linux ARM**

****

You can use this project to build a human sense light, in short, when a person gets close to the light, the pyroelectric module connected to Raspberry Pi will detect the human movement & ouput a high level to one of the GPIO port on RPi, then the program runs on RPi will light up some LEDs connected to it.
Also, you can set the time range of day, only in those time range the LEDs can be lighten up, and in other time range the LEDs will be turned off(this feature is still under developing).

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
