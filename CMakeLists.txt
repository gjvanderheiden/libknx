cmake_minimum_required(VERSION 4.0)

set(CMAKE_CXX_STANDARD 26)

project(KnxIP
    DESCRIPTION "Library to access KNX over IP"
    VERSION 0.1
    LANGUAGES CXX)

enable_testing()
include(FindThreads)
find_package(PkgConfig REQUIRED)
find_package(GTest REQUIRED)

pkg_check_modules(ASIO REQUIRED asio)

file(GLOB_RECURSE knx_source_files   src/**/*.cpp)
file(GLOB_RECURSE knx_header_files   src/**/*.h*)
file(GLOB_RECURSE test_source_files  test/**/*.cpp)

add_library(knx SHARED)
add_library(knx::knx ALIAS knx)
add_executable(knx-ip_tests ${test_source_files})
set_target_properties(knx PROPERTIES 
    VERIFY_INTERFACE_HEADER_SETS ON 
    VERSION ${PROJECT_VERSION})

target_include_directories(knx PUBLIC ${ASIO_INCLUDE_DIRS}) 

target_sources(knx
    PUBLIC
      FILE_SET knx_headers
      TYPE HEADERS
      BASE_DIRS src
      FILES ${knx_header_files}
    PRIVATE
      ${knx_source_files}
     )
target_link_libraries(knx-ip_tests PRIVATE knx GTest::gtest_main Threads::Threads)

include(GoogleTest)
gtest_discover_tests(knx-ip_tests)

install(
  TARGETS knx
  EXPORT knxTargets
  FILE_SET knx_headers)

install (
  EXPORT knxTargets
  DESTINATION share/cmake/knx
  NAMESPACE knx::
)

install (
  FILES cmake/knxConfig.cmake
  DESTINATION share/cmake/knx
)
