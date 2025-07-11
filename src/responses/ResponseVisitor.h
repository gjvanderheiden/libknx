#pragma once

#include "responses/ConnectResponse.h"
#include "responses/ConnectStateResponse.h"
#include "responses/DisconnectResponse.h"
#include "responses/SearchResponse.h"
#include "responses/TunnelAckResponse.h"

namespace knx::requestresponse {

class ResponseVisitor {
public:
  virtual void visit(ConnectResponse& response) = 0;
  virtual void visit(ConnectStateResponse& response) = 0;
  virtual void visit(DisconnectResponse& response) = 0;
  virtual void visit(SearchResponse& response) = 0;
  virtual void visit(TunnelAckResponse& response) = 0;

};

}
