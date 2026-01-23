#include "knx/ipreqresp/requests/DescriptionRequest.h"
#include "knx/ipreqresp/KnxIpHeader.h"
#include "knx/ipreqresp/responses/DisconnectResponse.h"

DescriptionRequest::DescriptionRequest(HPAI &&controlEndPoint)
    : controlEndPoint{std::move(controlEndPoint)} {
}

void DescriptionRequest::write(ByteBufferWriter& writer) const {
  KnxIpHeader{SERVICE_ID, SIZE}.write(writer);
  controlEndPoint.write(writer);
}

DescriptionRequest DescriptionRequest::parse(ByteBufferReader& reader) {
  return DescriptionRequest{HPAI::parse(reader)};
}


std::string DescriptionRequest::getShortDescription() const {
  return "DescriptionRequest";
}

bool DescriptionRequest::matchesResponse(
    knx::requestresponse::ResponseVariant response) const {
  return std::holds_alternative<knx::requestresponse::DescriptionResponse>(response);
}

