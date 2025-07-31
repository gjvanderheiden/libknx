#pragma once

#include "knx/headers/HPAI.h"

#include "knx/requests/AbstractRequest.h"

class SearchRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0201;

public:
  explicit SearchRequest(HPAI&& hpaiLocal);
  static SearchRequest newDefault();


  std::vector<std::uint8_t> toBytes() override;

  void appendToByteWriter(ByteBufferWriter& writer) const;

private:
  HPAI hpaiLocal;
};
