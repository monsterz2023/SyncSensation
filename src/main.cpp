// #include <random>
// #include <iostream>
#include <boost/asio.hpp>
#include "AsyncTcpClient.hpp"

namespace asio = boost::asio;
// class Util {
//     public:
//     static int randomNumber(int n, int r) {
//         std::random_device rd;
//         std::default_random_engine eng(rd());

//         int lower = n - r;
//         int upper = n + r;

//         std::uniform_int_distribution<int> distr(lower, upper);

//         return distr(eng);
//     }
// };

// void timer_handler(const boost::system::error_code &err, boost::asio::steady_timer &t, int &count, const int &total) {
//     if (count < total) {
//         ++count;
//         std::cout << "Hello world! the timer is done!" << count << "total is " << total << std::endl;
//         t.expires_at(t.expiry() + boost::asio::chrono::milliseconds(100));
//         t.async_wait([&t, &count, &total](const boost::system::error_code& code){
//             timer_handler(code, t, count, total);
//         });
//     }
    
// }

int main(){
    asio::io_context io;
    AsyncTcpClient client1(io, "192.168.68.104", "23554");
    // boost::asio::steady_timer t(io, boost::asio::chrono::milliseconds(10));

    // int count = 0;
    // int total = 10;
    // t.async_wait([&t, &count, &total](const boost::system::error_code& code){
    //     timer_handler(code, t, count, total);
    // });
    // t.wait();
    // auto r = Util::randomNumber(50, 2);
    // std::cout << r << std::endl;

    io.run();
    return 0;
}