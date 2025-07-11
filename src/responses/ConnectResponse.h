#pragma once

#include "ConnectionRequestData.h"
#include "HPAI.h"
#include "bytes/ByteBufferReader.h"

namespace knx::requestresponse {

class ConnectResponse {
public:
  static ConnectResponse parse(ByteBufferReader &buffer);

  static constexpr std::uint16_t SERVICE_ID = 0x0206;

private:
  std::uint8_t status;
  HPAI dataEndPoint;
  ConnectionRequestData crd;
};

} // namespace knx::requestresponse
