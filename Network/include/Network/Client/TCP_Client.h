#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <queue>
#include <iostream>
#include <string>

namespace TCPChat {
    class TCPClient {
        public:
            TCPClient(const std::string& addr, unsigned port);
            void Run();
            void Kill();
            void Send(const std::string& message); 
            std::function<void(std::string)> tcp_message_handler;
        private:
            void AsyncRead();
            void OnRead(boost::system::error_code err, unsigned bytes);
            void AsyncWrite();
            void OnWrite(boost::system::error_code err, unsigned bytes);
            boost::asio::io_context io_{};
            boost::asio::ip::tcp::socket tcp_socket_;
            boost::asio::ip::tcp::resolver::results_type tcp_endpoints_;
            boost::asio::streambuf stream_buffer_{65536};
            std::queue<std::string> tcp_messages_{};
    };
}