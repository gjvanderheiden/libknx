#pragma once

#include "knx/ipreqresp/KnxIpHeader.h"
#include "knx/ipreqresp/requests/requests.h"
#include "knx/ipreqresp/responses/responses.h"

enum class MessageType : std::uint8_t { request, response, unknown };

class RequestResponseFactory {
public:
  static MessageType getType(KnxIpHeader &ipHeader);
  static RequestVariant parseRequest(KnxIpHeader &header,
                                     ByteBufferReader &reader);
  static knx::requestresponse::ResponseVariant parseResponse(KnxIpHeader &header,
                                     ByteBufferReader &reader);
};
