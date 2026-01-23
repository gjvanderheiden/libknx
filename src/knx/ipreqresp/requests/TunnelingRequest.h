#pragma once

#include "knx/cemi/Cemi.h"
#include "knx/headers/ConnectionHeader.h"

#include "knx/ipreqresp/requests/AbstractRequest.h"

class TunnelRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0420;

public:
  TunnelRequest() = delete;
  TunnelRequest(const TunnelRequest &) = default;
  TunnelRequest(TunnelRequest &&) noexcept = default;
  TunnelRequest &operator=(const TunnelRequest &) = default;
  TunnelRequest &operator=(TunnelRequest &&) noexcept = default;
  ~TunnelRequest() override = default;

public:
  TunnelRequest(ConnectionHeader &&connectionHeader, Cemi &&cemi);
  void write(ByteBufferWriter& writer) const override;
  static TunnelRequest parse(ByteBufferReader& reader);
  [[nodiscard]] constexpr std::uint16_t getServiceType() const override {return SERVICE_ID;};
  [[nodiscard]] bool matchesResponse(knx::requestresponse::ResponseVariant response) const override;
  [[nodiscard]] std::string getShortDescription() const override;

  [[nodiscard]] const ConnectionHeader& getConnectionHeader() const;
  [[nodiscard]] const Cemi& getCemi() const;

private:
  ConnectionHeader connectionHeader;
  Cemi cemi;
};
