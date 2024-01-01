#include "AsyncTcpClient.hpp"

AsyncTcpClient::AsyncTcpClient(asio::io_context& ioc, std::string host, std::string port):resolver_(ioc),socket_(ioc), ping_timer_(ioc),
host_(std::move(host)), port_(std::move(port)){}

void AsyncTcpClient::start(){
    resolver_.async_resolve(
        host_, port_,
        [this](boost::system::error_code ec, tcp::resolver::results_type results) {
            if (!ec) {
                asio::async_connect(socket_, results,
                    [this](const boost::system::error_code& error, tcp::endpoint /*iterator*/){
                        if(!error) {
                            read();
                        } else {
                            std::cout << "Failed to connect! "<< error.message() << std::endl;
                        }
                });
                start_ping();
            } else {
                std::cerr << "Resolve failed: " << ec.message() << std::endl;
            }
        }
    );
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

void AsyncTcpClient::start_ping(){
    ping_timer_.expires_after(asio::chrono::seconds(1));
    ping_timer_.async_wait([this](boost::system::error_code error){
        if(!error){
            ping();
            start_ping();
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

    do_write(packet);
}

void AsyncTcpClient::do_write(const std::vector<char>& packet) {
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