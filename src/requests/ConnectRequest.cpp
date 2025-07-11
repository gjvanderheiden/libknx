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
  ByteBufferWriter writer{bytes};
  controlEndPoint.appendToByteArray(writer);
  dataEndPoint.appendToByteArray(writer);
  cri.appendToByteArray(writer);
  return bytes;
}
