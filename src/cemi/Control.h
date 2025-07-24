#pragma once

#include "ByteBufferWriter.h"
#include "bytes/ByteBufferReader.h"
#include <cstdint>

enum class KnxPrio : std::uint8_t {
  system = 0,
  normal = 1,
  urgent = 2,
  low = 3,
};


class Control {
public:
  explicit Control(KnxPrio prio, bool destinationAddressIsGroup, std::uint8_t hopCount = 5);

  static Control parse(ByteBufferReader &reader);
  void write(ByteBufferWriter &writer) const;

  [[nodiscard]] bool isStandardFrameType() const;
  [[nodiscard]] bool repeatOnErrorSet() const;
  [[nodiscard]] bool systemBroadcastSet() const;
  [[nodiscard]] KnxPrio getPrio() const;
  [[nodiscard]] bool isAcknowledgeWanted() const;
  [[nodiscard]] bool confirmationOnErrorSet() const;
  [[nodiscard]] bool isDestinationGroupAddress() const;
  [[nodiscard]] std::uint8_t getHopCount() const;

private:
  bool frameType;
  bool repeatOnError;
  bool systemBroadcast;
  KnxPrio priority;
  bool acknowledgeWanted;
  bool confirmationOnError;
  bool destinationAddressIsGroup;
  std::uint8_t hopCount;
  // 4 bits extended frame format;
};
