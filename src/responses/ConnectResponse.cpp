#include "ConnectResponse.h"
#include "ConnectionRequestData.h"

namespace knx::requestresponse {

ConnectResponse::ConnectResponse(std::uint8_t channelId, std::uint8_t status,
                                 HPAI &&dataEndPoint,
                                 ConnectionRequestData &&crd)
    : channelId{channelId}, status{status},
      dataEndPoint{std::move(dataEndPoint)}, crd{std::move(crd)} {}

ConnectResponse ConnectResponse::parse(ByteBufferReader &buffer) {
  return ConnectResponse{buffer.readUint8(), buffer.readUint8(),
                         HPAI::createAndParse(buffer),
                         ConnectionRequestData::createAndParse(buffer)};
}

std::uint8_t ConnectResponse::getChannelId() const { return channelId; }

std::uint8_t ConnectResponse::getStatus() const { return status; }

} // namespace knx::requestresponse
