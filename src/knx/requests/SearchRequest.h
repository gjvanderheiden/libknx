#pragma once

#include "knx/headers/HPAI.h"

#include "knx/requests/AbstractRequest.h"

class SearchRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0201;
  static constexpr std::uint16_t SIZE = 14;

public:
  SearchRequest(const SearchRequest &) = default;
  SearchRequest(SearchRequest &&) = default;
  SearchRequest &operator=(const SearchRequest &) = delete;
  SearchRequest &operator=(SearchRequest &&) = delete;
  ~SearchRequest() override = default; 
public:
  explicit SearchRequest(HPAI &&hpaiLocal);
  static SearchRequest newDefault();

  void write(ByteBufferWriter& writer) override;
  static SearchRequest parse(ByteBufferReader& reader);

private:
  HPAI hpaiLocal;
};
