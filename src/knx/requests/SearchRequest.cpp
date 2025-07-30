#include "knx/requests/SearchRequest.h"
#include "knx/headers/HPAI.h"
#include "knx/headers/IpAddress.h"
#include "knx/bytes/ByteBufferWriter.h"
#include "knx/headers/KnxIpHeader.h"

SearchRequest::SearchRequest(HPAI hpaiLocal) : hpaiLocal(hpaiLocal) {}

std::vector<std::uint8_t> SearchRequest::toBytes() {
  std::vector<std::uint8_t> result;
  ByteBufferWriter writer{result};
  appendToByteWriter(writer);
  return result;
}
void SearchRequest::appendToByteWriter(ByteBufferWriter& writer) {
  KnxIpHeader{SERVICE_ID, 14}.write(writer);
  hpaiLocal.write(writer);
}

SearchRequest SearchRequest::newDefault() {
  IpAddress multicast{244, 0, 23, 12};
  return SearchRequest(HPAI(multicast, 5371, HPAI::UDP));
}
