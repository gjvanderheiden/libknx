#pragma once

#include "knx/ipreqresp/requests/ConnectRequest.h"
#include "knx/ipreqresp/requests/ConnectStateRequest.h"
#include "knx/ipreqresp/requests/DisconnectRequest.h"
#include "knx/ipreqresp/requests/TunnelingRequest.h"
#include "knx/ipreqresp/requests/SearchRequest.h"
#include "knx/ipreqresp/requests/DescriptionRequest.h"

using RequestVariant = std::variant<std::monostate, ConnectRequest, ConnectStateRequest, TunnelRequest, SearchRequest, DisconnectRequest, DescriptionRequest>;
