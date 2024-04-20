#!/bin/bash

# Info: For debugging use -g and comment the && rm part

BASEDIR=$(dirname "$(readlink -f "$0")")

g++ $BASEDIR/Image/ImageUtilsTest.cpp -o $BASEDIR/Image/ImageUtilsTest && $BASEDIR/Image/ImageUtilsTest && rm $BASEDIR/Image/ImageUtilsTest

g++ $BASEDIR/Threads/ThreadPoolTest.cpp -o $BASEDIR/Threads/ThreadPoolTest && $BASEDIR/Threads/ThreadPoolTest && rm $BASEDIR/Threads/ThreadPoolTest

# g++ $BASEDIR/Utils/WebUtilsTest.cpp -o $BASEDIR/Utils/WebUtilsTest -l curl -l xml2 -l libxml2 -I /usr/include/libxml2 -f permissive && $BASEDIR/Utils/WebUtilsTest && rm $BASEDIR/Utils/WebUtilsTest

g++ $BASEDIR/Stdlib/SIMD/SIMD_HelperTest.cpp -o $BASEDIR/Stdlib/SIMD/SIMD_HelperTest && $BASEDIR/Stdlib/SIMD/SIMD_HelperTest && rm $BASEDIR/Stdlib/SIMD/SIMD_HelperTest

g++ -mavx -maes -msse -msse2 -msse3 -msse4.1 -mavx512f -mpclmul -mavx512dq -march=native $BASEDIR/Stdlib/IntrinsicsTest.cpp -o $BASEDIR/Stdlib/IntrinsicsTest && $BASEDIR/Stdlib/IntrinsicsTest && rm $BASEDIR/Stdlib/IntrinsicsTest

g++ -mavx -maes -msse -msse2 -msse3 -msse4.1 -mavx512f -mpclmul -mavx512dq -march=native $BASEDIR/Stdlib/SIMD/SIMD_F32Test.cpp -o $BASEDIR/Stdlib/SIMD/SIMD_F32Test && $BASEDIR/Stdlib/SIMD/SIMD_F32Test && rm $BASEDIR/Stdlib/SIMD/SIMD_F32Test
