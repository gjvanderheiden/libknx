#pragma once

#include "bytes/ByteBuffer.h"

class Control {
public:
  static Control parse(ByteBuffer &byteBuffer);
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
