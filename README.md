### 关注我的微信公众号获取更多信息（Follow my WeChat public account to get more info）：
![](https://raw.githubusercontent.com/codelast/raspberry-pi/master/codelast_wechat_qr_code.jpg)

# Raspberry Pi related code
My video courses corresponding to part of these codes on jikexueyuan.com: [link](http://search.jikexueyuan.com/course/?q=%E6%A0%91%E8%8E%93%E6%B4%BE "Click to watch")

在极客学院网站上，有我录制的和这些代码（部分）相关的视频课程: [链接](http://search.jikexueyuan.com/course/?q=%E6%A0%91%E8%8E%93%E6%B4%BE "点击跳转到极客学院查看")

**Environment: Raspberry Pi model B+/B**
* For each subproject, if there is no explicit declaration about the applicable RPi model, then it's Model B+.

**OS: Arch Linux ARM**

****

Description of each subproject:

## [auto-report-ip-via-email](./auto-report-ip-via-email)
Script & config files to let Raspberry Pi report its IP address to an specified Email address at system startup.

## [emacs-config](./emacs-config)
Emacs config files on my Raspberry Pi.

## [gpio](./gpio)
GPIO related applications on Raspberry Pi.
### gpio/lcd
Display some messages on a LCD1602 screen, including the examples of using/not using an PCF8574 I2C module.

![](https://raw.githubusercontent.com/codelast/raspberry-pi/master/gpio/lcd/demo/lcd1602-8bit-display.png)
![](https://raw.githubusercontent.com/codelast/raspberry-pi/master/gpio/lcd/demo/lcd1602-with-i2c-2.jpg)
### gpio/led/blink
Blink the LED connected to the GPIO port on Raspberry Pi by using the WiringPi library.
### gpio/led/input-control
Control a LED through a button(pressed or not).
### gpio/led/pwm
Control the brightness of a LED through PWM.
### gpio/piezo-element
Play music through a piezo element.
### gpio/sense-light
Control some LEDs through an pyroelectric module.
### gpio/stepper-motor
Control a stepper motor through the GPIO ports on Raspberry Pi.
![](https://raw.githubusercontent.com/codelast/raspberry-pi/master/gpio/stepper-motor/demo/stepper-motor.png)
### [gpio/timer-sense-light](./gpio/timer-sense-light)
A time-controllable human sense light based on Raspberry Pi.

[![Click to watch Youtube video demo](https://raw.githubusercontent.com/codelast/raspberry-pi/master/gpio/timer-sense-light/demo/finished_product_1.jpg)](https://www.youtube.com/watch?v=_6llPyMW7_M)

Please refer to the README of this subproject for details.

## [real-time-clock](./real-time-clock)
Slight modification to the ds1302.c shipped with WiringPi to work better with the DS1302 Real-Time Clock module.
![](https://raw.githubusercontent.com/codelast/raspberry-pi/master/real-time-clock/demo/ds1302_1.jpg)

## [rpi-remote-monitoring-system](./rpi-remote-monitoring-system)
A remote monitoring system based on Raspberry Pi

![](https://raw.githubusercontent.com/codelast/raspberry-pi/master/rpi-remote-monitoring-system/demo/pi-controller_1.png)

Please refer to the README of this subproject for details.

## [webcam](./webcam)
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
