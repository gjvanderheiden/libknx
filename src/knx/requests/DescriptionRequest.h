#pragma once

#include "knx/requests/AbstractRequest.h"
#include "knx/headers/HPAI.h"

class DescriptionRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0203;
  static constexpr std::uint16_t SIZE = 14;

public:
  explicit DescriptionRequest(HPAI&& controlEndPoint);

  void write(ByteBufferWriter &writer) override;
  static DescriptionRequest parse(ByteBufferReader& reader);
private:
  HPAI controlEndPoint;
};
