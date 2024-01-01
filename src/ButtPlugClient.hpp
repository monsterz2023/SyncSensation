#ifndef BUTT_PLUG_CLIENT_HPP
#define BUTT_PLUG_CLIENT_HPP

#include "AsyncWebSocketClient.hpp"

class ButtPlugClient
{
public:
    ButtPlugClient(boost::asio::io_context& ioc, std::string host, std::string port);
    void connect();
    // void start_scanning();
    // void stop_scanning();

private:
    std::shared_ptr<AsyncWebSocketClient> ws_;
};

#endif