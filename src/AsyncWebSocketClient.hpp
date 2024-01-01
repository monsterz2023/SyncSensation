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
namespace websocket=boost::beast::websocket;
namespace asio=boost::asio;
using tcp=boost::asio::ip::tcp;
using json = nlohmann::json;

class AsyncWebSocketClient : public std::enable_shared_from_this<AsyncWebSocketClient> {
    public:
    AsyncWebSocketClient(boost::asio::io_context& ioc, std::string host, std::string port, std::string target);
    void start();
    void on_message(std::function<void(beast::multi_buffer&)> message_cb);

    private:
    tcp::resolver resolver_;
    websocket::stream<tcp::socket> ws_;
    std::string host_;
    std::string port_;
    std::string target_;
    beast::multi_buffer buffer_;
    std::function<void(beast::multi_buffer&)> message_cb_;

    void do_connect(const tcp::resolver::results_type& endpoints);
    void do_handshake();
    void do_read();
};

#endif