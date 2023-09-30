# build the main application
set(BINARY ${PROJECT_NAME})

# set all the source files for the executable
set(SOURCES   
    ${CMAKE_SOURCE_DIR}/source/main.cpp
    ${CMAKE_SOURCE_DIR}/source/app/config_parser.cpp
    ${CMAKE_SOURCE_DIR}/source/graphics/fonts/font.cpp
    ${CMAKE_SOURCE_DIR}/source/graphics/fonts/character.cpp
    ${CMAKE_SOURCE_DIR}/source/graphics/primatives.cpp
    ${CMAKE_SOURCE_DIR}/source/graphics/text_box.cpp
    )

# create the executable
add_executable(${BINARY} ${SOURCES})

# link with the RPI_RGB library
target_link_libraries(${BINARY} -static-libstdc++ rpi-rgb-led-matrix Threads::Threads ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} fmt::fmt)
