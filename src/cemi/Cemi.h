#pragma once

#include "KnxAddress.h"
#include "bytes/ByteBufferWriter.h"
#include "bytes/ByteBufferReader.h"
#include "cemi/Control.h"
#include "cemi/NPDUFrame.h"
#include <variant>

class Cemi {
public:
  Cemi() = delete;
  Cemi(std::uint8_t messageCode, Control&& control, IndividualAddress&& source, std::variant<IndividualAddress, GroupAddress>&& destination, NPDUFrame&& frame);
  static Cemi parse(ByteBufferReader &byteBuffer);
  void toBytes(ByteBufferWriter& byteBuffer);
  std::uint8_t getMessageCode() const;
  const Control& getControl() const;
  const IndividualAddress& getSource() const;
  const std::variant<IndividualAddress,GroupAddress>& getDestination() const;
  const NPDUFrame& getNPDU() const;

private:
  std::uint8_t messageCode;
  Control control;
  IndividualAddress source;
  std::variant<IndividualAddress, GroupAddress> destination;
  NPDUFrame npdu;
};
