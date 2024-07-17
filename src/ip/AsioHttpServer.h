#include <asio.hpp>
#include <cstdint>
#include <functional>
#include <vector>
#include <string>

namespace http_server {

using asio::awaitable;
using asio::ip::tcp;
using HandlerFunction = std::function<void(std::vector<std::uint8_t>& data)>;
using std::chrono::steady_clock;

class AsioHttpServer {
public:
  AsioHttpServer(std::string bindHost, int port);
  void setHandler(HandlerFunction function);
  void start();

private:
  std::string bindHost;
  int port;
  std::unique_ptr<HandlerFunction> handlerFunction;

  awaitable<void> listen(tcp::acceptor &acceptor);
  awaitable<void> transfer(tcp::socket &socket,
                           steady_clock::time_point &deadline);
  awaitable<void> transferWithTimeOut(tcp::socket socket);
};

} // namespace http_server
