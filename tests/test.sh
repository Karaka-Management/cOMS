#!/bin/bash

# Info: For debugging use -g and comment the && rm part

BASEDIR=$(dirname "$(readlink -f "$0")")

g++ $BASEDIR/Image/ImageUtilsTest.cpp -o $BASEDIR/Image/ImageUtilsTest && $BASEDIR/Image/ImageUtilsTest && rm $BASEDIR/Image/ImageUtilsTest

g++ $BASEDIR/Threads/ThreadPoolTest.cpp -o $BASEDIR/Threads/ThreadPoolTest && $BASEDIR/Threads/ThreadPoolTest && rm $BASEDIR/Threads/ThreadPoolTest

g++ $BASEDIR/Utils/WebUtilsTest.cpp -o $BASEDIR/Utils/WebUtilsTest -l curl -l xml2 -l libxml2 -I /usr/include/libxml2 && $BASEDIR/Utils/WebUtilsTest && rm $BASEDIR/Utils/WebUtilsTest
