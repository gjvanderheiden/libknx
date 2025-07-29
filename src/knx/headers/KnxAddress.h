#pragma once

#include "knx/bytes/ByteBufferWriter.h"
#include "knx/bytes/ByteBufferReader.h"
#include <cstdint>
#include <iostream>

class KnxAddress {
public:
  KnxAddress() = default;
  KnxAddress(std::uint8_t high, std::uint8_t middle, std::uint8_t low);
  virtual ~KnxAddress() = default;
  std::uint8_t getHigh() const;
  std::uint8_t getMiddle() const;
  std::uint8_t getLow() const;
private:
  std::uint8_t high{0}, middle{0}, low{0};
};

class IndividualAddress final : public  KnxAddress {
public:
  IndividualAddress() = default;
  IndividualAddress(std::uint8_t high, std::uint8_t middle, std::uint8_t low);
  ~IndividualAddress() override = default;

  static IndividualAddress parse(ByteBufferReader& bytebuffer);
  void write(ByteBufferWriter& writer) const;

  std::uint8_t getArea() const;
  std::uint8_t getLine() const;
  std::uint8_t getDevice() const;
};

class GroupAddress final : public KnxAddress {
public:
  GroupAddress() = default;
  GroupAddress(std::uint8_t high, std::uint8_t middle, std::uint8_t low);
  ~GroupAddress() override = default;

  static GroupAddress parse(ByteBufferReader& reader);
  void toBytes(ByteBufferWriter& writer) const;

  bool operator==(const GroupAddress& otherGroupAddress) const;
};


std::ostream& operator<<(std::ostream &stream, const GroupAddress& address);
std::ostream& operator<<(std::ostream &stream, const IndividualAddress& address);
