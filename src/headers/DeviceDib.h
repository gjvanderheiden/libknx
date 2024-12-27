#pragma once

#include "KnxAddress.h"
#include "bytes/ByteBuffer.h"
#include <cstdint>
#include <array>
#include <string>

class DeviceDib {

 DeviceDib parse(ByteBuffer& byteBuffer);
private:
 IndividualAddress individualAddress;
 std::string deviceName;
 std::array<std::uint8_t, 6> serialNumber;


};
