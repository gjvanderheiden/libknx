cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

project(KnxIP
    DESCRIPTION "Library to access KNX over IP"
    VERSION 0.1
    LANGUAGES CXX)

include(FindThreads)
find_package(PkgConfig REQUIRED)
find_package(GTest REQUIRED)

pkg_check_modules(ASIO REQUIRED asio)

file(GLOB_RECURSE SOURCES  src/**/*.cpp)
file(GLOB_RECURSE TEST_SOURCES  test/**/*.cpp)

add_library(knx SHARED ${SOURCES})
add_executable(knx-ip src/main.cpp )
add_executable(knx-ip_tests ${TEST_SOURCES})
set_target_properties(knx PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(knx PUBLIC src src/bytes src/ip src/headers src/requests src/connection ${ASIO_INCLUDE_DIRS})
target_include_directories(knx-ip_tests PRIVATE src src/bytes src/ip src/headers src/requests src/connection test)

target_link_libraries(knx-ip PUBLIC knx Threads::Threads)
target_link_libraries(knx-ip_tests PRIVATE knx GTest::gtest_main Threads::Threads)

include(GoogleTest)
gtest_discover_tests(knx-ip_tests)
