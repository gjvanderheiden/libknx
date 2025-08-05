#pragma once

#include "knx/headers/KnxStructure.h"
#include "knx/headers/IpAddress.h"
#include "knx/headers/KnxAddress.h"
#include "knx/bytes/ByteBufferReader.h"
#include <array>
#include <cstdint>
#include <string>

class DeviceDib final : public KnxStructure {
public:
   static constexpr std::uint8_t TYPE = 0x01;

   static constexpr std::uint8_t PROGRAMMING_MODE_STATUS = 0x01;

public:
  std::string_view getDeviceName() const;
  std::span<const std::uint8_t> getSerialNumber() const;

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
