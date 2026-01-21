#pragma once

#include "knx/headers/ConnectionHeader.h"
#include "knx/bytes/ByteBufferReader.h"
#include "knx/ipreqresp/responses/AbstractResponse.h"

namespace knx::requestresponse {

class TunnelAckResponse final : public AbstractResponse {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0421;
  static constexpr std::uint16_t LENGTH = 10;

public:
  TunnelAckResponse() = default;
  TunnelAckResponse(const TunnelAckResponse &) = default;
  TunnelAckResponse(TunnelAckResponse &&) = default;
  TunnelAckResponse &operator=(const TunnelAckResponse &) = default;
  TunnelAckResponse &operator=(TunnelAckResponse &&) = default;
  ~TunnelAckResponse() override = default;

public:
  explicit TunnelAckResponse(ConnectionHeader &&connectionHeader);

  void write(ByteBufferWriter &writer) const override;
  static TunnelAckResponse parse(ByteBufferReader& reader);
  [[nodiscard]] std::vector<byte> toBytes() const;
  constexpr std::uint16_t getServiceType() const override {return SERVICE_ID;}

  [[nodiscard]] const ConnectionHeader& getConnectionHeader() const;

private:
  ConnectionHeader connectionHeader;
};

}
