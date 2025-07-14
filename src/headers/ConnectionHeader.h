#pragma once

#include "KnxStructure.h"
#include "bytes/ByteBufferReader.h"
#include <cstdint>

class ConnectionHeader final : public KnxStructure {
public:
  ConnectionHeader() = default;
  ConnectionHeader(std::uint8_t channel, std::uint8_t sequence,
                   std::uint8_t status = 0);

  void appendToByteArray(ByteBufferWriter &data) override;
  std::uint8_t getSeqeunce() const;
  std::uint8_t getChannel() const;
  std::uint8_t getStatus() const;

protected:
  void parseBody(ByteBufferReader &byteBuffer, std::uint16_t length) override;

private:
  std::uint8_t sequence{0};
  std::uint8_t status{0};
};
