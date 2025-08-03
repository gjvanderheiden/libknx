#pragma once

#include "knx/headers/KnxAddress.h"
#include "knx/bytes/ByteBufferReader.h"
#include "knx/bytes/ByteBufferWriter.h"
#include "knx/cemi/Control.h"
#include "knx/cemi/NPDUFrame.h"
#include <cstdint>
#include <variant>

class Cemi {
public:
  static constexpr std::uint8_t L_DATA_IND = 0x29;
  static constexpr std::uint8_t L_DATA_REQ = 0x11;
  static constexpr std::uint8_t L_DATA_CON = 0x2E;
public:
  Cemi(std::uint8_t messageCode, Control &&control, IndividualAddress &&source,
       std::variant<IndividualAddress, GroupAddress> &&destination,
       NPDUFrame &&frame);

  static Cemi parse(ByteBufferReader &reader);
  void write(ByteBufferWriter &writer) const;

  std::uint8_t getMessageCode() const;
  const Control &getControl() const;
  const IndividualAddress &getSource() const;
  const std::variant<IndividualAddress, GroupAddress> &getDestination() const;
  const NPDUFrame &getNPDU() const;

private:
  const std::uint8_t messageCode{0x29};
  Control control;
  IndividualAddress source;
  std::variant<IndividualAddress, GroupAddress> destination;
  NPDUFrame npdu;
};
