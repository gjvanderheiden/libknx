#include "DeviceDib.h"
#include "IpAddress.h"
#include "KnxAddress.h"
#include "KnxStructure.h"
#include <cstdint>

DeviceDib::DeviceDib(IpAddress &&ipAddress)
    : KnxStructure(DeviceDib::TYPE), multicastAddress{std::move(ipAddress)} {}

DeviceDib DeviceDib::parse(ByteBufferReader &reader) {
  auto [length, type] = KnxStructure::parse(reader);

  auto knxMedium = reader.readUint8();
  auto deviceStatus = reader.readUint8();
  auto individualAddress = IndividualAddress::parse(reader);
  auto projectInstallationIdentifier = reader.readUint16();
  std::array<std::uint8_t, LENGTH_SERIAL_NUMBER> serialNumber{0};
  reader.copyToSpan(serialNumber);
  auto multicastAddress = IpAddress::parse(reader);
  DeviceDib result{std::move(multicastAddress)};
  reader.copyToSpan(result.macAddress);
  result.knxMedium = knxMedium;
  result.deviceStatus = deviceStatus;
  result.individualAddress = individualAddress;
  result.projectInstallationIdentifier = projectInstallationIdentifier;
  result.serialNumber = serialNumber;
  result.deviceName = reader.readTerminatedString(MAX_LENGTH_DEVICE_NAME);
  return result;
}

void DeviceDib::write(ByteBufferWriter &writer) const {
  writeKnxStructure(writer, 2);
  writer.writeUint8(knxMedium);
  writer.writeUint8(deviceStatus);
  individualAddress.write(writer);
  writer.writeUint16(projectInstallationIdentifier);
  writer.writeByteSpan(serialNumber);
  multicastAddress.write(writer);
  writer.writeByteSpan(macAddress);
}

std::string_view DeviceDib::getDeviceName() const { return deviceName; }

std::span<const std::uint8_t> DeviceDib::getSerialNumber() const { return serialNumber; }
