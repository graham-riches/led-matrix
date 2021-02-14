cmake_minimum_required(VERSION 3.1...3.15)

# create the project name
project(led_matrix_test)

# set the version standards 
enable_language(C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

SET(GCC_CFLAGS "-g -O0 -Wno-psabi")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CFLAGS}")

# search for mandatory packages
find_package(Threads REQUIRED)

# set the parent directory
get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)


# set the binary and add custom sources as needed
set(BINARY led_matrix_tests)
set(SOURCES
	# add unit tests here
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/expected/expected_tests.cpp
    ${CMAKE_SOURCE_DIR}/font_parser/font_tests.cpp

    # add source files here
    ${PARENT_DIR}/source/fonts/font.cpp
    ${PARENT_DIR}/source/fonts/character.cpp
	)

add_executable(${BINARY} ${SOURCES})
target_link_libraries(${BINARY} gtest gtest_main)
add_test(NAME ${BINARY} COMMAND ${BINARY})

# set the include directories for the project
include_directories(
    ${PARENT_DIR}/source
    ${PARENT_DIR}/source/reactive
    ${PARENT_DIR}/source/fonts
    ${PARENT_DIR}/source/graphics
    ${PARENT_DIR}/source/io
	${PARENT_DIR}/modules/rpi-rgb-led-matrix/include
    ${PARENT_DIR}/modules/rpi-rgb-led-matrix/lib
    ${PARENT_DIR}/modules/json/include
    ${PARENT_DIR}/modules/range-v3/include
    ${CMAKE_SOURCE_DIR}/googletest/googletest/include
)

# add submodules: Note: the submodule include here is a bit of a hack
add_subdirectory(${CMAKE_SOURCE_DIR}/googletest)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../modules/range-v3 ${CMAKE_CURRENT_BINARY_DIR}/range-v3)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../modules/json ${CMAKE_CURRENT_BINARY_DIR}/json)
