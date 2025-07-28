#include "Discovery.h"
#include "KnxAddress.h"
#include "KnxClientConnection.h"
#include "KnxConnectionFactory.h"
#include <asio/as_tuple.hpp>
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/steady_timer.hpp>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string_view>

class Logger final : public knx::connection::KnxConnectionListener {
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

std::unique_ptr<asio::steady_timer> closeTimer;
std::unique_ptr<asio::steady_timer> writeTimer;
std::unique_ptr<asio::signal_set> signals;

asio::awaitable<void> stopConnection(asio::io_context &ctx,
                                     knx::connection::KnxClientConnection &connection) {
  closeTimer = std::make_unique<asio::steady_timer>(ctx);
  closeTimer->expires_after(std::chrono::seconds(120));
  auto [errorcode] = co_await closeTimer->async_wait(asio::as_tuple);
  if (!errorcode) {
    co_await connection.close();
  }
  if(writeTimer) {
    writeTimer->cancel();
  }
  if(signals) {
    signals->cancel();
  }
}

asio::awaitable<void> writeGroup(asio::io_context &ctx,
                                 knx::connection::KnxClientConnection &connection) {
  writeTimer = std::make_unique<asio::steady_timer>(ctx);
  writeTimer->expires_after(std::chrono::seconds(3));
  auto [errorcode] = co_await writeTimer->async_wait(asio::as_tuple);
  if(!errorcode && connection.isOpen()) {
    GroupAddress ga{4, 0, 8};
    std::array<std::uint8_t, 2> data{0x00, 0x01};
    connection.writeToGroup(ga, data);
  }
}
asio::awaitable<void> onExit(knx::connection::KnxClientConnection &connection) {
  co_await (connection.close());
  if (closeTimer) {
    closeTimer->cancel();
  }
  if(writeTimer) {
    writeTimer->cancel();
  }
}

void logEvents(std::string_view routerIP, std::uint16_t routerPort,
               std::string_view bindIp) {
  asio::io_context io_context;
  knx::connection::KnxClientConnection connection =
      knx::connection::KnxConnectionFactory::createTunnelingClient(io_context, routerIP,
                                                        routerPort, bindIp);
  // Log to the standard out
  std::shared_ptr logger = std::make_shared<Logger>();
  connection.addListener(logger);
  asio::co_spawn(io_context, connection.start(), asio::detached);
  asio::co_spawn(io_context, writeGroup(io_context, connection),
                 asio::detached);
  // stop after 2 minutes
  asio::co_spawn(io_context, stopConnection(io_context, connection),
                 asio::detached);
  // on ctrl-c gracefully close connection
  signals = std::make_unique<asio::signal_set>(io_context, SIGINT, SIGTERM);
  signals->async_wait([&](auto, auto) {
    co_spawn(io_context, onExit(connection), asio::detached);
  });
  io_context.run();
}

int main(int argc, char *argv[]) {
  asio::io_context io_context;
  if (argc < 3) {
    knx::connection::Discovery discovery{io_context};
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
