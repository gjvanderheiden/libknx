#pragma once

#include "DeviceDib.h"
#include "HPAI.h"
#include "bytes/ByteBuffer.h"

namespace knx::requestresponse {

class SearchResponse {
public:
  static SearchResponse parse(ByteBuffer& buffer);
  HPAI getContronEndPoint();
  DeviceDib& getDeviceDib();

  static constexpr std::array<std::uint8_t, 2> SERVICE_ID = {0x02, 0x02};

private:
  SearchResponse(HPAI &&controlEndPoint, DeviceDib && deviceDib);

  HPAI controlEndPoint;
  DeviceDib deviceDib;
};

}
