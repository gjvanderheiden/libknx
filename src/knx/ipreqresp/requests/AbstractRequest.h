#pragma once

#include "knx/ipreqresp/AbstractRequestResponse.h"
#include "knx/ipreqresp/responses/responses.h"

class AbstractRequest  : public AbstractRequestResponse {
protected:
  AbstractRequest() = default;
  AbstractRequest(const AbstractRequest &) = default;
  AbstractRequest &operator=(const AbstractRequest &) = default;
  AbstractRequest(AbstractRequest &&) noexcept = default;
  AbstractRequest &operator=(AbstractRequest &&) noexcept = default;

public:
  ~AbstractRequest() override = default;
  [[nodiscard]] virtual bool matchesResponse(knx::requestresponse::ResponseVariant response) const = 0;
};
