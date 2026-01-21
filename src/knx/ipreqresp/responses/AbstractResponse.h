#pragma once

#include "knx/ipreqresp/AbstractRequestResponse.h"

class AbstractResponse : public AbstractRequestResponse {
protected:
  AbstractResponse() = default;
  AbstractResponse(const AbstractResponse &) = default;
  AbstractResponse(AbstractResponse &&) = default;
  AbstractResponse &operator=(const AbstractResponse &) = default;
  AbstractResponse &operator=(AbstractResponse &&) = default;

public:
  ~AbstractResponse() override = default;
};
