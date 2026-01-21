#pragma once

#include "knx/ipreqresp/AbstractRequestResponse.h"

class AbstractResponse : public AbstractRequestResponse {
protected:
  AbstractResponse() = default;
  AbstractResponse(const AbstractResponse &) = default;
  AbstractResponse(AbstractResponse &&) noexcept = default;
  AbstractResponse &operator=(const AbstractResponse &) = default;
  AbstractResponse &operator=(AbstractResponse &&) noexcept = default;

public:
  ~AbstractResponse() override = default;
};
