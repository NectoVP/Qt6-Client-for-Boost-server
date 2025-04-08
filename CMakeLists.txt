cmake_minimum_required(VERSION 3.12)
project(app)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(${PROJECT_NAME} main.cpp)
add_subdirectory(lib)

target_link_libraries(${PROJECT_NAME} lib)
target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Network)