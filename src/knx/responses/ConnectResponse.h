#pragma once

#include "knx/headers/ConnectionRequestData.h"
#include "knx/headers/HPAI.h"
#include "knx/bytes/ByteBufferReader.h"
#include <cstdint>

namespace knx::requestresponse {

class ConnectResponse {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0206;


  static constexpr std::uint8_t NO_ERROR = 0x00;
  static constexpr std::uint8_t CONNECTION_TYPE_ERROR = 0x22;
  static constexpr std::uint8_t CONNECTION_OPTION_ERROR = 0x23;
  static constexpr std::uint8_t NO_MORE_CONNECTIONS_ERROR = 0x24;
public:
  ConnectResponse(std::uint8_t channelId, std::uint8_t status, HPAI&& dataEndPoint, ConnectionRequestData&& crd);
  static ConnectResponse parse(ByteBufferReader &buffer);


  std::uint8_t getChannelId() const;
  std::uint8_t getStatus() const;
  const ConnectionRequestData& getCRD() const;
  const HPAI& getDataEndPoint() const;

private:
  const std::uint8_t channelId;
  const std::uint8_t status;
  const HPAI dataEndPoint;
  const ConnectionRequestData crd;
};

} // namespace knx::requestresponse
