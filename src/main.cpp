#include <boost/asio.hpp>
#include "AsyncTcpClient.hpp"
#include "AsyncWebSocketClient.hpp"

namespace asio = boost::asio;

int main(){
    asio::io_context io;
    // AsyncTcpClient client1(io, "192.168.68.104", "23554");
    
    auto ws = std::make_shared<AsyncWebSocketClient>(io,"localhost","8080","/ofs");
    ws -> start();

    io.run();
    return EXIT_SUCCESS;
}