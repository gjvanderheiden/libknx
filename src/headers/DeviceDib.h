#pragma once

#include "IpAddress.h"
#include "KnxAddress.h"
#include "bytes/ByteBuffer.h"
#include <array>
#include <cstdint>
#include <string>

class DeviceDib {
public:
  static DeviceDib parse(ByteBuffer &byteBuffer);
  std::string_view getDeviceName();
  std::span<std::uint8_t> getSerialNumber();

private:
  IndividualAddress individualAddress;
  std::string deviceName;
  std::array<std::uint8_t, 6> serialNumber;
  std::uint8_t knxMedium;
  std::uint8_t deviceStatus;
  std::uint16_t projectInstallationIdentifier;
  IpAddress multicastAddress;
  std::array<std::uint8_t, 8> macAddress;
};
