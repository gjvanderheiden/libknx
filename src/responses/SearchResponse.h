#pragma once

#include "DeviceDib.h"
#include "HPAI.h"
#include "bytes/ByteBufferReader.h"

namespace knx::requestresponse {

class SearchResponse {
public:
  static SearchResponse parse(ByteBufferReader& buffer);
  HPAI getControlEndPoint();
  DeviceDib& getDeviceDib();

  static constexpr std::uint16_t SERVICE_ID = 0x0202;

private:
  SearchResponse(HPAI &&controlEndPoint, DeviceDib && deviceDib);

  HPAI controlEndPoint;
  DeviceDib deviceDib;
};

}
