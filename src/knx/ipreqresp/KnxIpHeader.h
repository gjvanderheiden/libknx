#pragma once

#include "knx/bytes/ByteBufferReader.h"
#include "knx/bytes/ByteBufferWriter.h"
#include <cstdint>

/**
 * All ip messages / frames are wrapped in the header. This is used to check the service message (request / response) type 
 */
class KnxIpHeader {
public:
   static constexpr std::uint8_t SIZE_HEADER = 0x06; //Constant size of KNXnet/IP as defined in protocol version 1.0
   static constexpr std::uint8_t PROTOCOL_VERSION = 0x10; // 0x10 KNXnet/IP protocol version 1.0
                                                          //
public:
  KnxIpHeader(std::uint16_t serviceType, std::uint16_t lengthInBytes);

  void write(ByteBufferWriter& writer) const;
  static void write(ByteBufferWriter& writer, std::uint16_t serviceType, std::uint16_t lengthInBytes);

  static KnxIpHeader parse(ByteBufferReader& reader);

  [[nodiscard]] std::uint16_t getServiceType() const;
  [[nodiscard]] std::uint16_t getLengthInBytes() const;
private:
  std::uint16_t serviceType;
  std::uint16_t lengthInBytes;
};
