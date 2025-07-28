#include "SearchRequest.h"
#include "HPAI.h"
#include "IpAddress.h"
#include "ByteBufferWriter.h"

SearchRequest::SearchRequest(HPAI hpaiLocal) : hpaiLocal(hpaiLocal) {}

std::vector<std::uint8_t> SearchRequest::toBytes() {
  std::vector<std::uint8_t> result;
  ByteBufferWriter writer{result};
  appendToByteWriter(writer);
  return result;
}
void SearchRequest::appendToByteWriter(ByteBufferWriter& writer) {
  writer.writeUint8(0x06);
  writer.writeUint8(0x10);
  writer.writeUint16(SERVICE_ID);
  writer.writeUint8(0x00);
  writer.writeUint8(14);
  hpaiLocal.write(writer);
}

SearchRequest SearchRequest::newDefault() {
  IpAddress multicast{244, 0, 23, 12};
  return SearchRequest(HPAI(multicast, 5371, HPAI::UDP));
}
