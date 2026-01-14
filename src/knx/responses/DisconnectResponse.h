#pragma once

#include "knx/bytes/ByteBufferReader.h"
#include <cstdint>
#include <vector>

namespace knx::requestresponse {

class DisconnectResponse {
public:
  static constexpr std::uint16_t LENGTH = 8;
  static constexpr std::uint16_t SERVICE_ID = 0x020A;

  explicit DisconnectResponse(std::uint8_t channel, std::uint8_t status = 0);

  static DisconnectResponse parse(ByteBufferReader& reader);
  [[nodiscard]] std::vector<std::uint8_t> toBytes() const;

private:
  std::uint8_t channel;
  std::uint8_t status;
};

}
