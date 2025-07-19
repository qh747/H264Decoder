# H264 Decoder
H264 Decoder

# H264 Standard

ITU-T H264 (v15) (08/2024) Advanced video coding for generic audiovisual services

# Compile Option

``` cmd

 1. OPTION_BUILD_SHARED_LIB: build shared library. default is OFF.
 2. OPTION_BUILD_STATIC_LIB: build static library. default is ON.
 3. OPTION_BUILD_TESTS: build test program. default is ON.

``` 

# Build

``` cmake

 1. cmake -B build -DOPTION_BUILD_SHARED_LIB=ON
 2. cmake --build build -j4

```

# Test

``` cmd

 1. cd bin/tests
 2. ./TestFileReader

```

# Clear

``` cmd
 
 rm -rf build bin lib

```