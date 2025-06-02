# set a minumum version of cmake
cmake_minimum_required(VERSION 3.31.2)

project(qt-shared-memory
    VERSION 1.0.0
    DESCRIPTION "QT Hello World"
    LANGUAGES CXX
)

# QT Setup
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
