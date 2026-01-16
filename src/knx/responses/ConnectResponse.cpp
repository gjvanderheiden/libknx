#include "knx/responses/ConnectResponse.h"
#include "knx/headers/ConnectionRequestData.h"

namespace knx::requestresponse {

ConnectResponse::ConnectResponse(const std::uint8_t channelId, const std::uint8_t status,
                                 HPAI &&dataEndPoint,
                                 ConnectionRequestData &&crd)
    : channelId{channelId}, status{status},
      dataEndPoint{std::move(dataEndPoint)}, crd{std::move(crd)} {}

ConnectResponse ConnectResponse::parse(ByteBufferReader &buffer) {
  return ConnectResponse{buffer.readUint8(), buffer.readUint8(),
                         HPAI::parse(buffer),
                         ConnectionRequestData::parse(buffer)};
}

void ConnectResponse::write(ByteBufferWriter &writer) {
  writer.writeUint8(channelId);
  writer.writeUint8(status);
  dataEndPoint.write(writer);
}

std::uint8_t ConnectResponse::getChannelId() const { return channelId; }

std::uint8_t ConnectResponse::getStatus() const { return status; }
const ConnectionRequestData& ConnectResponse::getCRD() const {return crd;}

const HPAI& ConnectResponse::getDataEndPoint() const {return dataEndPoint;}
} // namespace knx::requestresponse
