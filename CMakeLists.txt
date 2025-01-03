cmake_minimum_required(VERSION 3.5.0)
project(nBodySim VERSION 0.1.0 LANGUAGES C CXX)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

set(SOURCES
    src/main.cpp
)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-system sfml-network sfml-graphics sfml-window)