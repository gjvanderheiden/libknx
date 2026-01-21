#pragma once

#include "knx/bytes/ByteBufferReader.h"
#include "knx/headers/HPAI.h"
#include "knx/ipreqresp/requests/AbstractRequest.h"

class DisconnectRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0209;
  static constexpr std::uint16_t SIZE = 16;

public:
  DisconnectRequest() = delete;
  DisconnectRequest(const DisconnectRequest &) = default;
  DisconnectRequest(DisconnectRequest &&) = default;
  DisconnectRequest &operator=(const DisconnectRequest &) = delete;
  DisconnectRequest &operator=(DisconnectRequest &&) = delete;
  ~DisconnectRequest() override = default;

public:
  DisconnectRequest(std::uint8_t channel, HPAI &&hpai);

  static DisconnectRequest parse(ByteBufferReader &reader);
  void write(ByteBufferWriter &writer) const override;

  constexpr std::uint16_t getServiceType() const override {return SERVICE_ID;};
  bool matchesResponse(knx::requestresponse::ResponseVariant response) const override;

  [[nodiscard]] std::uint8_t getChannel() const;
  [[nodiscard]] const HPAI &getControlEndpoint() const;

private:
  std::uint8_t channel{0};
  HPAI controlEndpoint;
};
