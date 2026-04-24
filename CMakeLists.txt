cmake_minimum_required(VERSION 3.16)
project(Connectify VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

qt_add_executable(Connectify
    ${SOURCES}
    ${HEADERS}
    src/resources/resources.qrc
)

target_link_libraries(Connectify PRIVATE Qt6::Widgets)

target_include_directories(Connectify PRIVATE src)
