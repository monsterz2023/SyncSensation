#ifndef ASYNC_WEBSOCKET_CLIENT_HPP
#define ASYNC_WEBSOCKET_CLIENT_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

namespace beast=boost::beast;
namespace asio=boost::asio;
namespace beast=boost::beast;
using json = nlohmann::json;

class AsyncWebSocketClient {
    public:
    AsyncWebSocketClient(asio::io_context& io_ctx, const std::string& host, const std::string& port);
    void read();
    void write(const std::string &data);

    private:
    asio::io_context& io_context_;
    tcp::socket socket_;
    boost::array<char, 128> buffer_;
    std::string data_;
    asio::steady_timer ping_timer_;

    void doWrite(const std::vector<char>& package);
    void ping();
    void startPing();
    void handle_disconnect();
};

#endif