#pragma once

#include "bytes/ByteBuffer.h"
#include <cstdint>

class KnxAddress {
public:
  KnxAddress() = default;
  KnxAddress(std::uint8_t high, std::uint8_t middle, std::uint8_t low);
  virtual ~KnxAddress() = default;
  std::uint8_t getHigh();
  std::uint8_t getMiddle();
  std::uint8_t getLow();
private:
  std::uint8_t high{0}, middle{0}, low{0};
};

class IndividualAddress final : public  KnxAddress {
public:
  IndividualAddress() = default;
  IndividualAddress(std::uint8_t high, std::uint8_t middle, std::uint8_t low);
  ~IndividualAddress() override = default;
  static IndividualAddress createAndParse(ByteBuffer& bytebuffer);
  std::uint8_t getArea();
  std::uint8_t getLine();
  std::uint8_t getDevice();
};

class GroupAddress final : public KnxAddress{
public:
  GroupAddress() = default;
  GroupAddress(std::uint8_t high, std::uint8_t middle, std::uint8_t low);
  ~GroupAddress() override = default;
  static GroupAddress createAndParse(ByteBuffer& bytebuffer);
};
