#include "Network/Server/TCP_Connect.h"

namespace TCPChat {
    TCPConnect::TCPConnect(boost::asio::ip::tcp::socket&& socket) : tcp_socket_(std::move(socket)) {
        boost::system::error_code err;
        std::stringstream curr_name;
        curr_name << tcp_socket_.remote_endpoint();
        username_ = curr_name.str();
    }

    tcp::socket& TCPConnect::Socket() {   return tcp_socket_ ; }

    void TCPConnect::Send(const std::string& message) {
        bool queue_empty = messages_.empty();
        messages_.push(message);

        if (queue_empty) {
            AsyncWrite();
        }
    }

    std::string TCPConnect::GetUsername() const { return username_; }

    void TCPConnect::Start(std::function<void(std::string)>&& tcp_message_handler, std::function<void()>&& tcp_error_handler) {
        tcp_message_handler_ = std::move(tcp_message_handler);
        tcp_error_handler_ = std::move(tcp_error_handler);
        AsyncRead();
    }

    std::shared_ptr<TCPConnect> TCPConnect::Create(boost::asio::ip::tcp::socket&& socket) {
        return std::shared_ptr<TCPConnect>(new TCPConnect(std::move(socket)));
    }

    void TCPConnect::AsyncRead() {
        boost::asio::async_read_until(tcp_socket_, stream_buffer_, '\n', [self = shared_from_this()]
        (boost::system::error_code err, unsigned bytes) {
            self->OnRead(err, bytes);
        });
    }

    void TCPConnect::OnRead(boost::system::error_code err, unsigned bytes) {
        if (err) {
            tcp_socket_.close(err);
            tcp_error_handler_();
            return;
        }

        std::stringstream curr_message;
        curr_message << username_ << ": " << std::istream(&stream_buffer_).rdbuf();
        stream_buffer_.consume(bytes);
        
        tcp_message_handler_(curr_message.str());
        AsyncRead();
    }

    void TCPConnect::AsyncWrite() {
        boost::asio::async_write(tcp_socket_, boost::asio::buffer(messages_.front()), [self = shared_from_this()]
        (boost::system::error_code err, unsigned bytes) {
            self->OnWrite(err, bytes);
        });
    }

    void TCPConnect::OnWrite(boost::system::error_code err, unsigned bytes) {
        if (err) {
            tcp_socket_.close(err);
            tcp_error_handler_();
            return;
        }

        messages_.pop();

        if (!messages_.empty()) {
            AsyncWrite();
        }
    }
}