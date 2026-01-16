#pragma once

#include "knx/bytes/ByteBufferReader.h"
#include "knx/responses/AbstractResponse.h"
#include <cstdint>

namespace knx::requestresponse {

class ConnectStateResponse final : public AbstractResponse {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0208;

public:
  ConnectStateResponse(const ConnectStateResponse &) = default;
  ConnectStateResponse(ConnectStateResponse &&) = delete;
  ConnectStateResponse &operator=(const ConnectStateResponse &) = default;
  ConnectStateResponse &operator=(ConnectStateResponse &&) = delete;
  ~ConnectStateResponse() override = default;

public:
  ConnectStateResponse(std::uint8_t channelId, std::uint8_t status);

  static ConnectStateResponse parse(ByteBufferReader &buffer);
  void write(ByteBufferWriter &writer) override;

  [[nodiscard]] std::uint8_t getChannelId() const;
  [[nodiscard]] std::uint8_t getStatus() const;

private:
  std::uint8_t channelId;
  std::uint8_t status;
};

} // namespace knx::requestresponse
