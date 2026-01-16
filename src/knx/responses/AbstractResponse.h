#pragma once

#include "knx/bytes/ByteBufferWriter.h"

class AbstractResponse {
protected:
  AbstractResponse() = default;
  AbstractResponse(const AbstractResponse &) = default;
  AbstractResponse(AbstractResponse &&) = default;
  AbstractResponse &operator=(const AbstractResponse &) = default;
  AbstractResponse &operator=(AbstractResponse &&) = default;

public:
  virtual ~AbstractResponse() = default;
  virtual void write(ByteBufferWriter &writer) = 0;
};
