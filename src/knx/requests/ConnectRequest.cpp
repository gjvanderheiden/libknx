#include "knx/requests/ConnectRequest.h"
#include "knx/headers/KnxIpHeader.h"

ConnectRequest::ConnectRequest(HPAI &&controlEndPoint,
                                     HPAI &&dataEndPoint,
                                     ConnectionRequestInformation &&cri)
    : controlEndPoint{std::move(controlEndPoint)},
      dataEndPoint{std::move(dataEndPoint)}, 
      cri{std::move(cri)} {
}


std::vector<std::uint8_t> ConnectRequest::toBytes() {
  std::vector<std::uint8_t> bytes;
  bytes.reserve(SIZE);
  ByteBufferWriter writer{bytes};
  KnxIpHeader{SERVICE_ID, SIZE}.write(writer);
  controlEndPoint.write(writer);
  dataEndPoint.write(writer);
  cri.write(writer);
  return bytes;
}
