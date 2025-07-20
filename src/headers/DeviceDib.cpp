#include "headers/DeviceDib.h"
#include "IpAddress.h"
#include "KnxAddress.h"
#include "KnxStructure.h"
#include <cstdint>

DeviceDib::DeviceDib(IpAddress &&ipAddress)
    : KnxStructure(0x00), multicastAddress{std::move(ipAddress)} {}

DeviceDib DeviceDib::parseAndCreate(ByteBufferReader &byteBuffer) {
  auto [length, type] = KnxStructure::parse(byteBuffer);

  auto knxMedium = byteBuffer.readUint8();
  auto deviceStatus = byteBuffer.readUint8();
  auto individualAddress = IndividualAddress::createAndParse(byteBuffer);
  auto projectInstallationIdentifier = byteBuffer.readUint16();
  std::array<std::uint8_t, 6> serialNumber;
  byteBuffer.copyToSpan(serialNumber);
  auto multicastAddress = IpAddress::parse(byteBuffer);
  DeviceDib result{std::move(multicastAddress)};
  byteBuffer.copyToSpan(result.macAddress);
  result.knxMedium = knxMedium;
  result.deviceStatus = deviceStatus;
  result.individualAddress = individualAddress;
  result.projectInstallationIdentifier = projectInstallationIdentifier;
  result.serialNumber = serialNumber;
  result.deviceName = byteBuffer.readTerminatedString(30);
  return result;
}

void DeviceDib::appendToByteArray(ByteBufferWriter &data) const {

}

std::string_view DeviceDib::getDeviceName() { return deviceName; }

std::span<std::uint8_t> DeviceDib::getSerialNumber() { return serialNumber; }
