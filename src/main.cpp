#include "Discovery.h"
#include <asio/io_context.hpp>
#include <iostream>

int main(int argc, char *argv[]) {
  asio::io_context io_context;
  connection::Discovery discovery{io_context};
  discovery.lookAround(1);
  io_context.run();
  for (auto router : discovery.result()) {
    std::cout << "Found router " << router.name << std::endl;
    std::cout << "with ip address " << router.ip << std::endl;
  }
  return 0;
}
