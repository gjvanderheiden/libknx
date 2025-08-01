#pragma once

#include "knx/bytes/ByteBufferReader.h"
#include "knx/headers/ConnectionRequestInformation.h"
#include "knx/headers/HPAI.h"
#include "knx/connection/ConnectionListener.h"
#include "knx/headers/KnxIpHeader.h"
#include "knx/ip/UdpSocket.h"
#include <asio/awaitable.hpp>
#include <asio/io_context.hpp>
#include <cstdint>
#include <string_view>
#include <vector>

namespace knx::connection {

/**
 * I'm responsible for keeping a connection to a KNX IP Router.
 *
 * I deal with the KnxIpHeaders being received and send.
 *
 * Once in a while I'll send a ping to the server to see if we still have a good connection.
 * If can disconnect if:
 *  - a disconnect request is received
 *  - a timeout on a acknowlegde of a message expired
 *  - a error or wrong packet it received
 *  - manual close() is send.
 *
 * I initiate all te nessasary ports, gather all the nessasary objects together
 * to keep a sort of connection household. A facade if you like.
 *
 */
class TunnelingConnection {

public:
  TunnelingConnection(asio::io_context &ctx, std::string_view remoteIp,
                  std::uint16_t remotePort, std::string_view localBindIp,
                  std::uint16_t localDataPort,
                  std::uint16_t localControlPort);

  ~TunnelingConnection();

  /**
   * I will start up everything that is needed to maintain a connection with a
   * KNX IP Router.
   */
  asio::awaitable<void> start();

  void addListener(ConnectionListener& listener);
  void send(Cemi&& cemi);

  /**
   * Does not comply to RAII, but need to figure this one out a bit. Get the
   * project going.
   */
  asio::awaitable<void> close();

private:
  asio::awaitable<void> close(bool needsDisconnectRequest);

private:
  using CallBackFunction = std::function<
      auto(KnxIpHeader &knxIpHeader, ByteBufferReader &reader)->bool>;
  asio::awaitable<void> sendRequest(std::vector<std::uint8_t> &request,
                                    std::uint16_t responseServiceId,
                                    CallBackFunction &&callBackFunction);
  /**
   * periodically, the connection needs to be checked according to KNX IP
   * protocol. The server closes the connection if it get this sort of ping.
   */
  asio::awaitable<void> checkConnection();

  auto onReceiveData(std::vector<std::uint8_t>&& data) -> void;

  auto onReceiveTunnelRequest(KnxIpHeader &knxIpHeader,
                              ByteBufferReader &reader) -> bool;
  auto onReceiveDisconnectRequest(KnxIpHeader &knxIpHeader,
                                  ByteBufferReader &reader) -> bool;

  ConnectionRequestInformation createConnectRequestInformation();
  HPAI createDataHPAI();
  HPAI createControlHPAI();

  void
  forEveryListener(std::function<auto(ConnectionListener *)->void> doThis);

private:
  asio::io_context &ctx;
  asio::steady_timer checkConnectionTimer;
  asio::ip::udp::endpoint remoteControlEndPoint;
  asio::ip::udp::endpoint remoteDataEndPoint;
  asio::ip::address_v4 localBindIp;
  const std::uint16_t dataPort;
  const std::uint16_t controlPort;
  std::unique_ptr<udp::UdpSocket> dataSocket;
  std::unique_ptr<udp::UdpSocket> controlSocket;
  std::unordered_map<std::uint16_t, CallBackFunction> listeners{};
  std::uint8_t channelId{0};
  std::vector<ConnectionListener*> connectionListeners{};
  bool closingDown{false};
};

} // namespace connection
