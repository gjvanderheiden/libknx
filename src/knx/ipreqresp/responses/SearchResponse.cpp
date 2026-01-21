
#include "knx/ipreqresp/responses/SearchResponse.h"
#include "knx/headers/DeviceDib.h"
#include "knx/headers/SupportedServiceFamiliesDib.h"

namespace knx::requestresponse {

SearchResponse::SearchResponse(
    HPAI &&controlEndPoint, DeviceDib &&deviceDib,
    SupportedServiceFamiliesDib &&supportedServiceFamiliesDib)
    : controlEndPoint{std::move(controlEndPoint)},
      deviceDib{std::move(deviceDib)},
      supportedServiceFamiliesDib{std::move(supportedServiceFamiliesDib)} {}

const HPAI &SearchResponse::getControlEndPoint() const {
  return this->controlEndPoint;
}

const SupportedServiceFamiliesDib &
SearchResponse::getSupportedServiceFamiliesDib() const {
  return this->supportedServiceFamiliesDib;
}

const DeviceDib &SearchResponse::getDeviceDib() const {
  return this->deviceDib;
}

void SearchResponse::write(ByteBufferWriter &writer) const {
  controlEndPoint.write(writer);
  deviceDib.write(writer);
  supportedServiceFamiliesDib.write(writer);
}

SearchResponse SearchResponse::parse(ByteBufferReader &reader) {
  return {HPAI::parse(reader), DeviceDib::parse(reader),
          SupportedServiceFamiliesDib::parse(reader)};
}

} // namespace knx::requestresponse
