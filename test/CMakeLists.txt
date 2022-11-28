cmake_minimum_required(VERSION 3.5)

project(external_api_test LANGUAGES CXX)

enable_testing()

find_package(Threads REQUIRED)
find_package(QT NAMES Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
#FetchContent_MakeAvailable(googlemock)
add_library(GTest::GTest INTERFACE IMPORTED)
add_library(GMock::GMock INTERFACE IMPORTED)

include_directories(../scpi/scpi-lib/include)

target_link_libraries(GTest::GTest INTERFACE gtest_main)
target_link_libraries(GMock::GMock INTERFACE gmock_main)

add_executable(external_api_test main.cpp tst_external_api.cpp)

target_link_libraries(external_api_test PRIVATE Qt${QT_VERSION_MAJOR}::Test GTest::GTest GMock::GMock scpi-lib)

add_test(NAME external_api COMMAND external_api)
