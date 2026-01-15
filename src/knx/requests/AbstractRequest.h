#pragma once

#include "knx/bytes/ByteBufferWriter.h"

class AbstractRequest {
public:
  virtual ~AbstractRequest() = default;

  virtual void write(ByteBufferWriter &writer) = 0;
};
