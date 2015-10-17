# A remote monitoring system based on Raspberry Pi - this is an experimental project which may not be accomplished in a certain time period.
Project description: by this system you can use An Android App to control a Raspberry-Pi-based hardware to monitoring a place through the webcam connected to Pi, and you can control the webcam to rotate an angle to get a broader view.

****
The system is made of 2 parts:

**pi-commander - An Android application(Coding work NOT started yet)**

This is the UI for end users. It gives orders to Raspberry Pi, make it to execute some commands, receives result from it & display the result in the App.

**pi-controller - A Raspberry-Pi-based hardware(Coding work ongoing but NOT done yet)**

This is the backend monitoring part of the system. There is a webcam binded with a stepper motor, and the stepper motor is connected to Raspberry Pi, which can rotate according to the command from pi-commander, this will change the view angle of the system. The webcam can also take photos, and the photo will be sent to pi-commander, which means it will be displayed on the Android device.

I built this part with some Lego blocks, as the images shown below:

![](https://raw.githubusercontent.com/codelast/raspberry-pi/master/rpi-remote-monitoring-system/demo/pi-controller_1.png)
![](https://raw.githubusercontent.com/codelast/raspberry-pi/master/rpi-remote-monitoring-system/demo/pi-controller_2.png)

****
## Dependencies installation
Before compiling the pi-controller project, you should have some software packages installed on your Arch Linux ARM(besides the basic development tools such as gcc, make, etc.):
#### WiringPi(a GPIO access library for Raspberry Pi)
git clone git://git.drogon.net/wiringPi && cd wiringPi && ./build

#### OpenCV(Open Source Computer Vision Library)
pacman -S opencv opencv-samples pkg-config

#### glog(Google logging module):
pacman -S google-glog

#### libconfig(C/C++ library for processing configuration files):
pacman -S libconfig

****
## Communication Protocol

There is a JSON-RPC server runs on pi-controller, which accepts remote commands from pi-commander & executes them. So the pi-commander should send JSON format commands to pi-controller, when doing End-To-End test you should also use some tools(like curl) to do this.
Description of the JSON data:

{"id":1442818183,"method":"rotateMotor","direction":1,"round":150,"validate":"38e4de9df62d7a7395340a22226a041944c1fdc570d05c356f9d9053b5271c4d1932d474a2bee342488a379058d96b4e5dcd13c15536566a242c5af70fa97ea7"}

in which,
#### id: JSON-RPC message id, we use timestamp(in seconds) as the message id
#### method: the command to give Raspberry Pi to execute, e.g. the "rotateMotor" method name means that we want Raspberry Pi to rotate the stepper motor connected to it.
#### direction: one of the method parameters. When rotating the stepper motor, we may also want to specify the rotation driection, 1 for clockwise and -1 for counter clockwise.
#### round: one of the method parameters, used to control the rotation angle. 150 does not mean 150 degrees but is a relatively number, I did not test the conversion relation between this number & the real rotation angle, so currently we should not set this value too large.
#### validate: the validation string to check whether this is a authorized request, if not, it will be discarded by the pi-controller.
#### how to generate the validate string: SHA-512(id+token)

****
### E2E test guide

#### Build the hardware system, including connecting stepper motor & webcam to Raspberry Pi, etc.
#### make && make install
#### Run the pi-controller program on Raspberry Pi: cd deploy/bin && ./run-pi-controller.sh
#### install curl(if you are using Arch Linux ARM): pacman -S curl
#### Run the pi-controller/test/e2e-test/make-json-rpc-call.sh script under pi-controller project and check the result returned by the server
