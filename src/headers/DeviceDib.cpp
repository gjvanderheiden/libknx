#include "headers/DeviceDib.h"
#include "IpAddress.h"
#include "KnxAddress.h"
#include <cstdint>

DeviceDib DeviceDib::parse(ByteBuffer& byteBuffer) {
  DeviceDib dib{};
  dib.knxMedium  = byteBuffer.readUint8();
  dib.deviceStatus = byteBuffer.readUint8();
  dib.individualAddress = IndividualAddress::parse(byteBuffer);
  dib.projectInstallationIdentifier = byteBuffer.readUint16();
  byteBuffer.takeCopyToSpan(dib.serialNumber);
  dib.multicastAddress = IpAddress::parse(byteBuffer);
  byteBuffer.takeCopyToSpan(dib.macAddress);
  int max = 30;

  std::span<std::uint8_t> knxText = byteBuffer.takeByteSpan(max);
  int i = 0;
  for (i = 0; knxText[i] != 0x00 && i < max; i++) {
        dib.deviceName.push_back(knxText[i]);
  }
  byteBuffer.skip(max - i);

  return dib;
}


std::string_view DeviceDib::getDeviceName() {
  return deviceName;
}

std::span<std::uint8_t> DeviceDib::getSerialNumber() {
  return serialNumber;
}
