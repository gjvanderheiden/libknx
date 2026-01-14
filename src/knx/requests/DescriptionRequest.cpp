#include "knx/requests/DescriptionRequest.h"
#include "knx/headers/KnxIpHeader.h"

DescriptionRequest::DescriptionRequest(HPAI &&controlEndPoint)
    : controlEndPoint{std::move(controlEndPoint)} {
}


std::vector<std::uint8_t> DescriptionRequest::toBytes() {
  std::vector<std::uint8_t> bytes;
  bytes.reserve(SIZE);
  ByteBufferWriter writer{bytes};
  KnxIpHeader{SERVICE_ID, SIZE}.write(writer);
  controlEndPoint.write(writer);
  return bytes;
}
