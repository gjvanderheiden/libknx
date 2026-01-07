#include <iostream>
#include <knx/connection/Discovery.h>

int main(int argc, char *argv[]) {
  asio::io_context io_context;
  knx::connection::Discovery discovery{io_context};
  discovery.lookAround(1);
  io_context.run();
  for (auto router : discovery.result()) {
    std::cout << "Found router " << router.name << std::endl;
    std::cout << "with ip address " << router.ip << std::endl;
    std::cout << "at port " << router.port << std::endl;
  }
  return 0;
}
