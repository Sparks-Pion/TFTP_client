cmake_minimum_required(VERSION 3.23)

project(TFTP_Client LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "TFTP Client")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# execution-charset: UTF-8
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Concurrent REQUIRED)

add_executable(${PROJECT_NAME}
        WIN32
        main.cpp
        tftp.cpp
        tftp.h
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        icon/icon.rc
)

target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Concurrent
        ws2_32
        )

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin")
