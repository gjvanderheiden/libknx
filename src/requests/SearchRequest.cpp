#include "SearchRequest.h"
#include "HPAI.h"
#include "IpAddress.h"
#include <algorithm>
#include <iterator>

SearchRequest::SearchRequest(HPAI hpaiLocal) : hpaiLocal(hpaiLocal) {}

std::vector<std::uint8_t> SearchRequest::toBytes() {
  std::vector<std::uint8_t> bytes;
  // knx ip header
  bytes.push_back(0x06);
  bytes.push_back(0x10);
  // ip header: service id
  std::copy(SERVICE_ID.begin(), SERVICE_ID.end(), std::back_inserter(bytes));
  // ip header: total header size
  bytes.push_back(0);
  bytes.push_back(14); //  6 + body size
  // ip header: body
  auto bytesHpai = hpaiLocal.bodyAsByteArray();
  std::copy(bytesHpai.begin(), bytesHpai.end(), std::back_inserter(bytes));
  return bytes;
}

SearchRequest SearchRequest::newDefault() {
  IpAddress multicast{};
  multicast.address = {244, 0, 23, 12};
  return SearchRequest(HPAI(IpAddress{}, 5371, HPAI::UDP));
}
