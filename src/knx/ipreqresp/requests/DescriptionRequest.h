#pragma once

#include "knx/ipreqresp/requests/AbstractRequest.h"
#include "knx/headers/HPAI.h"

class DescriptionRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0203;
  static constexpr std::uint16_t SIZE = 14;

public:
  DescriptionRequest(const DescriptionRequest &) = delete;
  DescriptionRequest(DescriptionRequest &&) = default;
  DescriptionRequest &operator=(const DescriptionRequest &) = default;
  DescriptionRequest &operator=(DescriptionRequest &&) = default;
  ~DescriptionRequest() override = default;

public:
  explicit DescriptionRequest(HPAI &&controlEndPoint);

  void write(ByteBufferWriter &writer) const override;
  static DescriptionRequest parse(ByteBufferReader& reader);

  [[nodiscard]] constexpr std::uint16_t getServiceType() const override {return SERVICE_ID;};
  [[nodiscard]] bool matchesResponse(knx::requestresponse::ResponseVariant response) const override;
private:
  HPAI controlEndPoint;
};
