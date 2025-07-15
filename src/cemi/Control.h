#pragma once

#include "bytes/ByteBufferReader.h"

class Control {
public:
  explicit Control(bool destinationAddressIsGroup);
  static Control parse(ByteBufferReader &byteBuffer);
  [[nodiscard]] bool isDestinationGroupAddress() const;

private:
  // frameType
  // repeatOnError
  // systemBroadcast
  // priority
  // acknowledgeWanted
  // confirm
  bool destinationAddressIsGroup;
  // hopCount
  // eff;
};
