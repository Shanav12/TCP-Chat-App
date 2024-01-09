#include "Network/Server/TCP_Server.h"

namespace TCPChat {
    using boost::asio::ip::tcp;
    
    TCPServer::TCPServer(IPVersion ip_version, unsigned port_number) : ip_version_(ip_version), port_number_(port_number),
        tcp_acceptor_(tcp::acceptor(io_, tcp::endpoint(ip_version_ == IPVersion::V6 ? tcp::v6() : tcp::v4(), port_number_))) {
            
    }

    int TCPServer::Run() {
        try {
            StartAccept();
            io_.run();
        } catch (std::exception& err) {
            std::cerr << err.what() << std::endl;
            return -1;
        }
        
        return 0;
    }

    void TCPServer::StartAccept() {
        socket_.emplace(io_);
        tcp_acceptor_.async_accept(*socket_, [this](const boost::system::error_code& err) {
            std::shared_ptr<TCPConnect> tcp_connect = TCPConnect::Create(std::move(*socket_));

            if (tcp_join) {
                tcp_join(tcp_connect);
            }
            
            tcp_connections_.insert(tcp_connect);
            if (!err) {
                tcp_connect->Start(
                    [this](const std::string& tcp_message) {if (tcp_client_message) {tcp_client_message(tcp_message);} }, 

                    [&, weak_ptr = std::weak_ptr(tcp_connect)](){
                    if (auto shared_ptr = weak_ptr.lock(); shared_ptr && tcp_connections_.erase(shared_ptr)) {
                        if (tcp_leave) {
                            tcp_leave(shared_ptr);
                            }   
                        }
                    }
                );
            }
            StartAccept();
        });
    }

    void TCPServer::Broadcast(const std::string& tcp_message) {
        for (auto& tcp_connect : tcp_connections_) {
            tcp_connect->Send(tcp_message);
        }
    }
}
