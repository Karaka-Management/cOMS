#!/bin/bash

BASEDIR=$(dirname "$(readlink -f "$0")")

g++ $BASEDIR/Image/ImageUtilsTest.cpp -o $BASEDIR/Image/ImageUtilsTest && $BASEDIR/Image/ImageUtilsTest && rm $BASEDIR/Image/ImageUtilsTest