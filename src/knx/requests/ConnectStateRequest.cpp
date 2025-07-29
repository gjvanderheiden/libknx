#include "knx/requests/ConnectStateRequest.h"

ConnectStateRequest::ConnectStateRequest(HPAI&& controlEndPoint, std::uint8_t channelId) :
  controlEndPoint{std::move(controlEndPoint)}, channelId{channelId} {
}

std::vector<std::uint8_t> ConnectStateRequest::toBytes() {
  std::vector<std::uint8_t> bytes;
  ByteBufferWriter writer{bytes};
  writer.writeUint8(0x06);
  writer.writeUint8(0x10);
  writer.writeUint16(SERVICE_ID);
  writer.writeUint16(16);
  writer.writeUint8(channelId);
  writer.writeUint8(0x00);
  controlEndPoint.write(writer);
  return bytes;
}
