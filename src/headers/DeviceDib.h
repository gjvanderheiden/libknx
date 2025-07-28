#pragma once

#include "IpAddress.h"
#include "KnxAddress.h"
#include "KnxStructure.h"
#include "ByteBufferReader.h"
#include <array>
#include <cstdint>
#include <string>

class DeviceDib final : public KnxStructure {
public:
  std::string_view getDeviceName();
  std::span<std::uint8_t> getSerialNumber();

  static DeviceDib parse(ByteBufferReader& reader);
  void write(ByteBufferWriter& writer) const override;

private:
  DeviceDib(IpAddress&& ipAddress);
  IndividualAddress individualAddress;
  std::string deviceName;
  std::array<std::uint8_t, 6> serialNumber;
  std::uint8_t knxMedium;
  std::uint8_t deviceStatus;
  std::uint16_t projectInstallationIdentifier;
  const IpAddress multicastAddress;
  std::array<std::uint8_t, 6> macAddress;
};
