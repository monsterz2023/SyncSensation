#include "AsyncTcpClient.hpp"

AsyncTcpClient::AsyncTcpClient(asio::io_context& io_ctx, const std::string& host, const std::string& port):io_context_(io_ctx),socket_(io_context_), ping_timer_(io_context_){
    tcp::resolver resolver(io_context_);

    auto endpoints = resolver.resolve(host, port);
    asio::async_connect(socket_, endpoints, [this](const boost::system::error_code& error, tcp::endpoint /*iterator*/){
        if(!error) {
            std::cout << "Connected!" << std::endl;
            read();
        } else {
            std::cout << "Failed to connect! "<< error.message() << std::endl;
        }
    });
    startPing();
};

void AsyncTcpClient::read() {
    socket_.async_read_some(asio::buffer(buffer_), [this](const boost::system::error_code &error, std::size_t bytes_transferred){
        if(!error){
            data_.append(buffer_.data(), bytes_transferred);
            std::cout << "Content "<< data_ << std::endl;
        } else if (error == asio::error::eof || error == asio::error::connection_reset) {
            std::cout << "Connection closed by remote host." << std::endl;
            handle_disconnect();
        } else {
            std::cout << "Read failed: " << error.message() << std::endl;
        }
        read();
    });
}

void AsyncTcpClient::startPing(){
    ping_timer_.expires_after(asio::chrono::seconds(1));
    ping_timer_.async_wait([this](boost::system::error_code error){
        if(!error){
            ping();
            startPing();
        } else {
            std::cout << "Ping timer error: " << error.message() << std::endl;
        }
    });

}

void AsyncTcpClient::ping() {
    std::string empty;
    write(empty);
}

void AsyncTcpClient::write(const std::string& json_data) {
    std::vector<char> packet;

    std::uint32_t data_length = json_data.size();
    packet.resize(sizeof(data_length)+data_length);

    std::memcpy(packet.data(), &data_length, sizeof(data_length));

    if(!json_data.empty()){
        std::memcpy(packet.data() + sizeof(data_length), json_data.data(), data_length);
    }

    doWrite(packet);
}

void AsyncTcpClient::doWrite(const std::vector<char>& packet) {
    asio::async_write(socket_, asio::buffer(packet),
    [this](boost::system::error_code error, std::size_t /*length*/){
        if(!error){
            return;
        } else if (error == asio::error::eof || error == asio::error::connection_reset) {
            std::cout << "Connection closed by remote host." << std::endl;
            handle_disconnect();
        } else {
            std::cout << "Write failed: " << error.message() << std::endl;
        }
    });
}

void AsyncTcpClient::handle_disconnect(){
    ping_timer_.cancel();
    socket_.close();
}