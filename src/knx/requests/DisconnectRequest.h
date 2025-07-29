#pragma once

#include "knx/requests/AbstractRequest.h"
#include "knx/bytes/ByteBufferReader.h"
#include "knx/headers/HPAI.h"
#include <cstdint>
#include <vector>

class DisconnectRequest final : public AbstractRequest {
public:
  DisconnectRequest(std::uint8_t channel, HPAI&& hpai);
  static constexpr std::uint16_t SERVICE_ID = 0x0209;

  static DisconnectRequest parse(ByteBufferReader& reader);
  std::vector<std::uint8_t> toBytes() override;

  std::uint8_t getChannel() const;
  const HPAI& getControlEndpoint() const;

private:
  std::uint8_t channel{0};
  HPAI controlEndpoint;
};
