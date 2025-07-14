#include "ConnectStateResponse.h"

namespace knx::requestresponse {

ConnectStateResponse::ConnectStateResponse(std::uint8_t channelId,
                                           std::uint8_t status)
    : channelId{channelId}, status{status} {}

ConnectStateResponse ConnectStateResponse::parse(ByteBufferReader &buffer) {
  return {buffer.readUint8(), buffer.readUint8()};
}

std::uint8_t ConnectStateResponse::getChannelId() const {
  return channelId;
}
std::uint8_t ConnectStateResponse::getStatus() const {
  return status;
}

} // namespace knx::requestresponse
