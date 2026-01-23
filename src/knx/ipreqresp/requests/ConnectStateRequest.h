#pragma once

#include "knx/headers/HPAI.h"

#include "knx/ipreqresp/requests/AbstractRequest.h"
#include <cstdint>

class ConnectStateRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0207;
  static constexpr std::uint16_t SIZE = 16;

public:
  ConnectStateRequest() = delete;
  ConnectStateRequest(const ConnectStateRequest &) = default;
  ConnectStateRequest(ConnectStateRequest &&) noexcept = default;
  ConnectStateRequest &operator=(const ConnectStateRequest &) = delete;
  ConnectStateRequest &operator=(ConnectStateRequest &&) noexcept = delete;
  ~ConnectStateRequest() override = default;

public:
  ConnectStateRequest(HPAI &&controlEndPoint, std::uint8_t channelId);
 
  static ConnectStateRequest parse(ByteBufferReader reader);
  void write(ByteBufferWriter &writer) const override;

  [[nodiscard]] constexpr std::uint16_t getServiceType() const override {return SERVICE_ID;};
  [[nodiscard]] bool matchesResponse(knx::requestresponse::ResponseVariant response) const override;
  [[nodiscard]] std::string getShortDescription() const override;
private:
  HPAI controlEndPoint;
  std::uint8_t channelId;
};
