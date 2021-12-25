cmake_minimum_required(VERSION 3.20)

set(CMAKE_PROJECT_NAME ray-tracing)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

project(${CMAKE_PROJECT_NAME})

add_executable(
        ${CMAKE_PROJECT_NAME}
        main.cpp
)