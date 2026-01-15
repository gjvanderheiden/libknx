#include "knx/requests/ConnectStateRequest.h"
#include "knx/headers/KnxIpHeader.h"

ConnectStateRequest::ConnectStateRequest(HPAI &&controlEndPoint,
                                         const std::uint8_t channelId)
    : controlEndPoint{std::move(controlEndPoint)}, channelId{channelId} {}

void ConnectStateRequest::write(ByteBufferWriter& writer) {
  KnxIpHeader{SERVICE_ID, SIZE}.write(writer);
  writer.writeUint8(channelId);
  writer.writeUint8(0x00);
  controlEndPoint.write(writer);
}

ConnectStateRequest ConnectStateRequest::parse(ByteBufferReader reader) {
  std::uint8_t channelId = reader.readUint8();
  reader.skip(1);
  auto controlEndPoint = HPAI::parse(reader);
  return ConnectStateRequest{std::move(controlEndPoint), channelId};
}
