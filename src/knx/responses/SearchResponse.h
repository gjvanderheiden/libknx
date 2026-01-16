#pragma once

#include "knx/headers/DeviceDib.h"
#include "knx/headers/HPAI.h"
#include "knx/bytes/ByteBufferReader.h"
#include "knx/headers/SupportedServiceFamiliesDib.h"
#include "knx/responses/AbstractResponse.h"

namespace knx::requestresponse {

class SearchResponse final : public AbstractResponse {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0202;

public:
  SearchResponse(const SearchResponse &) = default;
  SearchResponse(SearchResponse &&) = delete;
  SearchResponse &operator=(const SearchResponse &) = delete;
  SearchResponse &operator=(SearchResponse &&) = delete;
  ~SearchResponse() override = default;

public:
  SearchResponse(HPAI &&controlEndPoint, DeviceDib &&deviceDib,
                 SupportedServiceFamiliesDib &&supportedServiceFamiliesDib);

  void write(ByteBufferWriter &writer) override;
  static SearchResponse parse(ByteBufferReader& reader);

  [[nodiscard]] const HPAI& getControlEndPoint() const;
  [[nodiscard]] const DeviceDib& getDeviceDib() const;
  [[nodiscard]] const SupportedServiceFamiliesDib& getSupportedServiceFamiliesDib() const;

private:
  HPAI controlEndPoint;
  DeviceDib deviceDib;
  SupportedServiceFamiliesDib supportedServiceFamiliesDib;
};

}
