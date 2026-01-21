#pragma once

#include "knx/ipreqresp/responses/ConnectResponse.h"
#include "knx/ipreqresp/responses/ConnectStateResponse.h"
#include "knx/ipreqresp/responses/DescriptionResponse.h"
#include "knx/ipreqresp/responses/DisconnectResponse.h"
#include "knx/ipreqresp/responses/SearchResponse.h"
#include "knx/ipreqresp/responses/TunnelAckResponse.h"
#include <variant>

namespace knx::requestresponse {
using ResponseVariant =
    std::variant<std::monostate, ConnectResponse, ConnectStateResponse, TunnelAckResponse,
                 SearchResponse, DisconnectResponse, DescriptionResponse>;
}
