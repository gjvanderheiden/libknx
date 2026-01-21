#pragma once

#include "knx/bytes/ByteBufferReader.h"
#include "knx/bytes/ByteBufferWriter.h"
#include <cstdint>

enum class AdditionalInformationType : std::uint8_t {
  plMediumInformation = 0x01,
  rfMediumInformation = 0x02,
  busMonitorStatusInfo = 0x03,
  timestampRelative = 0x04,
  timeDelayUntilSending = 0x05,
  extendedRelativeTimestamp = 0x06,
  bibatInformation = 0x07,
  rfMultiInformation = 0x08,
  preambleAndPostamble = 0x09,
  rfFastAckInformation = 0x0a,
  manufacturerSpecificData = 0xfe,
};

class AdditionalInformationElement {
public:
  void write(ByteBufferWriter &writer) const;
  [[nodiscard]] std::uint8_t frameSize() const;

  AdditionalInformationType type;
  std::vector<byte> data;
};

class AdditionalInformation {
public:
  AdditionalInformation() = default;
  static AdditionalInformation parse(ByteBufferReader &reader);
  void write(ByteBufferWriter &writer) const;

private:
  std::vector<AdditionalInformationElement> elements;
};
