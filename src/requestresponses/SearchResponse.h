#pragma once

#include "HPAI.h"
#include "bytes/ByteBuffer.h"

namespace knx::reqeuestresponse {

class SearchResponse {
public:
  template <typename T>
  static SearchResponse parse(StreamReader<T>& buffer);

private:
  SearchResponse(HPAI&& controlEndPoint);

  HPAI controlEndPoint;
};

}
