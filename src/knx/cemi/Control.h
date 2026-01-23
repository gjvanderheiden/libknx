#pragma once

#include "knx/bytes/ByteBufferReader.h"
#include "knx/bytes/ByteBufferWriter.h"
#include <cstdint>

enum class KnxPrio : std::uint8_t {
  system = 0,
  normal = 1,
  urgent = 2,
  low = 3,
};

class Control {
private:
  static constexpr std::uint8_t STANDARD_FRAME_TYPE_BIT_POS = 8;
  static constexpr std::uint8_t REPEAT_ON_ERROR_POS         = 6;
  static constexpr std::uint8_t SYSTEM_BROADCAST_POS        = 5;
  static constexpr std::uint8_t ACKNOWLEDGE_WANTED_POS      = 2;
  static constexpr std::uint8_t CONFIRMATION_ON_ERROR_POS   = 1;

  static constexpr std::uint8_t IS_GROUP_ADDRESS_POS        = 8;
public:
  static constexpr uint8_t DEFAULT_HOP_COUNT = 5;

public:
  explicit Control(KnxPrio prio, bool destinationAddressIsGroup,
                   std::uint8_t hopCount = DEFAULT_HOP_COUNT);

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
  bool repeatOnError{true};
  bool systemBroadcast{false};
  bool acknowledgeWanted{true};
  bool confirmationOnError{true};
  bool destinationAddressIsGroup;
  KnxPrio priority;
  std::uint8_t hopCount;
  // 4 bits extended frame format;
};
