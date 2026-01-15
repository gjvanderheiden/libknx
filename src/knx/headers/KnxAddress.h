#pragma once

#include "knx/bytes/ByteBufferWriter.h"
#include "knx/bytes/ByteBufferReader.h"
#include <cstdint>
#include <iostream>

class KnxAddress {
protected:
  KnxAddress() = default;
  KnxAddress(std::uint8_t high, std::uint8_t middle, std::uint8_t low);
  KnxAddress(const KnxAddress &) = default;
  KnxAddress(KnxAddress &&) = default;
  KnxAddress &operator=(const KnxAddress &) = default;
  KnxAddress &operator=(KnxAddress &&) = default;
public:
  virtual ~KnxAddress() = default;
  [[nodiscard]] std::uint8_t getHigh() const;
  [[nodiscard]] std::uint8_t getMiddle() const;
  [[nodiscard]] std::uint8_t getLow() const;
  bool operator==(const KnxAddress& otherGroupAddress) const = default;
  bool operator<(const KnxAddress& otherGroupAddress) const;
  bool operator>(const KnxAddress& otherGroupAddress) const;
private:
  std::uint8_t high{0}, middle{0}, low{0};
};

class IndividualAddress final : public  KnxAddress {
public:
  IndividualAddress() = default;
  IndividualAddress(std::uint8_t high, std::uint8_t middle, std::uint8_t low);
  ~IndividualAddress() override = default;
  IndividualAddress(const IndividualAddress& addres) = default;
  IndividualAddress(IndividualAddress&& addres) noexcept = default;
  IndividualAddress& operator=(const IndividualAddress& other) = default;
  IndividualAddress& operator=(IndividualAddress&& other) noexcept = default;

  static IndividualAddress parse(ByteBufferReader& reader);
  void write(ByteBufferWriter& writer) const;

  bool operator==(const IndividualAddress& otherIndividualAddress) const = default;
  bool operator<(const IndividualAddress& otherIndividualAddress) const;
  bool operator>(const IndividualAddress& otherIndividualAddress) const;

  [[nodiscard]] std::uint8_t getArea() const;
  [[nodiscard]] std::uint8_t getLine() const;
  [[nodiscard]] std::uint8_t getDevice() const;
};

class GroupAddress final : public KnxAddress {
public:
  GroupAddress() = default;
  GroupAddress(std::uint8_t high, std::uint8_t middle, std::uint8_t low);
  ~GroupAddress() override = default;
  GroupAddress(const GroupAddress& addres) = default;
  GroupAddress(GroupAddress&& addres) noexcept = default;
  GroupAddress& operator=(const GroupAddress& other) = default;
  GroupAddress& operator=(GroupAddress&& other) noexcept = default;

  static GroupAddress parse(ByteBufferReader& reader);
  void toBytes(ByteBufferWriter& writer) const;

  bool operator==(const GroupAddress& otherGroupAddress) const = default;
  bool operator<(const GroupAddress& otherGroupAddress) const;
  bool operator>(const GroupAddress& otherGroupAddress) const;
};


std::ostream& operator<<(std::ostream &stream, const GroupAddress& address);
std::ostream& operator<<(std::ostream &stream, const IndividualAddress& address);
