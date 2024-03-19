#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <thread>
#include <map>

std::map<int, int> client_sockets;
int next_id = 1;

void handle_client(int client_socket)
{
    char buffer[1024] = {0};
    int client_id = client_sockets[client_socket];

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(client_socket, buffer, 1024);
        if (valread <= 0)
        {
            break;
        }

        if (strncmp(buffer, "-p ", 3) == 0)
        {
            std::string command(buffer);
            size_t spacePos = command.find(' ', 3);
            if (spacePos != std::string::npos)
            {
                std::string recieverID = command.substr(3, spacePos - 3);
                std::string message = command.substr(spacePos+1);
                for (auto &pair : client_sockets)
                {
                    int other_socket = pair.first;
                    int other_client_id = pair.second;
                    if (other_client_id == stoi(recieverID)) 
                    {
                        send(other_socket, message.c_str(), message.size(), 0);
                        break; 
                    }
                }
            }
        }
        else
        {
            // Prepend the message with the sender's ID
            std::string message = "Client " + std::to_string(client_id) + ": " + buffer;

            // Relay the message to all other clients
            for (auto &pair : client_sockets)
            {
                int other_socket = pair.first;
                if (other_socket != client_socket)
                {
                    send(other_socket, message.c_str(), message.size(), 0);
                }
            }
        }
    }

    close(client_socket);
}

int main()
{
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        int new_socket;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        client_sockets[new_socket] = next_id++;
        std::thread client_thread(handle_client, new_socket);
        client_thread.detach();
    }

    return 0;
}