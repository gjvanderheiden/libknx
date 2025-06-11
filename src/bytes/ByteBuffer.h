#pragma once

#include <asio.hpp>
#include <bit>
#include <concepts>
#include <cstdint>
#include <string_view>
#include <span>

using byte = std::uint8_t;
using ByteSpan = std::span<byte>;
using asio::awaitable;

class ByteBuffer {
public:
  ByteBuffer(std::span<byte> data);
  std::uint32_t takeUInt32BigEndian();
  void skip(int numberOfBytes);
  std::string takeString(int numberOfBytes);
  ByteSpan takeByteSpan(int numberOfBytes);
  void takeCopyToSpan(std::span<byte> destination);
  bool takeBoolFromByte();
  std::uint8_t readUint8();
  std::uint16_t readUint16();

  static constexpr bool isNativeLittleEndian() {
    return std::endian::native == std::endian::little;
  }

private:
  std::span<byte> data;
  int index{0};
};

template <typename S>
concept WriteStream = requires(S s, ByteSpan &b) {
  { s.asyncWriteSome(b) } -> std::same_as<asio::awaitable<void>>;
};

template <typename S>
  requires WriteStream<S>
class StreamWriter {
public:
  StreamWriter(S &stream);

  awaitable<void> writeInt32BigEndian(std::uint32_t data);
  awaitable<void> writeToSpan(ByteSpan byteSpan);
  awaitable<void> writeBoolAsByte(bool data);
  awaitable<void> writeByte(byte byte);
  awaitable<void> writeIntAsByte(int data);
  awaitable<void> writeUint16(std::uint16_t data);
};

template <typename S>
concept ReadStream = requires(S s, ByteSpan &b) {
  { s.asyncReadSome(b) } -> std::same_as<asio::awaitable<void>>;
};

template <typename S>
  requires ReadStream<S>
class StreamReader {
public:
  StreamReader(S &stream);

  awaitable<int> readUInt32BigEndian();
  awaitable<void> skip(int numberOfBytes);
  awaitable<std::string_view> readString(int numberOfBytes);
  awaitable<void> readToSpan(ByteSpan byteSpan);
  awaitable<bool> readBoolFromByte();
  awaitable<byte> readByte();
  awaitable<int> readByteAsInt();
  awaitable<std::uint16_t> readUint16();
  awaitable<std::uint8_t> readUint8();

  static consteval bool isNativeLittleEndian() {
    return std::endian::native == std::endian::little;
  }

private:
  S &stream;
};

template <typename S>
  requires ReadStream<S>
StreamReader<S>::StreamReader(S &stream) : stream{stream} {}

template <typename S>
  requires ReadStream<S>
awaitable<std::uint8_t> StreamReader<S>::readUint8() {
  std::uint8_t answer;
  co_await stream.asyncReadSome(ByteSpan{&answer, 1});
  co_return answer;
}

template <typename S>
  requires ReadStream<S>
awaitable<std::uint16_t> StreamReader<S>::readUint16() {
  std::uint16_t answer;
  co_await stream.asyncReadSome(ByteSpan{(std::uint8_t *)&answer, 2});
  co_return std::byteswap(answer);
}

template <typename S>
  requires ReadStream<S>
awaitable<void> StreamReader<S>::readToSpan(ByteSpan span) {
  co_await stream.asyncReadSome(span);
  co_return;
}

template <typename S>
concept Serializable = requires(S s, ByteBuffer &b) {
  { s.toBytes() } -> std::same_as<ByteBuffer>;
  { S::parse(b) } -> std::same_as<S>;
};
