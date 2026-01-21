#pragma once

#include "knx/bytes/ByteBufferReader.h"
#include "knx/ipreqresp/responses/AbstractResponse.h"
#include <cstdint>
#include <vector>

namespace knx::requestresponse {

class DisconnectResponse final : public AbstractResponse {
public:
  static constexpr std::uint16_t LENGTH = 8;
  static constexpr std::uint16_t SERVICE_ID = 0x020A;

public:
  DisconnectResponse()  = default;
  DisconnectResponse(const DisconnectResponse &) = default;
  DisconnectResponse(DisconnectResponse &&) = default;
  DisconnectResponse &operator=(const DisconnectResponse &) = default;
  DisconnectResponse &operator=(DisconnectResponse &&) = default;
  ~DisconnectResponse() override = default;

public:
  explicit DisconnectResponse(std::uint8_t channel, std::uint8_t status = 0);

  void write(ByteBufferWriter &writer) const override;
  static DisconnectResponse parse(ByteBufferReader& reader);
  constexpr std::uint16_t getServiceType() const override {return SERVICE_ID;}
  [[nodiscard]] std::vector<std::uint8_t> toBytes() const;

private:
  std::uint8_t channel;
  std::uint8_t status;
};

}
