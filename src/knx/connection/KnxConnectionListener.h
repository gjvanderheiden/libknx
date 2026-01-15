#pragma once

#include "knx/headers/KnxAddress.h"
#include <cstdint>

namespace knx::connection {

class KnxConnectionListener {
public:
  KnxConnectionListener(const KnxConnectionListener &) = delete;
  KnxConnectionListener(KnxConnectionListener &&) = delete;
  KnxConnectionListener &operator=(const KnxConnectionListener &) = delete;
  KnxConnectionListener &operator=(KnxConnectionListener &&) = delete;

  KnxConnectionListener() = default;
  virtual ~KnxConnectionListener() = default; 

  virtual void onConnect() = 0;
  virtual void onDisconnect() = 0;

  virtual void onGroupRead(const IndividualAddress &source,
                           const GroupAddress &groupAddress) = 0;

  virtual void onGroupReadResponse(const IndividualAddress &source,
                                   const GroupAddress &groupAddress,
                                   std::span<const std::uint8_t> data) = 0;

  virtual void onGroupWrite(const IndividualAddress &source,
                            const GroupAddress &groupAddress,
                            std::span<const std::uint8_t> data) = 0;
};

} // namespace knx::connection
