#pragma once

#include "knx/headers/DeviceDib.h"
#include "knx/bytes/ByteBufferReader.h"
#include "knx/headers/SupportedServiceFamiliesDib.h"
#include "knx/responses/AbstractResponse.h"

namespace knx::requestresponse {

class DescriptionResponse final : public AbstractResponse {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0204;
public:
  DescriptionResponse(const DescriptionResponse &) = default;
  DescriptionResponse(DescriptionResponse &&) = delete;
  DescriptionResponse &operator=(const DescriptionResponse &) = delete;
  DescriptionResponse &operator=(DescriptionResponse &&) = delete;
  ~DescriptionResponse() override = default;

public:
  DescriptionResponse(
      DeviceDib &&deviceDib,
      SupportedServiceFamiliesDib &&supportedServiceFamiliesDib);

  static DescriptionResponse parse(ByteBufferReader& reader);
  void write(ByteBufferWriter &writer) override;

  [[nodiscard]] const DeviceDib& getDeviceDib() const;
  [[nodiscard]] const SupportedServiceFamiliesDib& getSupportedServiceFamiliesDib() const;

private:
  DeviceDib deviceDib;
  SupportedServiceFamiliesDib supportedServiceFamiliesDib;
};

}
