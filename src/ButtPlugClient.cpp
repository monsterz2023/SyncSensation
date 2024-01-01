
#include <nlohmann/json.hpp>
#include "ButtPlugClient.hpp"

ButtPlugClient::ButtPlugClient(boost::asio::io_context& ioc, std::string host, std::string port){
    ws_ = std::make_shared<AsyncWebSocketClient>(ioc, host, port, "/");
    ws_ -> on_message([](boost::beast::multi_buffer& buffer){
        std::cout << "<<" << boost::beast::make_printable(buffer.data()) << std::endl;
    });
    ws_ -> start([this](){
        connect();
    });
}

void ButtPlugClient::connect(){
    nlohmann::json requestServerInfo = {
        {"Id", 1},
        {"ClientName", "Test Client"},
        {"MessageVersion", 1}
    };
    nlohmann::json outer = {
        {"RequestServerInfo", requestServerInfo}
    };

    // Create a JSON array and add the object to it
    nlohmann::json request = nlohmann::json::array();
    request.push_back(outer);

    std::cout << request.dump() << std::endl;
    ws_ -> write(request.dump());
}