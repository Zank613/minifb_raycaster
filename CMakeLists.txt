cmake_minimum_required(VERSION 3.10)
project(minifb_raycaster LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(dependencies/minifb)

# Include all source files in the src directory
file(GLOB SOURCES "src/*.c")

# Include directories
include_directories(include)

# Create the test1 executable
add_executable(test1 ${SOURCES})

# Link the minifb library
target_link_libraries(test1 PRIVATE minifb)

# Set the linker language to CXX to use the C++ linker
set_target_properties(test1 PROPERTIES LINKER_LANGUAGE CXX)

# Force the console subsystem to ensure 'main' is used as the entry point
if(MINGW)
    target_link_options(test1 PRIVATE "-mconsole")
elseif(MSVC)
    target_link_options(test1 PRIVATE "/SUBSYSTEM:CONSOLE")
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)
