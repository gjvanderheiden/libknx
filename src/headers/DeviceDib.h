#pragma once

#include "IpAddress.h"
#include "KnxAddress.h"
#include "KnxStructure.h"
#include "bytes/ByteBufferReader.h"
#include <array>
#include <cstdint>
#include <string>

class DeviceDib final : public KnxStructure {
public:
  static DeviceDib createAndParse(ByteBufferReader &byteBuffer);
  std::string_view getDeviceName();
  std::span<std::uint8_t> getSerialNumber();

  void appendToByteArray(ByteBufferWriter& data) override;
protected:
  void parseBody(ByteBufferReader& byteBuffer, std::uint16_t length) override;

private:
  IndividualAddress individualAddress;
  std::string deviceName;
  std::array<std::uint8_t, 6> serialNumber;
  std::uint8_t knxMedium;
  std::uint8_t deviceStatus;
  std::uint16_t projectInstallationIdentifier;
  IpAddress multicastAddress;
  std::array<std::uint8_t, 6> macAddress;
};
