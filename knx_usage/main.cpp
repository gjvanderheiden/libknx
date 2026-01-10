#include <iostream>
#include <knx/connection/Discovery.h>

int main() {
  asio::io_context io_context;
  knx::connection::Discovery discovery{io_context};
  discovery.lookAround(1);
  io_context.run();
  for (auto [name, ip, port] : discovery.result()) {
    std::cout << "Found router " << name << std::endl;
    std::cout << "with ip address " << ip << std::endl;
    std::cout << "at port " << port << std::endl;
  }
  return 0;
}
