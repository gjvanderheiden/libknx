#pragma once

#include "knx/bytes/ByteBufferReader.h"
#include "knx/headers/HPAI.h"
#include "knx/requests/AbstractRequest.h"
#include <vector>

class DisconnectRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0209;

public:
  DisconnectRequest(std::uint8_t channel, HPAI &&hpai);

  static DisconnectRequest parse(ByteBufferReader &reader);
  std::vector<std::uint8_t> toBytes() override;

  [[nodiscard]] std::uint8_t getChannel() const;
  [[nodiscard]] const HPAI &getControlEndpoint() const;

private:
  std::uint8_t channel{0};
  const HPAI controlEndpoint;
};
