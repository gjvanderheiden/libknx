#include "Discovery.h"
#include "IpKnxConnection.h"
#include "KnxAddress.h"
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <iostream>
#include <memory>

std::ostream& operator <<(std::ostream& stream, const GroupAddress& address) {
  stream << (int)address.getHigh() << "/"  << (int)address.getMiddle() << "/" << (int)address.getLow();
  return stream;
}
std::ostream& operator <<(std::ostream& stream, const IndividualAddress& address) {
  stream << (int)address.getHigh() << "."  << (int)address.getMiddle() << "." << (int) address.getLow();
  return stream;
}

class Logger final : public connection::KnxConnectionListener  {
  public:
  Logger() = default;
  ~Logger() override = default;

  void onConnect() override{
    std::cout << "Connected\n";
  }
  void onDisconnect() override {
    std::cout << "Disconnected\n";
  } 

  void onGroupRead(const IndividualAddress& source, const GroupAddress& ga) override {
    std::cout << "Group Read from: " << source << ", for: " << ga << "\n";
  };

  void onGroupReadResponse(const IndividualAddress& source, const GroupAddress& ga, const std::array<byte, 2> data) override{
    std::cout << "Group read response, from " << source <<", for: " << ga << "\n";
  }


  void onGroupWrite(const IndividualAddress& source, const GroupAddress& ga, const std::array<byte, 2> data) override{
    std::cout << "Group write  from " << source <<", for: " << ga << "\n";
  }

};


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
      std::shared_ptr logger = std::make_shared<Logger>();
      connection.addListener(logger);
      asio::co_spawn(io_context, connection.start(), asio::detached);
      io_context.run();
    }
  }
  return 0;
}
