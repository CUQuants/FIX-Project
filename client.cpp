#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <thread>

void receiveMessages(int sock) {
    char buffer[1024] = {0};

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(sock, buffer, 1024);
        if (valread <= 0) {
            break;
        }

        std::cout << "\r" << std::string(80, ' ') << "\r";
        std::cout << buffer << std::endl;
        std::cout << "You: " << std::flush;
    }
}

int main()
{
    struct sockaddr_in serv_addr;
    int sock = 0;
    char buffer[1024] = {0};
    std::string hello = "Hello from client";

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "\n Socket creation error \n";
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        std::cout << "\nInvalid address/ Address not supported \n";
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << "\nConnection Failed \n";
        return -1;
    }


    std::thread receiveThread(receiveMessages, sock);
    receiveThread.detach();

    while (true)
    {
        std::string message;
        std::cout << "You: ";
        std::getline(std::cin, message);

        if (message == "exit")
        {
            break;
        }

        send(sock, message.c_str(), message.size(), 0);
        

    }

    return 0;
}