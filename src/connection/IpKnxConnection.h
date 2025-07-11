#include "KnxAddress.h"
#include "UdpSocket.h"
#include <asio/io_context.hpp>
#include <cstdint>
#include <string_view>

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
class IpKnxConnection {

public:
  IpKnxConnection(asio::io_context &ctx, std::string_view remoteIp,
                  std::uint16_t remotePort, std::uint16_t localDataPort = 2001,
                  std::uint16_t localControlPort = 2002);

  /**
   * I will start up everything that is needed to maintain a connection with a
   * KNx IP Router.
   */
  void start();

  /**
   * Just like the getter: A test method to get started: Bool value instead of a
   * KNX type. Still playing around with this.
   */
  void setGroupData(GroupAddress &ga, bool value);

  /**
   * Just like the setter: A test method to get started.
   */
  bool getGroupData(GroupAddress &ga);

  /**
   * Does not comply to RAII, but need to figure this one out a bit. Get the
   * project going.
   */
  void close();

private:
  /**
   * periodically, the connection needs to be checked according to KNX IP
   * protocol. The server closes the connection if it get this sort of ping.
   */
  void checkConnection();

  auto onReceiveData(std::vector<std::uint8_t>& data) -> void;

private:
  asio::io_context &ctx;
  std::uint16_t dataPort;
  asio::ip::udp::endpoint remoteControlEndPoint;
  std::uint16_t controlPort;
  udp::UdpSocket dataSocket;
  udp::UdpSocket controlSocket;
};

} // namespace connection
