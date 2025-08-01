#pragma once

#include "knx/headers/DeviceDib.h"
#include "knx/headers/HPAI.h"
#include "knx/bytes/ByteBufferReader.h"

namespace knx::requestresponse {

class SearchResponse {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0202;
public:
  SearchResponse(HPAI &&controlEndPoint, DeviceDib && deviceDib);

  static SearchResponse parse(ByteBufferReader& buffer);

  HPAI getControlEndPoint();
  DeviceDib& getDeviceDib();

private:
  HPAI controlEndPoint;
  DeviceDib deviceDib;
};

}
