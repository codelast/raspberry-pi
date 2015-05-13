# raspberry-pi
Raspberry Pi related code, which can run on the following environment:


**Raspberry Pi model: B+**

**OS: Arch Linux ARM**

Description of each subproject:

## auto-report-ip-via-email
Script & config files to let Raspberry Pi report its IP address to an specified Email address at system startup.

## emacs-config
Emacs config files on my Raspberry Pi.

## webcam
USB camera related applications on Raspberry Pi.
### webcam/motion-detection
Demonstrate how to detect motion in front of an USB camera using OpenCV.
### webcam/image-capture/opencv
Demonstrate how to capture an image from USB camera on Raspberry Pi using OpenCV.
### webcam/image-capture/v4l2
Demonstrate how to capture an image from USB camera on Raspberry Pi using V4L2, the code of this subproject is forked from https://github.com/twam/v4l2grab/tree/master & I add a shell script to invoke the program.
### webcam/video-capture
Demonstrate how to capture video from USB camera on Raspberry Pi & save to a file using OpenCV.
### webcam/face-recognition
Demonstrate how to implement a face-recognition application on Raspberry Pi.

## gpio
GPIO related applications on Raspberry Pi.
### gpio/led/blink
Blink the LED connected to the GPIO port on Raspberry Pi by using the WiringPi library.
### gpio/led/input-control
Control a LED through a button(pressed or not).
### gpio/led/pwm
Control the brightness of a LED through PWM.
### gpio/sense-light
Control some LEDs through an pyroelectric module.
