#include "KnxAddress.h"
#include "KnxConnectionListener.h"
#include "TunnelingConnection.h"
#include <asio/awaitable.hpp>
#include <asio/io_context.hpp>
#include <cstdint>
#include <memory>
#include <string_view>
#include <vector>

namespace connection {


/**
 * I'm responsible for keeping a connection to a KNX IP Router.
 *
 * I initiate all te nessasary ports, gather all the nessasary objects together
 * to keep a sort of connection household. A facade if you like.
 *
 * Note that I only support KNX ip tunneling for now.
 *
 * Refactor note: I wan't to constuct this object neeter with a builder of some
 * sort, to handle different configs and keep the option open to start a server
 * / IP router, KNX USB connection, KNX line etc.
 *
 */
class IpKnxConnection final : ConnectionListener {
public:
  IpKnxConnection(asio::io_context &ctx, std::string_view remoteIp,
                  std::uint16_t remotePort, std::string_view localBindIp,
                  std::uint16_t localDataPort = 2001,
                  std::uint16_t localControlPort = 2002);

  ~IpKnxConnection() override;

  /**
   * I will start up everything that is needed to maintain a connection with a
   * KNX IP Router.
   */
  asio::awaitable<void> start();

  void addListener(std::weak_ptr<KnxConnectionListener> listener);

  /**
   * I'll send out a message to the IP KNX Router to send the value to the KNX network
   */
  void writeToGroup(GroupAddress &ga, std::array<std::uint8_t,2>  value);

  /**
   * Get the data of the specified group address 
   */
  std::array<std::uint8_t,2> readGroup(GroupAddress &ga);

  /**
   * Does not comply to RAII, but need to figure this one out a bit. Get the
   * project going.
   */
  asio::awaitable<void> close();

private:


  void
  forEveryListener(std::function<auto(KnxConnectionListener *)->void> doThis);

  void onConnect() override;
  void onDisconnect() override;
  void onIncommingCemi(Cemi &cemi) override;
private:
  asio::io_context &ctx;
  std::unique_ptr<TunnelingConnection> physicalConnection;
  std::vector<std::weak_ptr<KnxConnectionListener>> connectionListeners{};
  friend class Listener;
};

} // namespace connection
