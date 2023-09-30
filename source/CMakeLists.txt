# Build the main application
set(BINARY ${PROJECT_NAME})

# Cet all the source files for the executable
set(SOURCES   
    ${CMAKE_SOURCE_DIR}/source/main.cpp
)

add_subdirectory(graphics)

# Create the executable
add_executable(${BINARY} ${SOURCES})

# Link in libraries
target_link_libraries(${BINARY} 
    -static-libstdc++ 
    LedGraphicsLib 
    Threads::Threads 
    ${Boost_SYSTEM_LIBRARY} 
    ${Boost_THREAD_LIBRARY} 
    fmt::fmt
)
