#include "ManufacturerDib.h"
#include "KnxStructure.h"

ManufacturerDib::ManufacturerDib()
    : KnxStructure(ManufacturerDib::TYPE) {} 

ManufacturerDib ManufacturerDib::parse(ByteBufferReader &reader) {
  auto [length, type] = KnxStructure::parse(reader);
  reader.skip(length);
  return {};
}

void ManufacturerDib::write(ByteBufferWriter &writer) const {

}

