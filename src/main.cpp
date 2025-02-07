#include <iostream>
#include "Discovery.h"

int main(int argc, char *argv[]) {
  std::cout << "Does nothing yet, but stay tuned." << std::endl;
  auto routers = connection::Discovery::lookAround();
  for(auto router  : routers) {
    std::cout << "Found router " << router.name << std::endl;
  }
  return 0;
}
