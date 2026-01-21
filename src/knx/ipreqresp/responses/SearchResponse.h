#pragma once

#include "knx/headers/DeviceDib.h"
#include "knx/headers/HPAI.h"
#include "knx/bytes/ByteBufferReader.h"
#include "knx/headers/SupportedServiceFamiliesDib.h"
#include "knx/ipreqresp/responses/AbstractResponse.h"

namespace knx::requestresponse {

class SearchResponse final : public AbstractResponse {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0202;

public:
  SearchResponse() = delete;
  SearchResponse(const SearchResponse &) = default;
  SearchResponse(SearchResponse &&) noexcept = default;
  SearchResponse &operator=(const SearchResponse &) = default;
  SearchResponse &operator=(SearchResponse &&) noexcept = default;
  ~SearchResponse() override = default;

public:
  SearchResponse(HPAI &&controlEndPoint, DeviceDib &&deviceDib,
                 SupportedServiceFamiliesDib &&supportedServiceFamiliesDib);

  void write(ByteBufferWriter &writer) const override;
  static SearchResponse parse(ByteBufferReader& reader);
  [[nodiscard]] constexpr std::uint16_t getServiceType() const override {return SERVICE_ID;}

  [[nodiscard]] const HPAI& getControlEndPoint() const;
  [[nodiscard]] const DeviceDib& getDeviceDib() const;
  [[nodiscard]] const SupportedServiceFamiliesDib& getSupportedServiceFamiliesDib() const;

private:
  HPAI controlEndPoint;
  DeviceDib deviceDib;
  SupportedServiceFamiliesDib supportedServiceFamiliesDib;
};

}
