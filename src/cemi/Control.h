#pragma once

#include "bytes/ByteBufferReader.h"

class Control {
public:
  static Control parse(ByteBufferReader &byteBuffer);
  bool isDestinationGroupAddress() const;

private:
  // frameType
  // repeatOnError
  // systemBroadcast
  // priority
  // acknowledgeWanted
  // confirm
  bool destinationAdresIsGroup;
  // hopCount
  // eff;
};
