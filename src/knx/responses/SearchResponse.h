#pragma once

#include "knx/headers/DeviceDib.h"
#include "knx/headers/HPAI.h"
#include "knx/bytes/ByteBufferReader.h"
#include "knx/headers/SupportedServiceFamiliesDib.h"

namespace knx::requestresponse {

class SearchResponse {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0202;
public:
  SearchResponse(HPAI &&controlEndPoint, DeviceDib && deviceDib, SupportedServiceFamiliesDib&& supportedServiceFamiliesDib);

  static SearchResponse parse(ByteBufferReader& reader);

  const HPAI& getControlEndPoint() const;
  const DeviceDib& getDeviceDib() const;
  const SupportedServiceFamiliesDib& getSupportedServiceFamiliesDib() const;

private:
  const HPAI controlEndPoint;
  const DeviceDib deviceDib;
  const SupportedServiceFamiliesDib supportedServiceFamiliesDib;
};

}
