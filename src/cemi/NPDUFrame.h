#pragma once


#include "bytes/ByteBufferReader.h"
#include "cemi/TCPI.h"

class NPDUFrame {
public:
  static NPDUFrame createAndParse(ByteBufferReader& bytebuffer);
private:
  TCPI tcpi;
  //acpi
};
