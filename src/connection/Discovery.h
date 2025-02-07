#pragma once

#include <string>
#include <vector>
namespace connection {

struct KnxIp{
  std::string name;
  std::string ip;
};

class Discovery {
public:
  static std::vector<KnxIp> lookAround();
};

}
