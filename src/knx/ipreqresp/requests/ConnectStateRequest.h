#pragma once

#include "knx/headers/HPAI.h"

#include "knx/ipreqresp/requests/AbstractRequest.h"
#include <cstdint>

class ConnectStateRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0207;
  static constexpr std::uint16_t SIZE = 16;

public:
  ConnectStateRequest(const ConnectStateRequest &) = default;
  ConnectStateRequest(ConnectStateRequest &&) = default;
  ConnectStateRequest &operator=(const ConnectStateRequest &) = delete;
  ConnectStateRequest &operator=(ConnectStateRequest &&) = delete;

public:
  ConnectStateRequest(HPAI &&controlEndPoint, std::uint8_t channelId);
  ~ConnectStateRequest() override = default;
 
  static ConnectStateRequest parse(ByteBufferReader reader);
  void write(ByteBufferWriter &writer) const override;

  constexpr std::uint16_t getServiceType() const override {return SERVICE_ID;};
  bool matchesResponse(knx::requestresponse::ResponseVariant response) const override;
private:
  HPAI controlEndPoint;
  std::uint8_t channelId;
};
