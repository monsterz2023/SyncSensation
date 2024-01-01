#include "AsyncWebSocketClient.hpp"

AsyncWebSocketClient::AsyncWebSocketClient(boost::asio::io_context &ioc, std::string host, std::string port, std::string target) : resolver_(ioc), ws_(ioc), host_(std::move(host)), port_(std::move(port)), target_(std::move(target))
{
}

void AsyncWebSocketClient::start(std::function<void()> handshake_cb)
{
    resolver_.async_resolve(
        host_, port_,
        [this, handshake_cb](boost::system::error_code ec, tcp::resolver::results_type results) {
            if (!ec) {
                do_connect(results, handshake_cb);
            } else {
                std::cerr << "Resolve failed: " << ec.message() << std::endl;
            }
    });
}


void AsyncWebSocketClient::do_connect(const tcp::resolver::results_type &endpoints, std::function<void()> handshake_cb) {
    boost::asio::async_connect(
        ws_.next_layer(), endpoints.begin(), endpoints.end(),
        [this,handshake_cb](boost::system::error_code ec, tcp::resolver::iterator) {
            if (!ec) {
                do_handshake(handshake_cb);
            } else {
                std::cerr << "Connect failed: " << ec.message() << std::endl;
            }
        });
}


void AsyncWebSocketClient::do_handshake(std::function<void()> handshake_cb) {
    ws_.async_handshake(host_, target_,
        [this, handshake_cb](boost::system::error_code ec) {
            if (!ec) {
                std::cout << "hand shake done" << std::endl;
                handshake_cb();
                do_read();
            } else {
                std::cerr << "Handshake failed: " << ec.message() << std::endl;
            }
        });
}

void AsyncWebSocketClient::on_message(std::function<void(beast::multi_buffer &)> func)
{
    message_cb_ = func;
}

void AsyncWebSocketClient::do_read() {
    ws_.async_read(
        buffer_,
        [this, self=shared_from_this()](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                if (message_cb_) {
                    message_cb_(buffer_);
                } else {
                    std::cout << boost::beast::make_printable(buffer_.data()) << std::endl;
                }
                buffer_.consume(buffer_.size());
                do_read();
            } else {
                ws_.async_close(websocket::close_code::normal,
                    [this, self](boost::system::error_code ec) {
                        if (ec) {
                            std::cerr << "Close failed: " << ec.message() << std::endl;
                        }
                    });
            }
        });
}

    void AsyncWebSocketClient::write(const std::string &data){
        ws_.async_write(asio::buffer(data),
        [this](boost::system::error_code ec, std::size_t){
            if(ec){
                std::cerr << "Write failed: " << ec.message() << std::endl;
            }
        });
    }