#include <sys/socket.h>     // For socket functions
#include <arpa/inet.h>      // For inet_pton function
#include <unistd.h>         // For read and write functions
#include <cstring>          // For string manipulation functions
#include <iostream>         // For input/output operations

int main() {
    struct sockaddr_in serv_addr;   //
    int sock = 0;
    char buffer[1024] = {0};
    std::string hello = "Hello from client";

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "\n Socket creation error \n";
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cout << "\nInvalid address/ Address not supported \n";
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "\nConnection Failed \n";
        return -1;
    }

    while( true ) {
        std::string message;
        std::cout << "Enter message (to exit type 'exit'): ";
        std::getline(std::cin, message);

        if (message == "exit") {
            break;
        }

        send(sock, message.c_str(), message.size(), 0);
        std::cout << "Message sent\n";

        read(sock, buffer, 1024);
        std::cout << buffer << std::endl;
    }
   
    return 0;
}