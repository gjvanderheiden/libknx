#pragma once

#include "bytes/ByteBuffer.h"

class IpAddress {
public:
  template <typename T>
  static asio::awaitable<IpAddress> parse(StreamReader<T>& byteBuffer);
  std::array<uint8_t,4> address{0};
private:

};


template <typename T>
asio::awaitable<IpAddress> IpAddress::parse(StreamReader<T>& byteBuffer) {
  IpAddress ipAddress;
  co_await byteBuffer.readToSpan(ipAddress.address);
  co_return ipAddress;
}
