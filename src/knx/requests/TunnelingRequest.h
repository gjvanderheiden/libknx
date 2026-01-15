#pragma once

#include "knx/cemi/Cemi.h"
#include "knx/headers/ConnectionHeader.h"

#include "AbstractRequest.h"

class TunnelRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0420;

public:
  TunnelRequest(ConnectionHeader&& connectionHeader,  Cemi&& cemi );

  void write(ByteBufferWriter& writer) override;
  static TunnelRequest parse(ByteBufferReader& reader);

  [[nodiscard]] const ConnectionHeader& getConnectionHeader() const;
  [[nodiscard]] const Cemi& getCemi() const;


private:
  ConnectionHeader connectionHeader;
  Cemi cemi;
};
