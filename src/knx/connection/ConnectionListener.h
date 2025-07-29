#include "knx/cemi/Cemi.h"

namespace knx::connection {

class ConnectionListener {
public:
  ConnectionListener() = default;
  virtual ~ConnectionListener() = default;
  virtual void onConnect() = 0;
  virtual void onDisconnect() = 0;
  virtual void onIncommingCemi(Cemi &cemi) = 0;
};

} // namespace connection
