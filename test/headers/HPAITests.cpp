#include "HPAI.h"
#include <asio/detached.hpp>
#include <asio/read.hpp>
#include <cstddef>
#include <gtest/gtest.h>

class BufferStream {
public:
  BufferStream(ByteSpan bytes) :bytes{bytes}{}
  
 asio::awaitable<void> asyncReadSome(ByteSpan b){
    std::copy(bytes.begin() + index, bytes.begin() + b.size() + index, b.data());
    this->index += b.size();
    co_return;
  }
private:
  ByteSpan bytes;
  std::size_t index{0};
};

struct Konijn {
  HPAI hpai;
};

asio::awaitable<void> readFromStream(ByteSpan bytes, Konijn& answer) {
  BufferStream bufferReader{bytes};
  StreamReader<BufferStream> sr{bufferReader};
  answer.hpai = co_await HPAI::readFromStream(sr);
  co_return;
}

HPAI readFromStream(ByteSpan bytes) {
  asio::io_context io_context;
  Konijn answer;
  asio::co_spawn(io_context, readFromStream(bytes,answer), asio::detached);
  io_context.run();
  return answer.hpai;
}

TEST(HPAI, toByteArray) {
  HPAI hpai(IpAddress{}, 0xABCD, HPAI::UDP);
  auto answer = hpai.bodyAsByteArray();
  
  EXPECT_EQ(8, answer.size());

  EXPECT_EQ(0x08, answer[0]); // size
  EXPECT_EQ(0x01, answer[1]); // udp
  EXPECT_EQ(0x00, answer[2]); // ip
  EXPECT_EQ(0x00, answer[3]);
  EXPECT_EQ(0x00, answer[4]);
  EXPECT_EQ(0x00, answer[5]);
  EXPECT_EQ(0xAB, answer[6]); // port
  EXPECT_EQ(0xCD, answer[7]);

  HPAI fromArray = readFromStream(answer);
  EXPECT_TRUE(fromArray.isUdp());
  EXPECT_EQ(0xABCD, fromArray.getPort());
}
