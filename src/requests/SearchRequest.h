#pragma once

#include "HPAI.h"
#include <vector>

#include "AbstractRequest.h"

class SearchRequest final : AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0201;

public:
  explicit SearchRequest(HPAI hpaiLocal);
  static SearchRequest newDefault();


  void appendToByteWriter(ByteBufferWriter& writer); 
  void toBytes(std::vector<std::uint8_t>& byteBufferReader);

private:
  HPAI hpaiLocal;
};
