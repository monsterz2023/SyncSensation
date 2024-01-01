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

class AsyncTcpClient : public std::enable_shared_from_this<AsyncTcpClient> {
    public:
    AsyncTcpClient(asio::io_context& ioc, std::string host, std::string port);
    void start();

    private:
    asio::ip::tcp::resolver resolver_;
    std::string host_;
    std::string port_;

    tcp::socket socket_;
    boost::array<char, 128> buffer_;
    std::string data_;
    asio::steady_timer ping_timer_;

    void read();
    void write(const std::string &data);
    void do_write(const std::vector<char>& package);
    void ping();
    void start_ping();
    void handle_disconnect();
};

#endif