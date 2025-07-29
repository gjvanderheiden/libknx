find_package(knx REQUIRED)
pkg_check_modules(ASIO REQUIRED asio)

add_executable(knx-usage main.cpp )
target_link_libraries(knx-usage PUBLIC knx::knx Threads::Threads)
