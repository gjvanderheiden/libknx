#include "knx/requests/DescriptionRequest.h"
#include "knx/headers/KnxIpHeader.h"

DescriptionRequest::DescriptionRequest(HPAI &&controlEndPoint)
    : controlEndPoint{std::move(controlEndPoint)} {
}

void DescriptionRequest::write(ByteBufferWriter& writer) {
  KnxIpHeader{SERVICE_ID, SIZE}.write(writer);
  controlEndPoint.write(writer);
}

DescriptionRequest DescriptionRequest::parse(ByteBufferReader& reader) {
  return DescriptionRequest{HPAI::parse(reader)};
}
