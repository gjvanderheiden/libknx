#pragma once

#include "knx/headers/HPAI.h"

#include "knx/ipreqresp/requests/AbstractRequest.h"

class SearchRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0201;
  static constexpr std::uint16_t SIZE = 14;

public:
  SearchRequest(const SearchRequest &) = delete;
  SearchRequest(SearchRequest &&) noexcept = default;
  SearchRequest &operator=(const SearchRequest &) = default;
  SearchRequest &operator=(SearchRequest &&) noexcept = default;
  ~SearchRequest() override = default; 
public:
  explicit SearchRequest(HPAI &&hpaiLocal);
  static SearchRequest newDefault();

  void write(ByteBufferWriter& writer) const override;
  static SearchRequest parse(ByteBufferReader& reader);
  [[nodiscard]] std::string getShortDescription() const override;

  [[nodiscard]] constexpr std::uint16_t getServiceType() const override {return SERVICE_ID;};
  [[nodiscard]] bool matchesResponse(knx::requestresponse::ResponseVariant response) const override;

private:
  HPAI hpaiLocal;
};
