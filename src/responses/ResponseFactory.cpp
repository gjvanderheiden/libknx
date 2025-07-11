#include "ResponseFactory.h"
#include "ConnectResponse.h"
#include "DisconnectResponse.h"
#include "SearchResponse.h"
#include "TunnelAckResponse.h"
#include "ResponseFactory.h"
#include "ResponseFactory.h"

#include "KnxIpHeader.h"

namespace knx::requestresponse {
bool ResponseFactory::parse(ByteBufferReader &reader) {
  bool parsed;
  KnxIpHeader header = KnxIpHeader::createAndParse(reader);
  std::uint16_t serviceType = header.getServiceType();
  
  if(ConnectResponse::SERVICE_ID == serviceType) {

  } else if (DisconnectResponse::SERVICE_ID == serviceType) {
  } else if (SearchResponse::SERVICE_ID == serviceType) {
  } else if (TunnelAckResponse::SERVICE_ID == serviceType) {
  } else {
  }
  return parsed;
}
} // namespace knx::requestresponse
