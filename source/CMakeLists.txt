# build the main application
set(BINARY ${PROJECT_NAME})

# set all the source files for the executable
set(SOURCES   
    ${CMAKE_SOURCE_DIR}/source/main.cpp
    ${CMAKE_SOURCE_DIR}/source/config_parser.cpp
    ${CMAKE_SOURCE_DIR}/source/font_parser.cpp
    )

# create the executable
add_executable(${BINARY} ${SOURCES})
add_library(${BINARY}_lib STATIC ${SOURCES} )

# link with the RPI_RGB library
target_link_libraries(${BINARY} -static-libstdc++ rpi-rgb-led-matrix Threads::Threads ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
