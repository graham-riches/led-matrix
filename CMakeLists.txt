cmake_minimum_required(VERSION 3.15)

# create the project name
project(led_matrix)

# set the version standards 
enable_language(C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# search for mandatory packages
find_package(Threads REQUIRED)
find_package(Boost REQUIRED)

if(Boost_FOUND)
    set(Boost_USE_STATIC_LIBS ON)
    message(STATUS "FOUND BOOST: ")
    message(STATUS ${Boost_INCLUDE_DIRS})
    include_directories(${Boost_INCLUDE_DIRS})
endif()

# add a custom target to generate the git version string
add_custom_target( BuildVersionHeader ALL DEPENDS version.h )

# set the include directories for the project
include_directories(
    ${CMAKE_SOURCE_DIR}/source
    ${CMAKE_SOURCE_DIR}/source/app
    ${CMAKE_SOURCE_DIR}/source/app/clocks
    ${CMAKE_SOURCE_DIR}/source/app/tasks    
    ${CMAKE_SOURCE_DIR}/source/graphics    
    ${CMAKE_SOURCE_DIR}/source/graphics/fonts    
    ${CMAKE_SOURCE_DIR}/source/graphics/matrix
    ${CMAKE_SOURCE_DIR}/source/graphics/utilities
    ${CMAKE_SOURCE_DIR}/source/io
    ${CMAKE_SOURCE_DIR}/source/reactive
	${CMAKE_SOURCE_DIR}/modules/rpi-rgb-led-matrix/include
    ${CMAKE_SOURCE_DIR}/modules/rpi-rgb-led-matrix/lib
    ${CMAKE_SOURCE_DIR}/modules/json/include
    ${CMAKE_SOURCE_DIR}/modules/range-v3/include
    ${CMAKE_SOURCE_DIR}/modules/fmt/include
)

# add custom build compiler flags
SET(GCC_CFLAGS "-g0 -O2 -Wno-psabi")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CFLAGS}")

# build any sub directories
add_subdirectory(source)
add_subdirectory(modules)
add_subdirectory(modules/json)
add_subdirectory(modules/range-v3)
add_subdirectory(modules/fmt)

# run the prebuild script
add_custom_command( OUTPUT version.h COMMAND ${CMAKE_SOURCE_DIR}/scripts/prebuild.sh )