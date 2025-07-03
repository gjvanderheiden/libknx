
#include "KnxStructure.h"
#include "bytes/ByteBufferReader.h"
#include <array>

class ConnectionHeader : public KnxStructure {
public:

  std::array<std::uint8_t, 8> bodyAsByteArray();
protected:
  void parseBody(ByteBufferReader& byteBuffer, std::uint16_t length) override;


private:
  std::uint8_t sequence;
  std::uint8_t status;
};

