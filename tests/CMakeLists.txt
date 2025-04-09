cmake_minimum_required(VERSION 3.12)
project(tests)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network Core Gui Test)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

enable_testing()

add_executable(${PROJECT_NAME} gui_tests.h gui_tests.cpp)

target_link_libraries(${PROJECT_NAME} lib)
target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Network Qt6::Core Qt6::Gui Qt6::Test)

add_test(NAME gui_test COMMAND ${PROJECT_NAME})