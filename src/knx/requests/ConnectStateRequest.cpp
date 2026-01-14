#include "knx/requests/ConnectStateRequest.h"
#include "knx/headers/KnxIpHeader.h"

ConnectStateRequest::ConnectStateRequest(HPAI&& controlEndPoint, const std::uint8_t channelId) :
  controlEndPoint{std::move(controlEndPoint)}, channelId{channelId} {
}

std::vector<std::uint8_t> ConnectStateRequest::toBytes() {
  std::vector<std::uint8_t> bytes;
  bytes.reserve(SIZE);
  ByteBufferWriter writer{bytes};
  KnxIpHeader{SERVICE_ID, SIZE}.write(writer);
  writer.writeUint8(channelId);
  writer.writeUint8(0x00);
  controlEndPoint.write(writer);
  return bytes;
}
