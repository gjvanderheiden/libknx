#include "ManufacturerDib.h"
#include "IpAddress.h"
#include "KnxAddress.h"
#include "KnxStructure.h"
#include <cstdint>

ManufacturerDib::ManufacturerDib(IpAddress &&ipAddress)
    : KnxStructure(ManufacturerDib::TYPE), multicastAddress{std::move(ipAddress)} {}

ManufacturerDib ManufacturerDib::parse(ByteBufferReader &reader) {
  auto [length, type] = KnxStructure::parse(reader);

  auto knxMedium = reader.readUint8();
  auto deviceStatus = reader.readUint8();
  auto individualAddress = IndividualAddress::parse(reader);
  auto projectInstallationIdentifier = reader.readUint16();
  std::array<std::uint8_t, 6> serialNumber;
  reader.copyToSpan(serialNumber);
  auto multicastAddress = IpAddress::parse(reader);
  ManufacturerDib result{std::move(multicastAddress)};
  reader.copyToSpan(result.macAddress);
  result.knxMedium = knxMedium;
  result.deviceStatus = deviceStatus;
  result.individualAddress = individualAddress;
  result.projectInstallationIdentifier = projectInstallationIdentifier;
  result.serialNumber = serialNumber;
  result.deviceName = reader.readTerminatedString(30);
  return result;
}

void ManufacturerDib::write(ByteBufferWriter &data) const {

}

std::string_view ManufacturerDib::getManufacturerName() { return deviceName; }

std::span<std::uint8_t> ManufacturerDib::getSerialNumber() { return serialNumber; }
