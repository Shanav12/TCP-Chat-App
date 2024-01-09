#include "Network/Client/TCP_Client.h"

namespace TCPChat {
    TCPClient::TCPClient(const std::string& addr, unsigned port) : tcp_socket_(io_) {
        boost::asio::ip::tcp::resolver tcp_resolver(io_);
        tcp_endpoints_ = tcp_resolver.resolve(addr, std::to_string(port));
    }

    void TCPClient::Run() {
        boost::asio::async_connect(tcp_socket_, tcp_endpoints_, 
        [this](boost::system::error_code err, boost::asio::ip::tcp::endpoint endp) {
            if (!err) {
                AsyncRead();
            }
        });

        io_.run();
    }

    void TCPClient::Kill() {
        boost::system::error_code err;
        tcp_socket_.close(err);

        if (err) {
            std::cout << err.message() << std::endl;
        }
    }

    void TCPClient::Send(const std::string &message) {
        bool queue_empty = tcp_messages_.empty();
        tcp_messages_.push(message);

        if (queue_empty) {
            AsyncWrite();
        }
    }

    void TCPClient::AsyncRead() {
        boost::asio::async_read_until(tcp_socket_, stream_buffer_, '\n', [this](boost::system::error_code err, unsigned bytes){
            OnRead(err, bytes);
        });
    }

    void TCPClient::OnRead(boost::system::error_code err, unsigned bytes) {
        if (err) {
            Kill();
            return;
        }
        
        std::stringstream tcp_message;
        tcp_message << std::istream(&stream_buffer_).rdbuf();
        tcp_message_handler(tcp_message.str());
        AsyncRead();
    }

    void TCPClient::AsyncWrite() {
        boost::asio::async_write(tcp_socket_, boost::asio::buffer(tcp_messages_.front()), 
            [this](boost::system::error_code err, unsigned bytes) {
                OnWrite(err, bytes);
        });
    }

    void TCPClient::OnWrite(boost::system::error_code err, unsigned bytes) {
        if (err) {
            Kill();
            return;
        }

        tcp_messages_.pop();
        if (!tcp_messages_.empty()) {
            AsyncWrite();
        }
    }
}
