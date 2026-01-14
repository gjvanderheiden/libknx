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
   static constexpr std::uint8_t MAX_LENGTH_DEVICE_NAME = 30;
   static constexpr std::uint8_t LENGTH_SERIAL_NUMBER = 6;
   static constexpr std::uint8_t LENGTH_MAC_ADDRESS = 6;

public:
  [[nodiscard]] std::string_view getDeviceName() const;
  [[nodiscard]] std::span<const std::uint8_t> getSerialNumber() const;

  static DeviceDib parse(ByteBufferReader& reader);
  void write(ByteBufferWriter& writer) const override;

private:
  explicit DeviceDib(IpAddress&& ipAddress);
  IndividualAddress individualAddress;
  std::string deviceName;
  std::array<std::uint8_t, LENGTH_SERIAL_NUMBER> serialNumber;
  std::uint8_t knxMedium;
  std::uint8_t deviceStatus;
  std::uint16_t projectInstallationIdentifier;
  IpAddress multicastAddress;
  std::array<std::uint8_t, LENGTH_MAC_ADDRESS> macAddress;
};
