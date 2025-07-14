#pragma once

#include "HPAI.h"

#include "AbstractRequest.h"

class SearchRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0201;

public:
  explicit SearchRequest(HPAI hpaiLocal);
  static SearchRequest newDefault();


  void appendToByteWriter(ByteBufferWriter& writer); 

private:
  HPAI hpaiLocal;
};
