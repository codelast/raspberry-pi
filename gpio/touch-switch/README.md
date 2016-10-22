Demo on how to use a touch-switch module.

![](https://raw.githubusercontent.com/codelast/raspberry-pi/master/gpio/touch-switch/demo/touch_sensor.jpeg)

****

该目录下的程序演示了如何使用一个触摸开关。该触摸开关模块是一个基于触摸检测IC（TTP223B）的电容式点动型触摸开关模块。常态下，模块输出低电平，模式为低功耗模式；当用手指触摸相应位置时，模块会输出高电平，模式切换为快速模式；当持续12秒没有触摸时，模式又切换为低功耗模式。
该触摸开关模块的控制接口：共3个引脚（GND、VCC、SIG），GND为地，VCC为供电电源，SIG为数字信号输出脚。
在网上很容易购买到该触摸开关模块，且很廉价。
