#pragma once

#include <bit>
#include <bytes/ByteDef.h>
#include <string_view>
#include <vector>

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
class ByteBufferWriter {

public:
  ByteBufferWriter(std::vector<byte>& data);

  /**
   * Just skip numberOfBytes, call this when bytes are reserved / unknown or
   * you're simply no interested.
   */
  void writeZeros(int numberOfBytes);

  /**
   * I will return a string with specified number of bytes.
   */
  void writeString(std::string_view string);

  /**
   * I will return a string with specified max length. If there is a 0x00, in the byte array
   * I will stop reading characters.
   */
  void writeTerminatedString(std::string_view, int maxLength);

  /**
   * Read numberOfBytes and creates a _reference_ to the buffer data.
   * Be carefull!
   */
  void writeByteSpan(ByteSpan span);

  /**
   * Read one byte and return it as a bool. 0 is false, any other value is true.
   */
  void writeBoolAsByte(bool value);

  /**
   * read one byte into a uint8_t
   */
  void writeUint8(std::uint8_t value);

  /**
   * read two bytes to a uint16
   */
  void writeUint16(std::uint16_t value);

  /**
   * I read 4 bytes of data into a native uint32.
   */
  void writeUint32(std::uint32_t value);

  static constexpr bool isNativeLittleEndian() {
    return std::endian::native == std::endian::little;
  }

private:
  std::vector<byte>& data;
};
