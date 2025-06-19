#pragma once


#include <cstdint>

class AbstractDataACPI {
};

class Abstract10bitDataACPI : public AbstractDataACPI {
  const std::uint8_t GROUP_VALUE_READ = 0b0000;
  const std::uint8_t INDIVIDUAL_ADDRESS_READ = 0b0100;
  const std::uint8_t INDIVIDUAL_ADDRESS_WRITE = 0b0101;
};


class Abstract4bitDataACPI : public AbstractDataACPI {
  const std::uint8_t GROUP_VALUE_RESPONSE = 0b0001;
  const std::uint8_t GROUP_VALUE_WRITE = 0b0010;
};

class ControlACPI : public AbstractDataACPI {
private:
  bool isConnect;
  bool isDisconnect;
  bool isNack;
  bool isAck;
};
