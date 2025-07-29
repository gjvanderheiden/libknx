#include "knx/requests/ConnectRequest.h"

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
  writer.writeUint8(0x06);
  writer.writeUint8(0x10);
  writer.writeUint16(SERVICE_ID);
  writer.writeUint16(26);
  controlEndPoint.write(writer);
  dataEndPoint.write(writer);
  cri.write(writer);
  return bytes;
}
