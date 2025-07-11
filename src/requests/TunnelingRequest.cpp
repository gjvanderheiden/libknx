#include "TunnelingRequest.h"


TunnelingRequest::TunnelingRequest(Cemi &&cemi,
                                   ConnectionHeader &&connectionHeader)
    : cemi{std::move(cemi)}, connectionHeader{std::move(connectionHeader)} {}



std::vector<std::uint8_t> TunnelingRequest::toBytes(){return{};}

