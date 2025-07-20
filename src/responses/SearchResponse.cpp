
#include "responses/SearchResponse.h"
#include "DeviceDib.h"

namespace knx::requestresponse {

SearchResponse::SearchResponse(HPAI &&controlEndPoint, DeviceDib && deviceDib)
    : controlEndPoint{std::move(controlEndPoint)}, deviceDib{std::move(deviceDib)} {}

HPAI SearchResponse::getControlEndPoint() {
  return this->controlEndPoint;
}

DeviceDib& SearchResponse::getDeviceDib() {
  return this->deviceDib;
}
SearchResponse SearchResponse::parse(ByteBufferReader &buffer) {
  HPAI controlEndPoint = HPAI::createAndParse(buffer);
  return {std::move(controlEndPoint), DeviceDib::parseAndCreate(buffer)};
}

} // namespace knx::requestresponse
