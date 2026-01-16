#pragma once

#include "knx/bytes/ByteBufferReader.h"
#include "knx/headers/KnxStructure.h"

class ManufacturerDib final : public KnxStructure {
public:
  static constexpr std::uint8_t TYPE = 0xFE;

public:
  ManufacturerDib();

  static ManufacturerDib parse(ByteBufferReader &reader);
  void write(ByteBufferWriter &writer) const override;
};
