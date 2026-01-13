#pragma once

#include "knx/bytes/ByteBufferReader.h"
#include "knx/bytes/ByteBufferWriter.h"
#include <cstdint>

enum class AdditionalInformationType : std::uint8_t {
  pl_medium_information = 0x01,
  rf_medium_information = 0x02,
  bus_monitor_status_info = 0x03,
  timestamp_relative = 0x04,
  time_delay_until_sending = 0x05,
  extended_relative_timestamp = 0x06,
  bibat_information = 0x07,
  rf_multi_information = 0x08,
  preamble_and_postamble = 0x09,
  rf_fast_ack_information = 0x0a,
  manufacturer_specific_data = 0xfe,
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
  static AdditionalInformation parse(ByteBufferReader &reader);
  void write(ByteBufferWriter &writer) const;

private:
  std::vector<AdditionalInformationElement> elements{};
};
