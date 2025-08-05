#include "knx/requests/DescriptionRequest.h"
#include "knx/headers/KnxIpHeader.h"

DescriptionRequest::DescriptionRequest(HPAI &&controlEndPoint)
    : controlEndPoint{std::move(controlEndPoint)} {
}


std::vector<std::uint8_t> DescriptionRequest::toBytes() {
  std::vector<std::uint8_t> bytes;
  ByteBufferWriter writer{bytes};
  KnxIpHeader{SERVICE_ID, 14}.write(writer);
  controlEndPoint.write(writer);
  return bytes;
}
