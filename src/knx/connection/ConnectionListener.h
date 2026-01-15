#pragma once

#include "knx/cemi/Cemi.h"

namespace knx::connection {

class ConnectionListener {
public:
  ConnectionListener() = default;
  ConnectionListener(const ConnectionListener &) = delete;
  ConnectionListener(ConnectionListener &&) = delete;
  ConnectionListener &operator=(const ConnectionListener &) = delete;
  ConnectionListener &operator=(ConnectionListener &&) = delete;
  virtual ~ConnectionListener() = default;

  virtual void onConnect() = 0;
  virtual void onDisconnect() = 0;
  virtual void onIncommingCemi(Cemi &cemi) = 0;
};

} // namespace connection
