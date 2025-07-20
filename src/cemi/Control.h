#pragma once

#include "ByteBufferWriter.h"
#include "bytes/ByteBufferReader.h"

class Control {
public:
  explicit Control(bool destinationAddressIsGroup);
  static Control parse(ByteBufferReader &byteBuffer);
  void toBytes(ByteBufferWriter &writer) const;
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
