#include "knx/ipreqresp/requests/TunnelingRequest.h"
#include "knx/ipreqresp/KnxIpHeader.h"
#include "knx/ipreqresp/responses/TunnelAckResponse.h"
#include <variant>
TunnelRequest::TunnelRequest(ConnectionHeader &&connectionHeader, Cemi &&cemi)
    : connectionHeader{std::move(connectionHeader)}, cemi{std::move(cemi)} {}

void TunnelRequest::write(ByteBufferWriter &writer) const {
  std::vector<std::uint8_t> requestBytes;
  ByteBufferWriter requestWriter{requestBytes};
  connectionHeader.write(requestWriter);
  cemi.write(requestWriter);

  KnxIpHeader ipHeader{SERVICE_ID,
                       static_cast<std::uint16_t>(requestBytes.size() +
                                                  KnxIpHeader::SIZE_HEADER)};
  ipHeader.write(writer);
  writer.writeVector(requestBytes);
}

TunnelRequest TunnelRequest::parse(ByteBufferReader &reader) {
  return TunnelRequest{ConnectionHeader::parse(reader), Cemi::parse(reader)};
}

const ConnectionHeader &TunnelRequest::getConnectionHeader() const {
  return connectionHeader;
}

bool TunnelRequest::matchesResponse(
    knx::requestresponse::ResponseVariant response) const {
  return std::visit(
      Overloaded{[&cHeader = connectionHeader](
                     const knx::requestresponse::TunnelAckResponse &response) {
                   return response.getConnectionHeader().getChannel() ==
                              cHeader.getChannel() &&
                          response.getConnectionHeader().getSequence() ==
                              cHeader.getSequence();
                 },
                 [](const auto &/*other*/) { return false; }},
      response);
}

const Cemi &TunnelRequest::getCemi() const { return cemi; }
