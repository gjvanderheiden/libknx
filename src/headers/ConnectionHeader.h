#pragma once

#include "KnxStructure.h"
#include "bytes/ByteBufferReader.h"
#include <cstdint>

class ConnectionHeader final : public KnxStructure {
public:
  ConnectionHeader(std::uint8_t channel, std::uint8_t sequence,
                   std::uint8_t status = 0);

  void write(ByteBufferWriter &writer) const override;
  static ConnectionHeader parse(ByteBufferReader &reader);

  std::uint8_t getSequence() const;
  std::uint8_t getChannel() const;
  std::uint8_t getStatus() const;

private:
  const std::uint8_t sequence;
  const std::uint8_t status;
};
