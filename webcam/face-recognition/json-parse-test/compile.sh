#!/bin/bash

# before compiling the source, you should have rapidjson library header files under one of the header file searching paths, e.g. /usr/include/
g++ parse_json_string.cpp -o parse_json_string
