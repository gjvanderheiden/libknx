#pragma once

#include "knx/cemi/Cemi.h"
#include "knx/headers/ConnectionHeader.h"
#include <vector>

#include "AbstractRequest.h"

class TunnelRequest final : public AbstractRequest {
public:
  TunnelRequest(ConnectionHeader&& connectionHeader,  Cemi&& cemi );

  static constexpr std::uint16_t SERVICE_ID = 0x0420;

  static TunnelRequest parse(ByteBufferReader& reader);
  [[nodiscard]] const ConnectionHeader& getConnectionHeader() const;
  [[nodiscard]] const Cemi& getCemi() const;

  std::vector<std::uint8_t> toBytes() override;

private:
  const ConnectionHeader connectionHeader;
  const Cemi cemi;
};
