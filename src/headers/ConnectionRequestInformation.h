#pragma once

#include "KnxStructure.h"
#include <array>
#include <cstdint>

class ConnectionRequestInformation : public KnxStructure{
public:

  std::array<std::uint8_t, 8> bodyAsByteArray();
  void appendToByteArray(std::vector<std::uint8_t>& data);
protected:
  void parseBody(ByteBufferReader& byteBuffer, std::uint16_t length) override;


private:
};


