
#include "requestresponses/SearchResponse.h"
#include <cmath>

namespace knx::reqeuestresponse {

SearchResponse::SearchResponse(HPAI&& controlEndPoint) 
  : controlEndPoint{std::move(controlEndPoint)} {
}

template <typename T>
SearchResponse SearchResponse::parse(StreamReader<T>& stream) {
  auto type = stream.readUint16();
  auto length = stream.readUint16();
  HPAI controlEndPoint = HPAI::readFromStream(stream);
  return {std::move(controlEndPoint)};
}

} // namespace knx::reqeuestrespons
