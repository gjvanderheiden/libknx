#include "headers/DeviceDib.h"
#include "IpAddress.h"
#include "KnxAddress.h"
#include <cstdint>

void DeviceDib::parseBody(ByteBufferReader& byteBuffer, std::uint16_t length) {
  knxMedium  = byteBuffer.readUint8();
  deviceStatus = byteBuffer.readUint8();
  individualAddress = IndividualAddress::createAndParse(byteBuffer);
  projectInstallationIdentifier = byteBuffer.readUint16();
  byteBuffer.copyToSpan(serialNumber);
  multicastAddress = IpAddress::parse(byteBuffer);
  byteBuffer.copyToSpan(macAddress);
  deviceName = byteBuffer.readTerminatedString(30);
}

DeviceDib DeviceDib::createAndParse(ByteBufferReader& byteBuffer) {
  DeviceDib deviceDib;
  deviceDib.parse(byteBuffer);
  return deviceDib;
}

void DeviceDib::appendToByteArray(ByteBufferWriter& data) {
}

std::string_view DeviceDib::getDeviceName() {
  return deviceName;
}

std::span<std::uint8_t> DeviceDib::getSerialNumber() {
  return serialNumber;
}
