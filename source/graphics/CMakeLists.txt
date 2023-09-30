# Build the graphics library
set(BINARY LedGraphicsLib)

# Set graphics lib source files
set(SOURCES   
    ${CMAKE_CURRENT_SOURCE_DIR}/fonts/character.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fonts/font.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix/config_parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text_box.cpp
)

# Create the executable
add_library(${BINARY} STATIC ${SOURCES})

# Export library headers
target_include_directories(${BINARY} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/fonts
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix
    ${CMAKE_CURRENT_SOURCE_DIR}/utilities
)

target_link_libraries(${BINARY}
    -static-libstdc++ 
    rpi-rgb-led-matrix 
    Threads::Threads 
    ${Boost_SYSTEM_LIBRARY} 
    ${Boost_THREAD_LIBRARY}     
)