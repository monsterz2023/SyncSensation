#ifndef ASYNC_TCP_CLIENT_HPP
#define ASYNC_TCP_CLIENT_HPP

#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <string>
#include <nlohmann/json.hpp>

using boost::asio::ip::tcp;
namespace asio=boost::asio;
using json = nlohmann::json;

class AsyncTcpClient {
    public:
    AsyncTcpClient(asio::io_context& io_ctx, const std::string& host, const std::string& port);
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