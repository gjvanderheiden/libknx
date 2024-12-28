#pragma once

#include "bytes/ByteBuffer.h"

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
