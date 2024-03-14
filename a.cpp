#include <iostream>
#include <map>
#include <string>
#include <csignal>
#include <cstdlib> // for atoi

class Server {
public:
    Server(const char* arg2, int arg1) : server_fd(arg1) {
        // Initialize server with arguments
    }

    static void signal_handler(int signum);

    static Server* serverInstance; // Static pointer to the server instance
    static int additionalParam; // Static variable for additional parameter

private:

    int server_fd;
};

// Define the static pointer and variable
Server* Server::serverInstance = nullptr;
int Server::additionalParam = 0;

void Server::signal_handler(int signum)
{
    // if(serverInstance != nullptr && signum == SIGINT)
    // {
    //     std::map<std::string, Channel*>::iterator it = serverInstance->channels.begin();
    //     for (; it != serverInstance->channels.end(); it++)
    //     {
    //         delete it->second;
    //     }
    //     std::map <int, Client*>::iterator it2 = serverInstance->clients.begin();
    //     for (; it2 != serverInstance->clients.end(); it2++)
    //     {
    //         close(it2->first);
    //         delete it2->second;
    //     }
    //     std::cout << "Server is shutting down with additional parameter: " << additionalParam << std::endl;
    //     close(serverInstance->server_fd);
    // }
}

int main(int argc, char *argv[]) {
    if (argc >= 4) {
        Server S(argv[3], atoi(argv[1]));
        // Assign the address of the server instance to the static pointer
        Server::serverInstance = &S;
        Server::additionalParam = atoi(argv[2]); // Assign additional parameter
        signal(SIGINT, Server::signal_handler);
        // Your server logic
    } else {
        std::cerr << "Usage: " << argv[0] << " <port> <additionalParam> <arg2>" << std::endl;
        return 1;
    }
    return 0;
}
