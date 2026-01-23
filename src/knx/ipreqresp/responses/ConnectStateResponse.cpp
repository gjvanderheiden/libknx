#include "knx/ipreqresp/responses/ConnectStateResponse.h"

namespace knx::requestresponse {

ConnectStateResponse::ConnectStateResponse(const std::uint8_t channelId,
                                           const std::uint8_t status)
    : channelId{channelId}, status{status} {}

ConnectStateResponse ConnectStateResponse::parse(ByteBufferReader &buffer) {
  return {buffer.readUint8(), buffer.readUint8()};
}

void ConnectStateResponse::write(ByteBufferWriter &writer) const {
  writer.writeUint8(channelId);
  writer.writeUint8(status);
}  

std::uint8_t ConnectStateResponse::getChannelId() const {
  return channelId;
}
std::uint8_t ConnectStateResponse::getStatus() const {
  return status;
}

std::string ConnectStateResponse::getShortDescription() const {
  return "ConnectStateResponse";
}
} // namespace knx::requestresponse
