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
    main.cpp
    expected_tests.cpp
    font_parser_tests.cpp
	)

add_executable(${BINARY} ${SOURCES})
target_link_libraries(${BINARY} gtest gtest_main)
add_test(NAME ${BINARY} COMMAND ${BINARY})

# set the include directories for the project
include_directories(
    ${PARENT_DIR}/source
	${PARENT_DIR}/modules/rpi-rgb-led-matrix/include
    ${PARENT_DIR}/modules/rpi-rgb-led-matrix/lib
    ${PARENT_DIR}/modules/json/include
    ${CMAKE_SOURCE_DIR}/modules/range-v3/include/range/v3/action
    ${CMAKE_SOURCE_DIR}/modules/range-v3/include/range/v3/algorithm
    ${CMAKE_SOURCE_DIR}/modules/range-v3/include/range/v3/detail
    ${CMAKE_SOURCE_DIR}/modules/range-v3/include/range/v3/experimental
    ${CMAKE_SOURCE_DIR}/modules/range-v3/include/range/v3/functional
    ${CMAKE_SOURCE_DIR}/modules/range-v3/include/range/v3/iterator
    ${CMAKE_SOURCE_DIR}/modules/range-v3/include/range/v3/range
    ${CMAKE_SOURCE_DIR}/modules/range-v3/include/range/v3/utility
    ${CMAKE_SOURCE_DIR}/modules/range-v3/include/range/v3/view
    ${CMAKE_SOURCE_DIR}/googletest/googletest/include
)

# add submodules
add_subdirectory(${CMAKE_SOURCE_DIR}/googletest)