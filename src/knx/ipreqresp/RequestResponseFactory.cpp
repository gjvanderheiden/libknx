#include "knx/ipreqresp/RequestResponseFactory.h"
#include "knx/ipreqresp/requests/DisconnectRequest.h"
#include "knx/ipreqresp/requests/SearchRequest.h"
#include "knx/ipreqresp/requests/TunnelingRequest.h"
#include "knx/ipreqresp/responses/DisconnectResponse.h"


MessageType RequestResponseFactory::getType(KnxIpHeader& ipHeader) {
  switch(ipHeader.getServiceType()) {
    case TunnelRequest::SERVICE_ID:
    case ConnectStateRequest::SERVICE_ID:
    case ConnectRequest::SERVICE_ID:
    case DisconnectRequest::SERVICE_ID:
    case SearchRequest::SERVICE_ID:
    case DescriptionRequest::SERVICE_ID:
      return MessageType::request;
    case knx::requestresponse::TunnelAckResponse::SERVICE_ID:
    case knx::requestresponse::ConnectStateResponse::SERVICE_ID:
    case knx::requestresponse::ConnectResponse::SERVICE_ID:
    case knx::requestresponse::DisconnectResponse::SERVICE_ID:
    case knx::requestresponse::SearchResponse::SERVICE_ID:
    case knx::requestresponse::DescriptionResponse::SERVICE_ID:
      return MessageType::response;

    default:
      return MessageType::unknown;
  };
}

   
knx::requestresponse::ResponseVariant RequestResponseFactory::parseResponse(KnxIpHeader &header,
                                     ByteBufferReader &reader) {
  using namespace knx::requestresponse;

  switch(header.getServiceType()) {
    case TunnelAckResponse::SERVICE_ID:
      return ResponseVariant{TunnelAckResponse::parse(reader)};
    case ConnectStateResponse::SERVICE_ID:
      return ResponseVariant{ConnectStateResponse::parse(reader)};
    case ConnectResponse::SERVICE_ID:
      return ResponseVariant{ConnectResponse::parse(reader)};
    case DisconnectResponse::SERVICE_ID:
      return ResponseVariant{DisconnectResponse::parse(reader)};
    case SearchResponse::SERVICE_ID:
      return ResponseVariant{SearchResponse::parse(reader)};
    case DescriptionResponse::SERVICE_ID:
      return ResponseVariant{DescriptionResponse::parse(reader)};
    default:
      return ResponseVariant{std::monostate{}};
  };
}

RequestVariant  RequestResponseFactory::parseRequest(KnxIpHeader& header, ByteBufferReader& reader) {
  switch(header.getServiceType()) {
    case TunnelRequest::SERVICE_ID:
      return RequestVariant{TunnelRequest::parse(reader)};
    case ConnectStateRequest::SERVICE_ID:
      return RequestVariant{ConnectStateRequest::parse(reader)};
    case ConnectRequest::SERVICE_ID:
      return RequestVariant{ConnectRequest::parse(reader)};
    case DisconnectRequest::SERVICE_ID:
      return RequestVariant{DisconnectRequest::parse(reader)};
    case SearchRequest::SERVICE_ID:
      return RequestVariant{SearchRequest::parse(reader)};
    case DescriptionRequest::SERVICE_ID:
      return RequestVariant{DescriptionRequest::parse(reader)};
    default:
      return RequestVariant{std::monostate()};
  };
}


