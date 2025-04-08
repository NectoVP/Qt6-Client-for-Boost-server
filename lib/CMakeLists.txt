cmake_minimum_required(VERSION 3.12)
project(lib)

add_library(${PROJECT_NAME}
    ImageHandler.h
    ImageHandler.cpp
    json.hpp
    NetworkHandler.h
    NetworkHandler.cpp
    MainWindow.h
    MainWindow.cpp
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Network)