#pragma once

#include "knx/headers/KnxAddress.h"
#include <cstdint>

namespace knx::connection {

class KnxConnectionListener {
public:
  KnxConnectionListener() = default;
  virtual ~KnxConnectionListener() = default;
  virtual void onConnect() = 0;
  virtual void onDisconnect() = 0;

  virtual void onGroupRead(const IndividualAddress &source,
                           const GroupAddress &ga) = 0;

  virtual void onGroupReadResponse(const IndividualAddress &source,
                                   const GroupAddress &ga,
                                   std::span<const std::uint8_t> data) = 0;

  virtual void onGroupWrite(const IndividualAddress &source,
                            const GroupAddress &ga,
                            std::span<const std::uint8_t> data) = 0;
};

} // namespace knx::connection
