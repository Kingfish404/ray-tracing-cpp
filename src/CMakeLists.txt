cmake_minimum_required(VERSION 3.13)

set(CMAKE_PROJECT_NAME ray-tracing)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

project(${CMAKE_PROJECT_NAME})

add_executable(
    ${CMAKE_PROJECT_NAME}
    main.cpp
)

target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    pthread
)