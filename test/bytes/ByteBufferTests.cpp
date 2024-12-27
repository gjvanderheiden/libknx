
#include "bytes/ByteBuffer.h"
#include <gtest/gtest.h>

class BufferStreamPje {
public:
  BufferStreamPje(ByteSpan bytes) :bytes{bytes}{}
  
 asio::awaitable<void> asyncReadSome(ByteSpan b){
    std::copy(bytes.begin(), bytes.end(), b.data());
    co_return;
  }
private:
  ByteSpan bytes;
};

struct KonijnTje {
  int answer;
};

asio::awaitable<void> readFromStream(ByteSpan bytes, KonijnTje& answer) {
  BufferStreamPje bufferReader{bytes};
  StreamReader<BufferStreamPje> sr{bufferReader};
  answer.answer = co_await sr.readUint8();
  co_return;
}

asio::awaitable<void> readFromStream16(ByteSpan bytes, KonijnTje& answer) {
  BufferStreamPje bufferReader{bytes};
  StreamReader<BufferStreamPje> sr{bufferReader};
  answer.answer = co_await sr.readUint16();
  co_return;
}

TEST(ByteBuffer, readUint8) {
  std::uint8_t testValue = 255;
  asio::io_context io_context;
  KonijnTje answer;
  asio::co_spawn(io_context, readFromStream(std::span{&testValue, 1},answer), asio::detached);
  io_context.run();
  ASSERT_EQ(testValue, answer.answer);
}

TEST(ByteBuffer, readUint16) {
  std::uint16_t testValue = 65003;
  asio::io_context io_context;
  KonijnTje answer;
  asio::co_spawn(io_context, readFromStream16(std::span{(std::uint8_t*)&testValue, 2},answer), asio::detached);
  io_context.run();
  ASSERT_EQ(0xebfd, answer.answer);
}
