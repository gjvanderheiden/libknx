#include "knx/ipreqresp/requests/SearchRequest.h"
#include "knx/bytes/ByteBufferWriter.h"
#include "knx/headers/HPAI.h"
#include "knx/headers/IpAddress.h"
#include "knx/ipreqresp/KnxIpHeader.h"

SearchRequest::SearchRequest(HPAI &&hpaiLocal)
    : hpaiLocal(std::move(hpaiLocal)) {}

void SearchRequest::write(ByteBufferWriter &writer) const {
  KnxIpHeader{SERVICE_ID, SIZE}.write(writer);
  hpaiLocal.write(writer);
}

SearchRequest SearchRequest::newDefault() {
  const IpAddress multicast{244, 0, 23, 12};
  return SearchRequest(HPAI(multicast, 5371, HPAI::UDP));
}

bool SearchRequest::matchesResponse(
    knx::requestresponse::ResponseVariant response) const {
  return std::holds_alternative<knx::requestresponse::SearchResponse>(response);
}

SearchRequest SearchRequest::parse(ByteBufferReader &reader) {
  return SearchRequest{HPAI::parse(reader)};
}

std::string SearchRequest::getShortDescription() const {
  return "SearchRequest";
}
