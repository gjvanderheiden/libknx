cmake_minimum_required(VERSION 3.10)

project(KnxIP
    DESCRIPTION "Library to access KNX over IP"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(PkgConfig REQUIRED)
find_package(GTest REQUIRED)

pkg_check_modules(ASIO REQUIRED asio)

file(GLOB_RECURSE SOURCES  src/**/*.cpp)
file(GLOB_RECURSE TEST_SOURCES  test/**/*.cpp)

add_executable(knx-ip src/main.cpp ${SOURCES})
add_executable(knx-ip_tests ${SOURCES} ${TEST_SOURCES})

target_include_directories(knx-ip PUBLIC src src/ip src/headers src/requests src/connection)
target_include_directories(knx-ip_tests PRIVATE src src/ip src/headers src/requests src/connection test)

target_link_libraries(knx-ip)
target_link_libraries(knx-ip_tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(knx-ip_tests)
