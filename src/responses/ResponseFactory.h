#pragma once

#include "bytes/ByteBufferReader.h"

namespace knx::requestresponse {

class ResponseFactory {
public:
  static bool parse(ByteBufferReader& reader);
  static void process();
};

}
