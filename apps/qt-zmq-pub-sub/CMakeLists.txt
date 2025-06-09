cmake_minimum_required(VERSION 3.31.2)

project(qt-zmq-pub-sub
    VERSION 1.0.0
    DESCRIPTION "QT ZMQ Pub Sub Application"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific configurations
if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE TRUE)
elseif(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")
    set(CMAKE_OSX_ARCHITECTURES "${CMAKE_HOST_SYSTEM_PROCESSOR}")
elseif(UNIX AND NOT APPLE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Qt Setup
find_package(Qt6 REQUIRED COMPONENTS Core Network)
qt_standard_project_setup()

# Create executable
qt_add_executable(${PROJECT_NAME}
    src/main.cpp
    src/int_converter.cpp
    src/memory_mapper.cpp
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Network
)

# Platform-specific linking
if(WIN32)
elseif(APPLE)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()

# Set executable properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
