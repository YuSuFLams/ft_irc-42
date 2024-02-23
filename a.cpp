#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>
#include <poll.h>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    std::vector<pollfd> fds;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        return 1;
    }

    std::cout << "Server is listening on port 8080" << std::endl;

    fds.push_back(
        {server_fd, POLLIN, 0}
    )

    while (true) {
        int ready = poll(fds.data(), fds.size(), -1);
        if (ready == -1) {
            perror("poll failed");
            break;
        }

        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_fd) {
                    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                        perror("accept");
                        continue;
                    }
                    std::cout << "New connection from " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << std::endl;
                    fds.push_back({new_socket, POLLIN, 0});
                } else {
                    char buffer[1024] = {0};
                    ssize_t bytesRead = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
                    if (bytesRead <= 0) {
                        std::cout << "Client disconnected" << std::endl;
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        --i; // Adjust index after erase
                    } else {
                        buffer[bytesRead] = '\0';
                        std::cout << "Received: " << buffer << std::endl;
                    }
                }
            }
        }
    }

    return 0;
}
