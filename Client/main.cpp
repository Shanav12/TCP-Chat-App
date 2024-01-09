#include <Network/Client/TCP_Client.h>
#include <thread>


using namespace TCPChat;
int main(int argc, char* argv[]) {
    TCPClient tcp_client{"localhost", 1026};

    tcp_client.tcp_message_handler = [](const std::string& tcp_message){ std::cout << tcp_message; };

    std::thread client_thread([&tcp_client](){ tcp_client.Run(); });

    while (true) {
        std::string tcp_message;
        getline(std::cin, tcp_message);
        if (tcp_message == "\\q") {
            break;
        }
        tcp_message += "\n";

        tcp_client.Send(tcp_message);
    }

    tcp_client.Kill();
    client_thread.join();

    return 0;
}
