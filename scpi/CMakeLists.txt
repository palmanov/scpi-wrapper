cmake_minimum_required(VERSION 3.14)

project(scpi-wrapper LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

include_directories(scpi-lib/include)

add_executable(scpi-wrapper
  main.cpp
)
target_link_libraries(scpi-wrapper Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network scpi-lib)

install(TARGETS scpi-wrapper
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
