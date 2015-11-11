#****************************************
# Makefile for timer-sense-light project.
# Author: Darran Zhang @ codelast.com
#****************************************

# DEBUG can be set to YES to include debugging info, or NO otherwise
DEBUG = NO

CC = gcc
CXX = g++

BASIC_FLAG = -Wall
ifeq (YES, ${DEBUG})
   FLAG = ${BASIC_FLAG} -g -DDEBUG
else
   FLAG = ${BASIC_FLAG}
endif

BASIC_INCLUDE_PATH = -I./include

CXXFLAGS = ${FLAG} \
           ${BASIC_INCLUDE_PATH}/3rd \
           ${BASIC_INCLUDE_PATH}

CFLAGS = ${CXXFLAGS}

LDFLAGS = -lpthread -lconfig++ -lglog -lwiringPi

C_SOURCE_FILE = ./src/3rd/mongoose.c
CXX_SOURCE_FILE = ./src/timer_sense_light.cpp \
        ./src/config_loader.cpp \
        ./src/util.cpp

C_OBJECT = ./src/3rd/mongoose.o

timer_sense_light:
	$(CC) $(C_SOURCE_FILE) -c -o ${C_OBJECT} $(CFLAGS)
	$(CXX) ${C_OBJECT} $(CXX_SOURCE_FILE) -o $@ $(CXXFLAGS) ${LDFLAGS}

install:
	rm -rf deploy
	mkdir -p deploy/conf
	mkdir -p deploy/script
	cp -f script/* deploy/script
	mv -f timer_sense_light deploy
	cp -f conf/* deploy/conf
	cp -rf web-root deploy

.PHONY : clean
clean:
	rm -f timer_sense_light *.o ./src/*.o ./src/3rd/*.o
	rm -rf ./deploy
