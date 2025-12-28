cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 26)

project(KnxUsage
    DESCRIPTION "Library to access KNX over IP"
    LANGUAGES CXX)

include(FindThreads)
find_package(PkgConfig        REQUIRED)
find_package(knx REQUIRED)
pkg_check_modules(ASIO REQUIRED asio)

add_executable(knx-usage main.cpp )
target_link_libraries(knx-usage PUBLIC knx::knx Threads::Threads)
