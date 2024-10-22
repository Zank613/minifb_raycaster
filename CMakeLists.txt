cmake_minimum_required(VERSION 3.10)
project(minifb_raycaster)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(dependencies/minifb)

add_executable(minifb_raycaster src/raycaster.c)

target_link_libraries(${PROJECT_NAME}
    minifb
)