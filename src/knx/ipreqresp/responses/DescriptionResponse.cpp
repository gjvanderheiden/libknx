
#include "knx/ipreqresp/responses/DescriptionResponse.h"
#include "knx/headers/DeviceDib.h"
#include "knx/headers/SupportedServiceFamiliesDib.h"

namespace knx::requestresponse {

DescriptionResponse::DescriptionResponse(
    DeviceDib &&deviceDib,
    SupportedServiceFamiliesDib &&supportedServiceFamiliesDib)
    : deviceDib{std::move(deviceDib)},
      supportedServiceFamiliesDib{std::move(supportedServiceFamiliesDib)} {}

const SupportedServiceFamiliesDib &
DescriptionResponse::getSupportedServiceFamiliesDib() const {
  return this->supportedServiceFamiliesDib;
}

const DeviceDib &DescriptionResponse::getDeviceDib() const {
  return this->deviceDib;
}

DescriptionResponse DescriptionResponse::parse(ByteBufferReader &reader) {
  return {DeviceDib::parse(reader), SupportedServiceFamiliesDib::parse(reader)};
}

void DescriptionResponse::write(ByteBufferWriter &writer) const {
  deviceDib.write(writer);
  supportedServiceFamiliesDib.write(writer);
}

} // namespace knx::requestresponse
