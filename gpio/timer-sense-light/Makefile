#****************************************
# Makefile for timer-sense-light project.
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

CXXFLAGS = ${FLAG} \
           ${BASIC_INCLUDE_PATH}

CFLAGS = ${CXXFLAGS}

LDFLAGS = -lglog -lwiringPi

OBJECTS=./src/timer_sense_light.o \
        ./src/config_loader.o \
        ./src/util.o

timer_sense_light: $(OBJECTS)
	$(CC) $(CXXFLAGS) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

install:
	rm -rf deploy
	mkdir -p deploy/conf
	mkdir -p deploy/bin
	cp -f bin/* deploy/bin
	mv -f timer_sense_light deploy
	cp -f conf/time-range.txt deploy/conf

.PHONY : clean
clean:
	rm -f timer_sense_light *.o ./src/*.o ./src/3rd/*.o
	rm -rf ./deploy
