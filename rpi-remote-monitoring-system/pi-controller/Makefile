#****************************************
# Makefile for pi-controller project.
# Author: Darran Zhang @ codelast.com
#****************************************

# DEBUG can be set to YES to include debugging info, or NO otherwise
DEBUG = NO

CC = g++

BASIC_FLAG = -Wall -D_GNU_SOURCE -D_REENTRANT
ifeq (YES, ${DEBUG})
   FLAG = ${BASIC_FLAG} -g -DDEBUG
else
   FLAG = ${BASIC_FLAG}
endif

BASIC_INCLUDE_PATH = -I./include
BASIC_LIB_PATH = -L./lib

OPENCV_INCLUDE_PATH = -I/usr/include/opencv2/ `pkg-config --cflags opencv`
OPENCV_LIB_PATH = `pkg-config --libs opencv`

CXXFLAGS = ${FLAG} \
           ${BASIC_INCLUDE_PATH} \
           ${BASIC_INCLUDE_PATH}/3rd \
           ${OPENCV_INCLUDE_PATH}

LDFLAGS = ${OPENCV_LIB_PATH} \
          -lconfig++ -lglog -lwiringPi

OBJECTS=./src/pi_controller.o \
        ./src/util.o \
        ./src/config.o \
        ./src/motor_control.o \
        ./src/webcam_control.o \
	./src/3rd/mongoose.o \
	./src/3rd/sha512.o

pi_controller: $(OBJECTS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

install:
	rm -rf deploy
	mkdir -p deploy/conf
	mkdir -p deploy/bin
	cp -f bin/* deploy/bin
	mv -f pi_controller deploy/bin
	cp -f conf/pi-controller.cfg deploy/conf

.PHONY : clean
clean:
	rm -f pi_controller *.o ./src/*.o ./src/3rd/*.o
	rm -rf ./deploy
