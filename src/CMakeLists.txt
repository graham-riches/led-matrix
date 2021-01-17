# build the main application
set(BINARY ${PROJECT_NAME})

# set all the source files for the executable
set(SOURCES   
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    )

# create the executable
add_executable(${BINARY} ${SOURCES})
add_library(${BINARY}_lib STATIC ${SOURCES} )

# link with the RPI_RGB library
target_link_libraries(${BINARY} rpi-rgb-led-matrix)
