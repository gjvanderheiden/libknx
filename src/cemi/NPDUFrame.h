#pragma once


#include "ByteBufferWriter.h"
#include "bytes/ByteBufferReader.h"
#include "cemi/TCPI.h"
#include "cemi/ACPI.h"


class NPDUFrame {
public:
  NPDUFrame(TCPI&& tcpi, DataACPI&& acpi);
  static NPDUFrame createAndParse(ByteBufferReader& bytebuffer);
  void toBytes(ByteBufferWriter& writer) const;
  [[nodiscard]] const TCPI& getTCPI() const;
  [[nodiscard]] const DataACPI& getACPI() const;
private:
  TCPI tcpi;
  DataACPI acpi;
};
