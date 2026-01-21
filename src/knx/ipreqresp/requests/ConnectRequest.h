#pragma once

#include "knx/ipreqresp/requests/AbstractRequest.h"
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

  static ConnectRequest parse(ByteBufferReader reader);
  void write(ByteBufferWriter &writer) const override;
  constexpr std::uint16_t getServiceType() const override {return SERVICE_ID;};

  bool matchesResponse(knx::requestresponse::ResponseVariant response) const override;
private:
  HPAI controlEndPoint;
  HPAI dataEndPoint;
  ConnectionRequestInformation cri;
};
