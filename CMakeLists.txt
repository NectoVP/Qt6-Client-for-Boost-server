cmake_minimum_required(VERSION 3.12)
project(app)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network LinguistTools)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/translations)

file(GLOB_RECURSE APP_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.h"
)

add_executable(${PROJECT_NAME} ${APP_SOURCES})
add_subdirectory(lib)
add_subdirectory(tests)

enable_testing()

target_link_libraries(${PROJECT_NAME} lib)
target_link_libraries(${PROJECT_NAME} 
    Qt6::Widgets 
    Qt6::Network
)

foreach(lang en ru)
    set(ts_file ${CMAKE_CURRENT_SOURCE_DIR}/translations/${lang}.ts)
    if(NOT EXISTS ${ts_file})
        file(WRITE ${ts_file} "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<!DOCTYPE TS>\n<TS version=\"2.1\"></TS>")
    endif()
endforeach()

set(TRANSLATION_FILES)
foreach(lang en ru)
    list(APPEND TRANSLATION_FILES ${CMAKE_CURRENT_SOURCE_DIR}/translations/${lang}.ts)
endforeach()

qt_create_translation(QM_FILES
    ${APP_SOURCES}
    ${TRANSLATION_FILES}
)

qt_add_resources(${PROJECT_NAME} "translations"
    PREFIX "/i18n"
    FILES ${QM_FILES}
)

add_custom_target(update_translations
    COMMAND ${Qt6_LUPDATE_EXECUTABLE}
        -locations relative
        ${APP_SOURCES}
        -ts ${TRANSLATION_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)