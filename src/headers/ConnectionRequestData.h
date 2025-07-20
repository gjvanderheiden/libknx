
#pragma once

#include "KnxAddress.h"
#include "KnxStructure.h"
#include <cstdint>

class ConnectionRequestData final : public KnxStructure {
public:
  static constexpr std::uint8_t TUNNELING_TYPE = 4;
  ConnectionRequestData(std::uint8_t type, IndividualAddress&& address);
  static ConnectionRequestData createAndParse(ByteBufferReader &data);
  static ConnectionRequestData newTunneling();
  const IndividualAddress& getAddress() const;

  void appendToByteArray(ByteBufferWriter &data) const override;
private:
  IndividualAddress address;

};
