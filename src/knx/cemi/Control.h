#pragma once

#include "knx/bytes/ByteBufferWriter.h"
#include "knx/bytes/ByteBufferReader.h"
#include <cstdint>

enum class KnxPrio : std::uint8_t {
  system = 0,
  normal = 1,
  urgent = 2,
  low = 3,
};


class Control {
public:
  Control() = default;
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
  bool standardFrameType{true};
  bool repeatOnError {true};
  bool systemBroadcast{false};
  KnxPrio priority;
  bool acknowledgeWanted{true};
  bool confirmationOnError{true};
  bool destinationAddressIsGroup;
  std::uint8_t hopCount;
  // 4 bits extended frame format;
};
