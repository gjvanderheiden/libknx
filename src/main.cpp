#include "Discovery.h"
#include "IpKnxConnection.h"
#include "KnxAddress.h"
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string_view>

std::ostream &operator<<(std::ostream &stream, const GroupAddress &address) {
  stream << (int)address.getHigh() << "/" << (int)address.getMiddle() << "/"
         << (int)address.getLow();
  return stream;
}
std::ostream &operator<<(std::ostream &stream,
                         const IndividualAddress &address) {
  stream << (int)address.getHigh() << "." << (int)address.getMiddle() << "."
         << (int)address.getLow();
  return stream;
}

class StopContextOnDisconnect final : public connection::KnxConnectionListener {
public:
  StopContextOnDisconnect(asio::io_context &ctx) : ctx{ctx} {};
  ~StopContextOnDisconnect() override = default;

  void onConnect() override {}
  void onDisconnect() override {
    ;
    ctx.stop();
  }

  void onGroupRead(const IndividualAddress &source,
                   const GroupAddress &ga) override {}

  void onGroupReadResponse(const IndividualAddress &source,
                           const GroupAddress &ga,
                           const std::array<byte, 2> data) override {}

  void onGroupWrite(const IndividualAddress &source, const GroupAddress &ga,
                    const std::array<byte, 2> data) override {}

private:
  asio::io_context &ctx;
};

class Logger final : public connection::KnxConnectionListener {
public:
  Logger() = default;
  ~Logger() override = default;

  void onConnect() override { std::cout << "Connected\n"; }
  void onDisconnect() override { std::cout << "Disconnected\n"; }

  void onGroupRead(const IndividualAddress &source,
                   const GroupAddress &ga) override {
    std::cout << "Group Read from: " << source << ", for: " << ga << "\n";
  };

  void onGroupReadResponse(const IndividualAddress &source,
                           const GroupAddress &ga,
                           const std::array<byte, 2> data) override {
    std::cout << "Group read response, from " << source << ", for: " << ga
              << "\n";
  }

  void onGroupWrite(const IndividualAddress &source, const GroupAddress &ga,
                    const std::array<byte, 2> data) override {
    std::cout << "Group write  from " << source << ", for: " << ga << "\n";
  }
};

asio::awaitable<void> stopConnection(asio::io_context &ctx,
                                     connection::IpKnxConnection &connection) {
  asio::steady_timer timer(ctx);
  timer.expires_after(std::chrono::seconds(120));
  co_await timer.async_wait();
  co_await connection.close();
}

void logEvents(std::string_view routerIP, std::uint16_t routerPort,
               std::string_view bindIp) {
  asio::io_context io_context;
  connection::IpKnxConnection connection{io_context, routerIP, routerPort,
                                         bindIp};
  std::shared_ptr logger = std::make_shared<Logger>();
  connection.addListener(logger);
  std::shared_ptr stopCtxOnDisconnect =
      std::make_shared<StopContextOnDisconnect>(io_context);
  connection.addListener(stopCtxOnDisconnect);
  asio::co_spawn(io_context, connection.start(), asio::detached);
  asio::co_spawn(io_context, stopConnection(io_context, connection),
                 asio::detached);
  asio::signal_set signals(io_context, SIGINT, SIGTERM);
  signals.async_wait([&](auto, auto) { co_spawn(io_context, connection.close(), asio::detached);
  });
  io_context.run();
}

int main(int argc, char *argv[]) {
  asio::io_context io_context;
  if (argc < 3) {
    connection::Discovery discovery{io_context};
    discovery.lookAround(1);
    io_context.run();
    for (auto router : discovery.result()) {
      std::cout << "Found router " << router.name << std::endl;
      std::cout << "with ip address " << router.ip << std::endl;
      if (argc >= 2) {
        logEvents(router.ip, router.port, argv[1]);
      }
    }
  } else {
    logEvents(argv[2], 3671, argv[1]);
  }
  return 0;
}
