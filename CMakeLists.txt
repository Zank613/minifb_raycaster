cmake_minimum_required(VERSION 3.10)
project(minifb_raycaster)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add the directory where the include files are located
include_directories(include)

# Add subdirectory for minifb dependencies
add_subdirectory(dependencies/minifb)

# Globbing source files
file(GLOB SOURCES "src/*.c")

# Exclude raycasting.c from the compiled sources if needed
list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/raycasting.c)

# Add executable
add_executable(minifb_raycaster src/raycasting.c ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    minifb
)
