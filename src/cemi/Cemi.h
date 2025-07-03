#pragma once

#include "KnxAddress.h"
#include "bytes/ByteBufferReader.h"
#include "cemi/Control.h"
#include "cemi/NPDUFrame.h"
#include <variant>

class Cemi {
public:
  Cemi(std::uint8_t messageCode, Control&& control, IndividualAddress&& source, std::variant<IndividualAddress, GroupAddress>&& destination, NPDUFrame&& frame);
  static Cemi parse(ByteBufferReader &byteBuffer);
  std::uint8_t getMessageCode() const;
  Control& getControl();
  IndividualAddress& getSource();
  std::variant<IndividualAddress, GroupAddress>& getDestination();
  NPDUFrame& getNPDU();

private:
  std::uint8_t messageCode;
  Control control;
  IndividualAddress source;
  std::variant<IndividualAddress, GroupAddress> destination;
  NPDUFrame npdu;
};
