#include "ConnectResponse.h"
#include "ConnectionRequestData.h"

namespace knx::requestresponse {

ConnectResponse ConnectResponse::parse(ByteBufferReader &buffer) {
  ConnectResponse response;
  response.channelId = buffer.readUint8();
  response.status = buffer.readUint8();
  response.dataEndPoint = HPAI::createAndParse(buffer);
  response.crd.parse(buffer);
  return response;
}

std::uint8_t ConnectResponse::getChannelId() const {
  return channelId;
}

std::uint8_t ConnectResponse::getStatus() const {
  return status;
}

} // namespace knx::requestresponse
