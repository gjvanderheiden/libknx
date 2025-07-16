#include "Discovery.h"
#include "IpKnxConnection.h"
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
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
    if (argc == 2) {
      asio::io_context io_context;
      connection::IpKnxConnection connection{io_context, router.ip, router.port,
                                             argv[1]};
      asio::co_spawn(io_context, connection.start(), asio::detached);
      io_context.run();
    }
  }
  return 0;
}
