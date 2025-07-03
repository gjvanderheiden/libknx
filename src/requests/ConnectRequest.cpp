#include "ConnectRequest.h"

ConnectRequest::ConnectRequest(HPAI &&controlEndPoint,
                                     HPAI &&dataEndPoint,
                                     ConnectionRequestInformation &&cri)
    : controlEndPoint{std::move(controlEndPoint)},
      dataEndPoint{std::move(dataEndPoint)}, 
      cri{std::move(cri)} {
}


std::vector<std::uint8_t> ConnectRequest::toBytes() {
  std::vector<std::uint8_t> bytes;
  controlEndPoint.appendToByteArray(bytes);
  dataEndPoint.appendToByteArray(bytes);
  cri.appendToByteArray(bytes);
  return bytes;
}
