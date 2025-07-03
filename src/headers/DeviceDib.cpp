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
  int max = 30;

  std::span<std::uint8_t> knxText = byteBuffer.readByteSpan(max);
  int i = 0;
  for (i = 0; knxText[i] != 0x00 && i < max; i++) {
    deviceName.push_back(knxText[i]);
  }
  byteBuffer.skip(max - i);
}

DeviceDib DeviceDib::createAndParse(ByteBufferReader& byteBuffer) {
  DeviceDib deviceDib;
  deviceDib.parse(byteBuffer);
  return deviceDib;
}


std::string_view DeviceDib::getDeviceName() {
  return deviceName;
}

std::span<std::uint8_t> DeviceDib::getSerialNumber() {
  return serialNumber;
}
