![](https://raw.githubusercontent.com/codelast/raspberry-pi/master/real-time-clock/demo/ds1302_1.jpg)

Slight modification to the ds1302.c shipped with WiringPi to work better with the DS1302 Real-Time Clock module.

Usage instruction:
* ./ds1302.sh：print the date & time stored in DS1302 module
* ./ds1302.sh -rtest：test whether the DS1302 module works fine
* ./ds1302.sh -sdsc：write OS time into the DS1302 module
* ./ds1302.sh -slc：write the time stored in DS1302 module into OS
* ./ds1302.sh -slc b：same effect as above command, especially that it's for RPi model B(default model B+ if no 2nd param provided)


****

对WiringPi附带的 ds1302.c 做了少量修改，使其更好地配合 DS1302 实时时钟(硬件时钟)模块工作。

使用方法：
* ./ds1302.sh：打印出 DS1302 模块中储存的时间
* ./ds1302.sh -rtest：测试 DS1302 模块是否工作正常
* ./ds1302.sh -sdsc：将Linux系统时间写入 DS1302 模块中
* ./ds1302.sh -slc：将 DS1302 模块中的时间写入Linux系统
* ./ds1302.sh -slc b：与上面命令效果相同，但只适用于树莓派model B（如果不提供第2个参数的话默认是model B+）
