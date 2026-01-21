
#pragma once

#include "KnxAddress.h"
#include "KnxStructure.h"
#include <cstdint>

class ConnectionRequestData final : public KnxStructure {
public:
  static constexpr std::uint8_t TUNNELING_TYPE = 4;

public:
  ConnectionRequestData() = delete;

  ConnectionRequestData(const ConnectionRequestData &) = default;
  ConnectionRequestData(ConnectionRequestData &&) = default;
  ConnectionRequestData &operator=(const ConnectionRequestData &) = default;
  ConnectionRequestData &operator=(ConnectionRequestData &&) = default;
  ~ConnectionRequestData() override = default;

public:
  ConnectionRequestData(std::uint8_t type, IndividualAddress &&address);
  static ConnectionRequestData newTunneling();

  static ConnectionRequestData parse(ByteBufferReader &reader);
  void write(ByteBufferWriter &writer) const override;

  [[nodiscard]] const IndividualAddress& getAddress() const;

private:
  IndividualAddress address;
};
