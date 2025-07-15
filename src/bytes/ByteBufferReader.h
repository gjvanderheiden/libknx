#pragma once

#include <bit>
#include "ByteDef.h"
#include <string>

/**
 * I can read data from a byte array to specific types and keep track of the
 * position in the bytearray. I do not own the byte array, I only reference to
 * it and the return values from the read... methods are copies.
 *
 * Byte array types are converted as big endian, no functionality to change that
 * (not needed in KNX)
 *
 * I do take into account that the target cpu can be little of big endian.
 *
 */
class ByteBufferReader {

public:
  explicit ByteBufferReader(ByteSpan data);

  /**
   * Just skip numberOfBytes, call this when bytes are reserved / unknown or
   * you're simply no interested.
   */
  void skip(int numberOfBytes);

  /**
   * I will return a string with specified number of bytes.
   */
  std::string readString(int numberOfBytes);

  /**
   * I will return a string with specified max length. If there is a 0x00, in the byte array
   * I will stop reading characters.
   */
  std::string readTerminatedString(int maxLength);

  /**
   * Read numberOfBytes and creates a _reference_ to the buffer data.
   * Be carefull!
   */
  ByteSpan readByteSpan(int numberOfBytes);

  /**
   * The data wrapped by the span will be overwritten with the data I wrap.
   * Length is dictated by the given span.
   */
  void copyToSpan(ByteSpan destination);

  /**
   * Read one byte and return it as a bool. 0 is false, any other value is true.
   */
  bool readBoolFromByte();

  /**
   * read one byte into a uint8_t
   */
  std::uint8_t readUint8();

  /**
   * read two bytes to a uint16
   */
  std::uint16_t readUint16();

  /**
   * I read 4 bytes of data into a native uint32.
   */
  std::uint32_t readUint32();

  static constexpr bool isNativeLittleEndian() {
    return std::endian::native == std::endian::little;
  }

  std::size_t bytesLeft();

private:
  ByteSpan data;
  int index{0};
};
