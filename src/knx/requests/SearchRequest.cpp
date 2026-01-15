#include "knx/requests/SearchRequest.h"
#include "knx/headers/HPAI.h"
#include "knx/headers/IpAddress.h"
#include "knx/bytes/ByteBufferWriter.h"
#include "knx/headers/KnxIpHeader.h"

SearchRequest::SearchRequest(HPAI&& hpaiLocal) : hpaiLocal(std::move(hpaiLocal)) {}

void SearchRequest::write(ByteBufferWriter& writer) {
  KnxIpHeader{SERVICE_ID, SIZE}.write(writer);
  hpaiLocal.write(writer);
}

SearchRequest SearchRequest::newDefault() {
  const IpAddress multicast{244, 0, 23, 12};
  return SearchRequest(HPAI(multicast, 5371, HPAI::UDP));
}

SearchRequest SearchRequest::parse(ByteBufferReader& reader) {
  return SearchRequest{HPAI::parse(reader)};

}
