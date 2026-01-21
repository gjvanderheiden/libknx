#pragma once

#include "knx/bytes/ByteBufferWriter.h"

template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};

class AbstractRequestResponse {
protected:
  AbstractRequestResponse() = default;
  AbstractRequestResponse(const AbstractRequestResponse &) = default;
  AbstractRequestResponse &operator=(const AbstractRequestResponse &) = default;
  AbstractRequestResponse(AbstractRequestResponse &&) = default;
  AbstractRequestResponse &operator=(AbstractRequestResponse &&) = default;

public:
  virtual ~AbstractRequestResponse() = default;
  virtual void write(ByteBufferWriter &writer) const = 0;
  virtual std::uint16_t getServiceType() const = 0;
  std::vector<byte> toBytes() const {
    std::vector<byte> data;
    ByteBufferWriter writer(data);
    this->write(writer);
    return data;
  }
};
