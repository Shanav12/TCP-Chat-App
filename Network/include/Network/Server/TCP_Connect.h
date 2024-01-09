#pragma once
#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include <memory>
#include <queue>

namespace TCPChat {
    using boost::asio::ip::tcp;
    class TCPConnect : public std::enable_shared_from_this<TCPConnect> {
        public:
            tcp::socket& Socket();
            static std::shared_ptr<TCPConnect> Create(boost::asio::ip::tcp::socket&& socket);
            void Start(std::function<void(std::string)>&& tcp_message_handler, std::function<void()>&& tcp_error_handler);
            void Send(const std::string& message);
            std::string GetUsername() const;
        private:
            explicit TCPConnect(boost::asio::ip::tcp::socket&& socket);
            void AsyncRead();
            void OnRead(boost::system::error_code err, unsigned bytes);
            void AsyncWrite();
            void OnWrite(boost::system::error_code err, unsigned bytes);

            tcp::socket tcp_socket_;
            std::string username_;
            std::queue<std::string> messages_{};
            boost::asio::streambuf stream_buffer_{65536};
            std::function<void(std::string)> tcp_message_handler_;
            std::function<void()> tcp_error_handler_;
    };  
}