cmake_minimum_required(VERSION 3.1...3.15)

# create the project name
project(led_matrix_test)

# set the version standards 
enable_language(C CXX ASM)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set visual studio build flags
set(CMAKE_CXX_FLAGS_RELEASE "/MT")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi /Ob0 /Od /RTC1")

# search for mandatory packages
find_package(Threads REQUIRED)

# set the parent directory
get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)


# set the binary and add custom sources as needed
set(BINARY led_matrix_tests)
set(SOURCES
	# add unit tests here
    main.cpp
    config_tests.cpp

    # add each application file to test here
    ${PARENT_DIR}/source/config_parser.cpp      
	)

add_executable(${BINARY} ${SOURCES})
target_link_libraries(${BINARY} gtest gtest_main)
add_test(NAME ${BINARY} COMMAND ${BINARY})

# set the include directories for the project
include_directories(
    ${PARENT_DIR}/source
	${PARENT_DIR}/modules/rpi-rgb-led-matrix/include
    ${PARENT_DIR}/modules/rpi-rgb-led-matrix/lib
    ${CMAKE_SOURCE_DIR}/googletest/googletest/include
)

# add submodules
add_subdirectory(${CMAKE_SOURCE_DIR}/googletest)