
#include "KnxStructure.h"
#include "bytes/ByteBuffer.h"
#include <array>

class ConnectionHeader : public KnxStructure {
public:

  std::array<std::uint8_t, 8> bodyAsByteArray();
protected:
  void parseBody(ByteBuffer& byteBuffer, std::uint16_t length) override;


private:
  std::uint8_t sequence;
  std::uint8_t status;
};

