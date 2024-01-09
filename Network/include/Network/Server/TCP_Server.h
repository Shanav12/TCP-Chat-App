#pragma once

#include <Network/Server/TCP_Connect.h>
#include <boost/asio.hpp>
#include <stdexcept>
#include <iostream>
#include <optional>
#include <unordered_set>
#include <functional>

namespace TCPChat {
    enum class IPVersion {  V4, V6  };

    class TCPServer {
        public:
            TCPServer(IPVersion ip_version, unsigned port_number);
            int Run();
            void Broadcast(const std::string& message);
            std::function<void(std::shared_ptr<TCPConnect>)> tcp_join;
            std::function<void(std::shared_ptr<TCPConnect>)> tcp_leave;
            std::function<void(std::string)> tcp_client_message;
        private:
            void StartAccept();
            IPVersion ip_version_;
            unsigned port_number_;
            boost::asio::io_context io_;
            boost::asio::ip::tcp::acceptor tcp_acceptor_;
            std::optional<boost::asio::ip::tcp::socket> socket_;
            std::unordered_set<std::shared_ptr<TCPConnect>> tcp_connections_;
    };
}