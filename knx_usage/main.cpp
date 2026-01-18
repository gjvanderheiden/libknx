#include <iostream>
#include <knx/connection/Discovery.h>

int main() {
  knx::connection::Discovery discovery{};
  discovery.lookAround([] (knx::connection::KnxIp& knxip){
    std::cout << "Found router " << knxip.name << "\n";
    std::cout << "with ip address: " << knxip.ip << " and port: " << knxip.port << "\n";
  });
  return 0;
}
