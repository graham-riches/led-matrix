# build the main rpi-rgb-led-matrix controller library
set(BINARY rpi-rgb-led-matrix)

# set all the source files for the executable
file(GLOB RPI_RGB_LED_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/rpi-rgb-led-matrix/lib/*.cc" "${CMAKE_CURRENT_SOURCE_DIR}/rpi-rgb-led-matrix/lib/*.c")

# log the files found
message(STATUS "Found RPI_RGB_LED_SOURCES:")
foreach(file_path ${RPI_RGB_LED_SOURCES})
    message(${file_path})
endforeach()

# create the library
add_library(${BINARY} STATIC ${RPI_RGB_LED_SOURCES} )

# add the include headers to the target to compile it
target_include_directories(${BINARY} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)