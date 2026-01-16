#pragma once

#include "knx/bytes/ByteBufferWriter.h"

class AbstractRequest {
protected:
  AbstractRequest() = default;
  AbstractRequest(const AbstractRequest &) = default;
  AbstractRequest &operator=(const AbstractRequest &) = default;
  AbstractRequest(AbstractRequest &&) = default;
  AbstractRequest &operator=(AbstractRequest &&) = default;

public:
  virtual ~AbstractRequest() = default;
  virtual void write(ByteBufferWriter &writer) = 0;
};
