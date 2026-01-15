#pragma once

#include "knx/bytes/ByteBufferReader.h"
#include "knx/connection/ConnectionListener.h"
#include "knx/connection/SendTunnelingState.h"
#include "knx/headers/HPAI.h"
#include "knx/headers/KnxIpHeader.h"
#include "knx/ip/UdpSocket.h"
#include "knx/requests/AbstractRequest.h"
#include <asio/awaitable.hpp>
#include <asio/io_context.hpp>
#include <map>
#include <string_view>
#include <vector>

namespace knx::connection {

/**
 * I'm responsible for keeping a connection to a KNX IP Router.
 *
 * I deal with the KnxIpHeaders being received and send.
 *
 * Once in a while I'll send a ping to the server to see if we still have a good
 * connection. If can disconnect if:
 *  - a disconnect request is received
 *  - a timeout on a acknowlegde of a message expired
 *  - a error or wrong packet it received
 *  - manual close() is send.
 *
 * I initiate all te nessasary ports, gather all the nessasary objects together
 * to keep a sort of connection household. A facade if you like.
 */
class TunnelingConnection {
public:
  static constexpr int TIME_BETWEEN_CHECK_CONNECTION = 45;

public:
  TunnelingConnection(const TunnelingConnection &) = delete;
  TunnelingConnection(TunnelingConnection &&) = delete;
  TunnelingConnection &operator=(const TunnelingConnection &) = delete;
  TunnelingConnection &operator=(TunnelingConnection &&) = delete;

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

  void addListener(ConnectionListener &listener);
  asio::awaitable<void> send(Cemi &&cemi);

  /**
   * Does not comply to RAII, but need to figure this one out a bit. Get the
   * project going.
   */
  asio::awaitable<void> close();

  asio::awaitable<void> printDescription();

  const IndividualAddress &getKnxAddress();

private:
  asio::awaitable<void> close(bool needsDisconnectRequest);

  void onControlSocketClosed();
  void onDataSocketClosed();
  void reset();

private:
  using CallBackFunction = std::function<
      auto(KnxIpHeader &knxIpHeader, ByteBufferReader &reader)->bool>;

  /**
   * send request to KNX server (router).
   * This should also do a timeout, which is currently doesn't do.
   */
  asio::awaitable<void> sendRequest(AbstractRequest &request,
                                    std::uint16_t responseServiceId,
                                    CallBackFunction &&callBackFunction);
  /**
   * periodically, the connection needs to be checked according to KNX IP
   * protocol. The server closes the connection if it get this sort of ping.
   */
  asio::awaitable<void> checkConnection();

  /**
   * Raw data from the UdpSocket. I will parse it and handle it
   */
  auto onReceiveData(std::vector<std::uint8_t> &&data) -> void;

  /**
   * Tunneling connection releated handler: a tunnel request
   * This is called from onReceiveData (first registered as a listener)
   * send an ACK back and check the cemi, inform listeners
   */
  auto onReceiveTunnelRequest(KnxIpHeader &knxIpHeader,
                              ByteBufferReader &reader) -> bool;

  /**
   * Tunneling connection releated handler: disconnect request
   * This is called from onReceiveData (first registered as a listener)
   * Server want to kick us out: reset() and inform listeners
   */
  auto onReceiveDisconnectRequest(KnxIpHeader &knxIpHeader,
                                  ByteBufferReader &reader) -> bool;

  /**
   * Tunneling connection releated handler: a ack tunnel response
   * This is called from onReceiveData (first registered as a listener)
   * Server has received our tunneling packet.
   * Check if it was send by me and inform send item handler
   */
  auto onReceiveAckTunnelResponse(KnxIpHeader &knxIpHeader,
                                  ByteBufferReader &reader) -> bool;

  HPAI createDataHPAI() const;
  HPAI createControlHPAI() const;

  /**
   * inform every ConnectionListener with whatever is in the lambda
   */
  void forEveryListener(
      const std::function<auto(ConnectionListener *)->void> &doThis) const;

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
  std::vector<ConnectionListener *> connectionListeners{};
  bool closingDown{false};
  std::uint8_t sequence{0};
  std::map<std::uint8_t, std::unique_ptr<SendTunnelingState>> sendItems{};
  IndividualAddress knxAddress;
};

} // namespace knx::connection
