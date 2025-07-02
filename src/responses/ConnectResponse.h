#pragma once

#include "ConnectionRequestData.h"
#include "HPAI.h"
#include "bytes/ByteBuffer.h"

namespace knx::requestresponse {

class ConnectResponse {
public:
  static ConnectResponse parse(ByteBuffer &buffer);

  static constexpr std::array<std::uint8_t, 2> SERVICE_ID = {0x02, 0x06};

private:
  std::uint8_t status;
  HPAI dataEndPoint;
  ConnectionRequestData crd;
};

} // namespace knx::requestresponse
