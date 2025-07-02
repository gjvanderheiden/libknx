#include "NPDUFrame.h"



NPDUFrame NPDUFrame::createAndParse(ByteBuffer& bytebuffer) {
  auto tcpi = TCPI::parseAndCreate(bytebuffer);
  return {};
}
