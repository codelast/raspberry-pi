# A time-controllable human sense light based on Raspberry Pi.
I only tested this program on Raspberry Pi Model B, but it should work on other models of RPi too, except that the hardware wiring should be some difference, and the code should not need to be modified.

Environment:

**Raspberry Pi model: B**

**OS: Arch Linux ARM**

****

This is a sense light used at home, when a person gets close to the light, the pyroelectric module connected to Raspberry Pi will detect the human movement & ouput a high level to one of the GPIO port on RPi, then the program runs on RPi will light up some LEDs connected to it.
Also, you can set the time range of day, only in those time range the LEDs can be lighten up, and in other time range the LEDs will be turned off(this feature is still under developing).
