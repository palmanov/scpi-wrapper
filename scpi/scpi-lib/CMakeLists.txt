cmake_minimum_required(VERSION 3.14)

project(scpi-lib LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

include_directories(include)

add_library(scpi-lib
  include/controller.h src/controller.cpp
  include/externalrequestreceiver.h src/externalrequestreceiver.cpp
  include/scpi_command.h src/scpi_command.cpp
  include/device.h src/device.cpp
  include/monitor.h src/monitor.cpp
  include/signalgenerator.h
)
target_link_libraries(scpi-lib Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)

install(TARGETS scpi-lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
