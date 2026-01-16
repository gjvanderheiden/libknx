#pragma once

#include "knx/headers/ConnectionHeader.h"
#include "knx/bytes/ByteBufferReader.h"
#include "knx/responses/AbstractResponse.h"

namespace knx::requestresponse {

class TunnelAckResponse final : public AbstractResponse {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0421;
  static constexpr std::uint16_t LENGTH = 10;

public:
  TunnelAckResponse(const TunnelAckResponse &) = default;
  TunnelAckResponse(TunnelAckResponse &&) = delete;
  TunnelAckResponse &operator=(const TunnelAckResponse &) = delete;
  TunnelAckResponse &operator=(TunnelAckResponse &&) = delete;
  ~TunnelAckResponse() override = default;

public:
  explicit TunnelAckResponse(ConnectionHeader &&connectionHeader);

  void write(ByteBufferWriter &writer) override;
  static TunnelAckResponse parse(ByteBufferReader& reader);
  [[nodiscard]] std::vector<byte> toBytes() const;

  [[nodiscard]] const ConnectionHeader& getConnectionHeader() const;

private:
  ConnectionHeader connectionHeader;
};

}
