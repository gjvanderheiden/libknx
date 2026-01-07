#pragma once

#include "knx/headers/KnxStructure.h"
#include "knx/bytes/ByteBufferReader.h"
#include <cstdint>

class ManufacturerDib final : public KnxStructure {
public:
  ManufacturerDib();
   static constexpr std::uint8_t TYPE = 0xFE;

public:
  static ManufacturerDib parse(ByteBufferReader& reader);
  void write(ByteBufferWriter& writer) const override;
};
