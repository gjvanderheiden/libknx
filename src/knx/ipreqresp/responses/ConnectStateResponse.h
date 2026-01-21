#pragma once

#include "knx/bytes/ByteBufferReader.h"
#include "knx/ipreqresp/responses/AbstractResponse.h"
#include <cstdint>

namespace knx::requestresponse {

class ConnectStateResponse final : public AbstractResponse {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0208;

public:
  ConnectStateResponse() = delete;
  ConnectStateResponse(const ConnectStateResponse &) = default;
  ConnectStateResponse(ConnectStateResponse &&) noexcept = default;
  ConnectStateResponse &operator=(const ConnectStateResponse &) = default;
  ConnectStateResponse &operator=(ConnectStateResponse &&) noexcept = default;
  ~ConnectStateResponse() override = default;

public:
  ConnectStateResponse(std::uint8_t channelId, std::uint8_t status);

  static ConnectStateResponse parse(ByteBufferReader &buffer);
  void write(ByteBufferWriter &writer) const override;

  [[nodiscard]] constexpr std::uint16_t getServiceType() const override {return SERVICE_ID;}

  [[nodiscard]] std::uint8_t getChannelId() const;
  [[nodiscard]] std::uint8_t getStatus() const;

private:
  std::uint8_t channelId;
  std::uint8_t status;
};

} // namespace knx::requestresponse
