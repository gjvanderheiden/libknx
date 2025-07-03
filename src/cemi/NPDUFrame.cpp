#include "NPDUFrame.h"



NPDUFrame NPDUFrame::createAndParse(ByteBufferReader& bytebuffer) {
  auto tcpi = TCPI::parseAndCreate(bytebuffer);
  return {};
}
