#include "headers/DeviceDib.h"

DeviceDib DeviceDib::parse(ByteBuffer& byteBuffer) {
  // byte knx medium
  // byte device status
  // IndividualAddress
  // 2 bytes, project installation identifier 
  // 6 bytes serial number
  // 4 bytes, multicast address 
  // 6 bytes, device mac address
  // knx string max 30, device name
  return {};
}
