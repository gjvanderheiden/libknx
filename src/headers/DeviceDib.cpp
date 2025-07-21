#include "headers/DeviceDib.h"
#include "IpAddress.h"
#include "KnxAddress.h"
#include "KnxStructure.h"
#include <cstdint>

DeviceDib::DeviceDib(IpAddress &&ipAddress)
    : KnxStructure(0x00), multicastAddress{std::move(ipAddress)} {}

DeviceDib DeviceDib::parse(ByteBufferReader &reader) {
  auto [length, type] = KnxStructure::parse(reader);

  auto knxMedium = reader.readUint8();
  auto deviceStatus = reader.readUint8();
  auto individualAddress = IndividualAddress::parse(reader);
  auto projectInstallationIdentifier = reader.readUint16();
  std::array<std::uint8_t, 6> serialNumber;
  reader.copyToSpan(serialNumber);
  auto multicastAddress = IpAddress::parse(reader);
  DeviceDib result{std::move(multicastAddress)};
  reader.copyToSpan(result.macAddress);
  result.knxMedium = knxMedium;
  result.deviceStatus = deviceStatus;
  result.individualAddress = individualAddress;
  result.projectInstallationIdentifier = projectInstallationIdentifier;
  result.serialNumber = serialNumber;
  result.deviceName = reader.readTerminatedString(30);
  return result;
}

void DeviceDib::write(ByteBufferWriter &data) const {

}

std::string_view DeviceDib::getDeviceName() { return deviceName; }

std::span<std::uint8_t> DeviceDib::getSerialNumber() { return serialNumber; }
