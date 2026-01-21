#pragma once

#include "knx/bytes/ByteBufferWriter.h"

template <class... Ts> struct Overloaded : Ts... {
  using Ts::operator()...;
};

class AbstractRequestResponse {
protected:
  AbstractRequestResponse() = default;
  AbstractRequestResponse(const AbstractRequestResponse &) = default;
  AbstractRequestResponse &operator=(const AbstractRequestResponse &) = default;
  AbstractRequestResponse(AbstractRequestResponse &&) noexcept = default;
  AbstractRequestResponse &operator=(AbstractRequestResponse &&) noexcept = default;

public:
  virtual ~AbstractRequestResponse() = default;
  virtual void write(ByteBufferWriter &writer) const = 0;
  [[nodiscard]] virtual std::uint16_t getServiceType() const = 0;
  [[nodiscard]] std::vector<byte> toBytes() const {
    std::vector<byte> data;
    ByteBufferWriter writer(data);
    this->write(writer);
    return data;
  }
};
