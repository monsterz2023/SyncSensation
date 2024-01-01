#include <boost/asio.hpp>
#include "AsyncTcpClient.hpp"
#include "AsyncWebSocketClient.hpp"

namespace asio = boost::asio;

int main(){
    asio::io_context io;
    auto s = std::make_shared<AsyncTcpClient>(io, "192.168.68.104","23554");
    s -> start();

    auto ws = std::make_shared<AsyncWebSocketClient>(io,"localhost","8080","/ofs");
    ws -> start();
    ws -> on_message([](boost::beast::multi_buffer& buffer){
        std::cout << "<<" << boost::beast::make_printable(buffer.data()) << std::endl;
    });

    io.run();
    return EXIT_SUCCESS;
}