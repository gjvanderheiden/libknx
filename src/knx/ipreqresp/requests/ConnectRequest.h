#pragma once

#include "knx/ipreqresp/requests/AbstractRequest.h"
#include "knx/headers/ConnectionRequestInformation.h"
#include "knx/headers/HPAI.h"
#include "knx/bytes/ByteBufferReader.h"

class ConnectRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0205;
  static constexpr std::uint16_t SIZE = 26;

public:
  ConnectRequest() = delete;
  ConnectRequest(const ConnectRequest &) = default;
  ConnectRequest(ConnectRequest &&) noexcept = default;
  ConnectRequest &operator=(const ConnectRequest &) = default;
  ConnectRequest &operator=(ConnectRequest &&) noexcept = default;
  ~ConnectRequest() override = default;

public:
  explicit ConnectRequest(HPAI &&controlEndPoint, HPAI &&dataEndPoint,
                          ConnectionRequestInformation &&cri);

  static ConnectRequest parse(ByteBufferReader reader);
  void write(ByteBufferWriter &writer) const override;
  [[nodiscard]] constexpr std::uint16_t getServiceType() const override {return SERVICE_ID;};

  [[nodiscard]] bool matchesResponse(knx::requestresponse::ResponseVariant response) const override;
  [[nodiscard]] std::string getShortDescription() const override;
private:
  HPAI controlEndPoint;
  HPAI dataEndPoint;
  ConnectionRequestInformation cri;
};
