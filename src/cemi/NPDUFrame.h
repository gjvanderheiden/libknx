#pragma once


#include "bytes/ByteBuffer.h"
#include "cemi/TCPI.h"

class NPDUFrame {
public:
  static NPDUFrame createAndParse(ByteBuffer& bytebuffer);
private:
  TCPI tcpi;
  //acpi
};
