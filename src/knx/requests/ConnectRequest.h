#pragma once

#include "knx/requests/AbstractRequest.h"
#include "knx/headers/ConnectionRequestInformation.h"
#include "knx/headers/HPAI.h"
#include "knx/bytes/ByteBufferReader.h"

class ConnectRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0205;
  static constexpr std::uint16_t SIZE = 26;

public:
  ConnectRequest() = delete;
  ConnectRequest(const ConnectRequest &) = default;
  ConnectRequest(ConnectRequest &&) = default;

  ConnectRequest &operator=(const ConnectRequest &) = delete;
  ConnectRequest &operator=(ConnectRequest &&) = delete;

  ~ConnectRequest() override = default;
public:
  explicit ConnectRequest(HPAI &&controlEndPoint, HPAI &&dataEndPoint,
                          ConnectionRequestInformation &&cri);

  void write(ByteBufferWriter &writer) override;
  static ConnectRequest parse(ByteBufferReader reader);

private:
  HPAI controlEndPoint;
  HPAI dataEndPoint;
  ConnectionRequestInformation cri;
};
