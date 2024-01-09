
#include <Network/Server/TCP_Server.h>
#include <iostream>

using namespace TCPChat;

int main(int argc, char* argv[]) {

    TCPServer tcp_server{ IPVersion::V4, 1026 };

    tcp_server.tcp_join = [](std::shared_ptr<TCPConnect> server) { std::cout << "User has joined the server: " 
        << server->GetUsername() << std::endl; };

    tcp_server.tcp_leave = [](std::shared_ptr<TCPConnect> server) { std::cout << "User has left the server: " 
        << server->GetUsername() << std::endl; };

    tcp_server.tcp_client_message = [&tcp_server](const std::string& message) { tcp_server.Broadcast(message);  };

    tcp_server.Run();

    return 0;
}