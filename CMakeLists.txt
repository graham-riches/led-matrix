cmake_minimum_required(VERSION 3.1...3.15)

# create the project name
project(led_matrix)

# set the version standards 
enable_language(C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# add a custom target to generate the git version string
add_custom_target( BuildVersionHeader ALL DEPENDS version.h )

# set the include directories for the project
include_directories(
    ${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/modules/rpi-rgb-led-matrix/include
	${CMAKE_SOURCE_DIR}/modules/rpi-rgb-led-matrix/lib
	
)

# add custom build compiler flags
SET(GCC_CFLAGS "-g -O0 -Wno-psabi")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CFLAGS}")

# build any sub directories
add_subdirectory(src)
add_subdirectory(modules/rpi-rgb-led-matrix)

# run the prebuild script
add_custom_command( OUTPUT version.h COMMAND ${CMAKE_SOURCE_DIR}/scripts/prebuild.sh )