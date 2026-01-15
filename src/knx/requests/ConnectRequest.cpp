#include "knx/requests/ConnectRequest.h"
#include "knx/headers/KnxIpHeader.h"

ConnectRequest::ConnectRequest(HPAI &&controlEndPoint,
                                     HPAI &&dataEndPoint,
                                     ConnectionRequestInformation &&cri)
    : controlEndPoint{std::move(controlEndPoint)},
      dataEndPoint{std::move(dataEndPoint)}, 
      cri{std::move(cri)} {}


ConnectRequest ConnectRequest::parse(ByteBufferReader reader) {
  auto controlEndPoint = HPAI::parse(reader);
  auto dataEndPoint = HPAI::parse(reader);
  auto cri = ConnectionRequestInformation::parse(reader);
  return ConnectRequest{std::move(controlEndPoint), std::move(dataEndPoint), std::move(cri)};
}

void ConnectRequest::write(ByteBufferWriter& writer) {
  KnxIpHeader{SERVICE_ID, SIZE}.write(writer);
  controlEndPoint.write(writer);
  dataEndPoint.write(writer);
  cri.write(writer);
}
