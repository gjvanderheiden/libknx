#pragma once

#include "knx/headers/HPAI.h"

#include "AbstractRequest.h"
#include <cstdint>

class ConnectStateRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0207;
  static constexpr std::uint16_t SIZE = 16;

public:
  ConnectStateRequest(HPAI &&controlEndPoint, std::uint8_t channelId);
  ~ConnectStateRequest() override = default;
 
  static ConnectStateRequest parse(ByteBufferReader reader);
  void write(ByteBufferWriter &writer) override;

private:
  HPAI controlEndPoint;
  std::uint8_t channelId;
};
