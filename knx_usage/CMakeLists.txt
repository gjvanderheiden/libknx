cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

project(Knx_Usage
  DESCRIPTION "How to use knx Library"
    LANGUAGES CXX)

find_package(knx REQUIRED)
pkg_check_modules(ASIO REQUIRED asio)

add_executable(knx-usage main.cpp )
target_link_libraries(knx-usage PUBLIC knx asio Threads::Threads)

