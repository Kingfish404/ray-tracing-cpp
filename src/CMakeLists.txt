cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME ray-tracing)
set(CXX_STANDARD 17)
set(C_STANDARD 17)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
    main.cpp
)