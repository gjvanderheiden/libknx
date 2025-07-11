#include "SearchRequest.h"
#include "HPAI.h"
#include "IpAddress.h"
#include "bytes/ByteBufferWriter.h"

SearchRequest::SearchRequest(HPAI hpaiLocal) : hpaiLocal(hpaiLocal) {}

void SearchRequest::appendToByteWriter(ByteBufferWriter& writer) {
  writer.writeUint8(0x06);
  writer.writeUint8(0x10);
  writer.writeUint16(SERVICE_ID);
  writer.writeUint8(0x00);
  writer.writeUint8(14);
  hpaiLocal.appendToByteArray(writer);
}

SearchRequest SearchRequest::newDefault() {
  IpAddress multicast{};
  multicast.address = {244, 0, 23, 12};
  return SearchRequest(HPAI(IpAddress{}, 5371, HPAI::UDP));
}
