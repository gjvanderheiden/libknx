#include "ConnectionRequestInformation.h"
#include "KnxConnectionListener.h"
#include "HPAI.h"
#include "KnxAddress.h"
#include "UdpSocket.h"
#include <asio/awaitable.hpp>
#include <asio/io_context.hpp>
#include <cstdint>
#include <memory>
#include <vector>
#include <string_view>
#include "ByteBufferReader.h"
#include "KnxIpHeader.h"

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
                  std::uint16_t remotePort,
                  std::string_view localBindIp,
                  std::uint16_t localDataPort = 2001,
                  std::uint16_t localControlPort = 2002);

  /**
   * I will start up everything that is needed to maintain a connection with a
   * KNX IP Router.
   */
  asio::awaitable<void>  start();

  void addListener(std::weak_ptr<KnxConnectionListener> listener);

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
  using CallBackFunction = std::function<auto (KnxIpHeader& knxIpHeader, ByteBufferReader& reader) -> bool>;
  asio::awaitable<void> sendRequest(std::vector<std::uint8_t>& request, std::uint16_t responseServiceId, CallBackFunction&& callBackFunction);
  /**
   * periodically, the connection needs to be checked according to KNX IP
   * protocol. The server closes the connection if it get this sort of ping.
   */
  asio::awaitable<void> checkConnection();

  auto onReceiveData(std::vector<std::uint8_t> &data) -> void;

  auto onReceiveTunnelRequest(KnxIpHeader& knxIpHeader, ByteBufferReader& reader) -> bool;
  auto onReceiveDisconnectRequest(KnxIpHeader& knxIpHeader, ByteBufferReader& reader) -> bool;

  ConnectionRequestInformation createConnectRequestInformation();
  HPAI createDataHPAI();
  HPAI createControlHPAI();

  void forEveryListener(std::function<auto(KnxConnectionListener*)->void> doThis);

private:
  asio::io_context &ctx;
  asio::ip::udp::endpoint remoteControlEndPoint;
  asio::ip::address_v4 localBindIp;
  const std::uint16_t dataPort;
  const std::uint16_t controlPort;
  udp::UdpSocket dataSocket;
  udp::UdpSocket controlSocket;
  std::unordered_map<std::uint16_t, CallBackFunction> listeners{};
  std::uint8_t channelId{0};
  std::vector<std::weak_ptr<KnxConnectionListener>> connectionListeners{};
};

} // namespace connection
