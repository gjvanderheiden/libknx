#pragma once

#include "knx/headers/DeviceDib.h"
#include "knx/headers/HPAI.h"
#include "knx/bytes/ByteBufferReader.h"
#include "knx/headers/SupportedServiceFamiliesDib.h"

namespace knx::requestresponse {

class DescriptionResponse {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0204;
public:
  DescriptionResponse(DeviceDib && deviceDib, SupportedServiceFamiliesDib&& supportedServiceFamiliesDib);

  static DescriptionResponse parse(ByteBufferReader& reader);

  const DeviceDib& getDeviceDib() const;
  const SupportedServiceFamiliesDib& getSupportedServiceFamiliesDib() const;

private:
  const DeviceDib deviceDib;
  const SupportedServiceFamiliesDib supportedServiceFamiliesDib;
};

}
