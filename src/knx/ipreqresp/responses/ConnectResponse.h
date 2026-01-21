#pragma once

#include "knx/headers/ConnectionRequestData.h"
#include "knx/headers/HPAI.h"
#include "knx/bytes/ByteBufferReader.h"
#include "knx/ipreqresp/responses/AbstractResponse.h"
#include <cstdint>

namespace knx::requestresponse {

class ConnectResponse final : public AbstractResponse {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0206;

  static constexpr std::uint8_t NO_ERROR = 0x00;
  static constexpr std::uint8_t CONNECTION_TYPE_ERROR = 0x22;
  static constexpr std::uint8_t CONNECTION_OPTION_ERROR = 0x23;
  static constexpr std::uint8_t NO_MORE_CONNECTIONS_ERROR = 0x24;

public:
  ConnectResponse() = delete;
  ConnectResponse(const ConnectResponse &) = default;
  ConnectResponse(ConnectResponse &&) noexcept = default;
  ConnectResponse &operator=(const ConnectResponse &) = default;
  ConnectResponse &operator=(ConnectResponse &&) noexcept = default;
  ~ConnectResponse() override = default;

public:
  ConnectResponse(std::uint8_t channelId, std::uint8_t status,
                  HPAI &&dataEndPoint, ConnectionRequestData &&crd);

  static ConnectResponse parse(ByteBufferReader &buffer);
  void write(ByteBufferWriter &writer) const override;


  [[nodiscard]] constexpr std::uint16_t getServiceType() const override {return SERVICE_ID;}

  [[nodiscard]] std::uint8_t getChannelId() const;
  [[nodiscard]] std::uint8_t getStatus() const;
  [[nodiscard]] const ConnectionRequestData& getCRD() const;
  [[nodiscard]] const HPAI& getDataEndPoint() const;

private:
  std::uint8_t channelId{0};
  std::uint8_t status{0};
  HPAI dataEndPoint;
  ConnectionRequestData crd;
};

} // namespace knx::requestresponse
