#pragma once

#include "knx/bytes/ByteBufferWriter.h"
#include "knx/bytes/ByteBufferReader.h"
#include "knx/cemi/TCPI.h"
#include "knx/cemi/ACPI.h"


class NPDUFrame {
public:
  NPDUFrame(TCPI&& tcpi, DataACPI&& acpi);

  static NPDUFrame parse(ByteBufferReader& bytebuffer);
  void write(ByteBufferWriter& writer) const;

  [[nodiscard]] const TCPI& getTCPI() const;
  [[nodiscard]] const DataACPI& getACPI() const;

private:
  TCPI tcpi;
  DataACPI acpi;
};
