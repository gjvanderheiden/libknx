#pragma once

#include "knx/ipreqresp/AbstractRequestResponse.h"
#include "knx/ipreqresp/responses/responses.h"

class AbstractRequest  : public AbstractRequestResponse {
protected:
  AbstractRequest() = default;
  AbstractRequest(const AbstractRequest &) = default;
  AbstractRequest &operator=(const AbstractRequest &) = default;
  AbstractRequest(AbstractRequest &&) = default;
  AbstractRequest &operator=(AbstractRequest &&) = default;

public:
  ~AbstractRequest() override = default;
  virtual bool matchesResponse(knx::requestresponse::ResponseVariant response) const = 0;
};
