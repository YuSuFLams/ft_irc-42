/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 04:36:22 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/02 22:50:43 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <vector>
#include <poll.h>
#include <cstring>
#include <arpa/inet.h>
#include "Client.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fcntl.h>


//server side
// socket() -> bind() -> listen() -> accept() -> send() -> recv() -> close()
// client side
// socket() -> connect() -> send() -> recv() -> close()

int kick_command(std::vector<std::string > words , Server &server , int fd , std::string str)
{
    if(words.size() == 1)
    {
        std::string str = ":" + server.get_hostnames() + " " + server.to_string(ERR_NEEDMOREPARAMS) + " " + words[0] + " :Not enough parameters\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return (1);
    }
    else
    {
        server.KickChannel(words, server.getChannels(), fd, server.get_nickname(fd), server, str);
    }
    return (0);
}

int join_command(std::vector<std::string > words , Server &server , int fd , std::string str)
{
    if(words.size() == 1)
    {
            std::string str = ":" + server.get_hostnames() + " " + server.to_string(ERR_NEEDMOREPARAMS) + " " + words[0] + " :Not enough parameters\r\n";
            send(fd, str.c_str(), str.length(), 0);
            return (1);
    }
    else
    {
        if(server.JoinChannel(words, server.get_nickname(fd), fd , server ,str) == -1)
        {
            std::string str = ":" + server.get_hostnames() + " " + server.to_string(ERR_NOSUCHCHANNEL) + " " + server.get_nickname(fd) + " " + words[1] + " :No such channel\r\n";
            send(fd, str.c_str(), str.length(), 0);
        }
    }
    return (0);
}

int topic_command(std::vector<std::string > words , Server &server , int fd)
{
    if(words.size() == 1)
    {
        std::string str = ":" + server.get_hostnames() + " " + server.to_string(ERR_NEEDMOREPARAMS) + " " + words[0] + " :Not enough parameters\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return (1);
    }
    else
    {
        if(server.TopicChannel(words , server.getChannels(), fd, server) == -1)
        {
            std::string str = ":" + server.get_hostnames() + " " + server.to_string(ERR_NOSUCHCHANNEL) + " " + server.get_nickname(fd) + " " + words[1] + " :No such channel\r\n";
            send(fd, str.c_str(), str.length(), 0);
        }
    }
    return (0);
}

int part_command(std::vector<std::string > words , Server &server , int fd)
{
    if(words.size() == 1)
    {
        std::string str = ":" + server.get_hostnames() + " " + server.to_string(ERR_NEEDMOREPARAMS) + " " + words[0] + " :Not enough parameters\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return (1);
    }
    else
    {
        if(server.PartChannel(words , server.getChannels(), fd, server.get_nickname(fd), server) == -1)
        {
            std::string str = ":" + server.get_hostnames() + " " + server.to_string(ERR_NOSUCHCHANNEL) + " " + server.get_nickname(fd) + " " + words[1] + " :No such channel\r\n";
            send(fd, str.c_str(), str.length(), 0);
        }
    }
    return (0);
}

int quit_command(std::vector<std::string > words , Server &server , int fd)
{
    if(words[0] == "QUIT" && server.get_password(fd) != "" && server.is_registered(fd) == 1)
    {
        close(fd);
        // remove client from the channels
        server.remove_client_from_channels(fd);
        // remove client from the map
        server.removeClient(fd);
        // free the memory
        delete server.getClients()[fd];
        return (1);
    }
    return (0);
}

void ModeChannel(std::vector<std::string > words, std::map<std::string, Channel *> &channels, int fd, std::string nickname, Server &server)
{
    // if (words.size() < 3)
    // {
    //     std::string str = ":" + server.get_hostnames() + " " + server.to_string(ERR_NEEDMOREPARAMS) + " " + nickname + " MODE :Not enough parameters\r\n";
    //     send(fd, str.c_str(), str.length(), 0);
    //     return;
    // }
    if (channels.find(words[1]) == channels.end())
    {
        std::string str = ":" + server.get_hostnames() + " " + server.to_string(ERR_NOSUCHCHANNEL) + " " + nickname + " " + words[1] + " :No such channel\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return;
    }
    if (channels[words[1]]->getUsers().find(nickname) == channels[words[1]]->getUsers().end())
    {
        std::string str = ":" + server.get_hostnames() + " " + server.to_string(ERR_NOTONCHANNEL) + " " + nickname + " " + words[1] + " :You're not on that channel\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return;
    }
    if (words.size() == 3)
    {
        std::string str = ":" + server.get_hostnames() + " 324 " + nickname + " " + words[1] + " +" + words[2] + "\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return;
    }
   if (words.size() == 4)
{
    std::cout << "ModeChannel" << std::endl;
    if (words[2] == "+o")
    {
        if (channels.find(words[1]) == channels.end())
        {
            // Channel does not exist
            std::string str = ":" + server.get_hostnames() + " 403 " + nickname + " " + words[1] + " :No such channel\r\n";
            send(fd, str.c_str(), str.length(), 0);
            return;
        }

        if (!channels[words[1]]->isOperator("@" + nickname))
        {
            std::string str = ":" + server.get_hostnames() + " 482 " + nickname + " " + words[1] + " :You're not a channel operator\r\n";
            send(fd, str.c_str(), str.length(), 0);
            return;
        }

        if (channels[words[1]]->getUsers().find(words[3]) == channels[words[1]]->getUsers().end())
        {
            std::string str = ":" + server.get_hostnames() + " 441 " + nickname + " " + words[3] + " " + words[1] + " :They aren't on that channel\r\n";
            send(fd, str.c_str(), str.length(), 0);
            return;
        }
        
        channels[words[1]]->addOperator("@" + words[3]);
        
        std::set<std::string>::iterator it3 = channels[words[1]]->getUsers().begin();
        std::string str = "";
            while (it3 != channels[words[1]]->getUsers().end())
            {
            std::string user = *it3;
            int user_fd = server.get_fd_users(user);
            if (channels[words[1]]->isOperator("@" + user) == true)
                str = ":" + server.get_hostnames() + " " + server.to_string(RPL_NAMREPLY) + " " + server.get_nickname(fd) + " = " + words[1] + " :@" + user + "\r\n"; // Prefix '@' to operator's name
            else
                str = ":" + server.get_hostnames() + " " + server.to_string(RPL_NAMREPLY) + " " + server.get_nickname(fd) + " = " + words[1] + " :" + user + "\r\n";
            send(user_fd, str.c_str(), str.length(), 0);
            it3++;
        }
    }
}
        // if (words[2] == "-o")
        // {
        //     if (channels[words[1]]->isOperator("@" + nickname) == false)
        //     {
        //         std::string str = ":" + server.get_hostnames() + " 482 " + nickname + " " + words[1] + " :You're not a channel operator\r\n";
        //         send(fd, str.c_str(), str.length(), 0);
        //         return;
        //     }
        //     if (channels[words[1]]->getUsers().find(words[3]) == channels[words[1]]->getUsers().end())
        //     {
        //         std::string str = ":" + server.get_hostnames() + " 441 " + nickname + " " + words[3] + " " + words[1] + " :They aren't on that channel\r\n";
        //         send(fd, str.c_str(), str.length(), 0);
        //         return;
        //     }
        //     if (channels[words[1]]->isOperator("@" + words[3]) == false)
        //     {
        //         std::string str = ":" + server.get_hostnames() + " 441 " + nickname + " " + words[3] + " " + words[1] + " :They aren't an operator\r\n";
        //         send(fd, str.c_str(), str.length(), 0);
        //         return;
        //     }
        //     channels[words[1]]->removeOperator("@" + words[3]);
        //     std::string str = ":" + server.get_hostnames() + " 331 " + nickname + " " + words[1] + " " + words[3] + " :is no longer an operator\r\n";
        //     send(fd, str.c_str(), str.length(), 0);
        //     return;
        // }
    // }
}


int join_topic_part_part(std::vector<std::string > words, Server &server, int fd , std::string str)
{
    if(words[0] == "JOIN" && server.get_password(fd) != "" && server.is_registered(fd) == 1)
    {
        if(join_command(words, server, fd , str) == 1)
            return (1);
    }
    else if(words[0] == "TOPIC" && server.get_password(fd) != "" && server.is_registered(fd) == 1)
    {
        if(topic_command(words, server, fd) == 1)
            return (1);
    }
    else if(words[0] == "PART" && server.get_password(fd) != "" && server.is_registered(fd) == 1)
    {
        if(part_command(words, server, fd) == 1)
            return (1);
    }
    else if(words[0] == "KICK" && server.get_password(fd) != "" && server.is_registered(fd) == 1)
    {
        if(kick_command(words, server, fd, str) == 1)
            return (1);
    }
    return (0);
}



int main2(int ac, char **av)
{
    std::vector<pollfd> fds;
    if(ac != 3)
    {
        std::cout << "Usage: ./ircserv [port] [password]" << std::endl;
        return (0);
    }
    Server server;
    server.setPort(atoi(av[1]));
    // create socket file descriptor
    server.setServer_fd(socket(AF_INET, SOCK_STREAM, 0)); // 0 for TCP and 1 for UDP
    // check if socket is created
    if (server.getServer_fd() < 0)
    {
        std::cout << "Socket failed" << std::endl;
        return (0);
    }
    //scokaddr_in is a structure containing an internet address
    int opt = 1;
    sockaddr_in address;
    // set address family
    address.sin_family = AF_INET; // AF_INET is the address family for IPv4
    address.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY means any address for binding
    address.sin_port = htons(server.getPort()); // htons() converts port number to network byte order
    server.setAddress(address);
    // bind socket to address
    // non blocking
    if(setsockopt(server.getServer_fd(), SOL_SOCKET,SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        std::cout << "Setsockopt failed" << std::endl;
        return (0);
    }
    if (fcntl(server.getServer_fd(), F_SETFL, O_NONBLOCK) < 0)
    {
        std::cout << "Fcntl failed" << std::endl;
        return (0);
    }
    
    if (bind(server.getServer_fd(), (struct sockaddr *)&address, sizeof(server.getAddress())) < 0)
    {
        std::cout << "Bind failed" << std::endl;
        return (0);
    }
    // listen for connections on a socket
    if (listen(server.getServer_fd(), 5000) < 0) // 3 is the maximum size of queue connections
    {
        std::cout << "Listen failed" << std::endl;
        return (0);
    }
    
    fds.push_back(pollfd());
    fds.back().fd = server.getServer_fd();
    fds.back().events = POLLIN;

    std::cout << "Server is listening on port " << server.getPort() << std::endl;
    while (true) 
    {
    int ready = poll(&fds[0], fds.size(), -1);
    if (ready == -1) 
    {
        std::cout << "Poll failed" << std::endl;
        break;
    }

    for (size_t i = 0; i < fds.size(); ++i) 
    {
        if (fds[i].revents & POLLIN) 
        {
            if (fds[i].fd == server.getServer_fd())
            {
                // New connection
                sockaddr_in client_address;
                socklen_t client_address_size = sizeof(client_address);
                int client_fd = accept(server.getServer_fd(), (struct sockaddr *)&client_address, &client_address_size);
                if (client_fd < 0) 
                {
                    std::cout << "Accept failed" << std::endl;
                    continue;
                }
                std::cout << "New connection from " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << std::endl;
                server.addClient(client_fd, new Client());
                fds.push_back(pollfd());
                fds.back().fd = client_fd;
                fds.back().events = POLLIN;
            } 
            else 
            {
                char buffer[1024] = {0};
                ssize_t bytesRead = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
                if (bytesRead <= 0) 
                {
                    // Client disconnected or CTRL+C was pressed
                    std::cout << "Client disconnected" << std::endl;
                    server.remove_client_from_channels(fds[i].fd);
                    server.removeClient(fds[i].fd);
                    delete server.getClients()[fds[i].fd];
                    close(fds[i].fd);
                    
                } 
                else 
                {
                    buffer[bytesRead] = '\0';
                    std::cout << "Received: " << buffer << std::endl;
                }
                        
                        std::string str = buffer;
    
                        str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
                        str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
                        if(str.empty())
                            continue;
                        std::vector<std::string> words;
                        std::string word;
                        std::istringstream iss(str);
                        while (iss >> word)
                        {
                            words.push_back(word);
                        }
                        if(words.size() == 0)
                            continue;

                        if(words[0] == "QUIT" && server.get_password(fds[i].fd) != "" && server.is_registered(fds[i].fd) == 1)
                        {
                            std::string str = ":" + server.get_nickname(fds[i].fd) + " QUIT :Quit: " + words[1] + "\r\n";
                            for (size_t j = 1; j < fds.size(); j++) 
                            {
                                if (fds[j].fd != fds[i].fd) 
                                {
                                    send(fds[j].fd, str.c_str(), str.length(), 0);
                                }
                            }
                            close(fds[i].fd);
                            // remove client from the channels
                            server.remove_client_from_channels(fds[i].fd);
                            // remove client from the map
                            server.removeClient(fds[i].fd);
                            // free the memory
                            delete server.getClients()[fds[i].fd];
                            break;
                        }
                        
                        if(words[0] == "PASS")
                        {
                            if(str.find(" :") != std::string::npos)
                            {
                                std::string password = str.substr(str.find(" :") + 2);
                                server.set_password(fds[i].fd, password);
                            }
                            else if(words[1].empty())
                            {
                                std::string str = ":" + server.get_hostnames() + " " + server.to_string(ERR_NEEDMOREPARAMS) + " " + words[0] + " :No enough parameters\r\n";
                                send(fds[i].fd, str.c_str(), str.length(), 0);
                            }
                            else
                            {
                                server.set_password(fds[i].fd, words[1]);
                            }
                        }
                        else if(words[0] == "NICK" && server.get_password(fds[i].fd) != "")
                        {
                            if(words[1].empty())
                            {
                                std::string str = server.get_hostnames() + " 431 " + words[0] + " :No nickname given\r\n";
                                send(fds[i].fd, str.c_str(), str.length(), 0);
                            }
                            else if(words[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_[]\\`^{}") != std::string::npos || words.size() > 2)
                            {
                                std::string str = ":" + server.get_hostnames() + " 432 " + words[1] + " :Erroneous nickname\r\n";
                                send(fds[i].fd, str.c_str(), str.length(), 0);
                                words.clear();
                                continue;
                            }
                            else
                            {
                                server.set_nickname(fds[i].fd, words[1]);
                            }
                        }
                        
                        else if(words[0] == "USER" && server.get_password(fds[i].fd) != "")
                        {
                            if(words[1].empty() || words[2].empty() || words[3].empty() || words[4].empty())
                            {
                                std::string str = ":" + server.get_hostnames()  + " " + server.to_string(ERR_NEEDMOREPARAMS) + " " + words[0]  + " :Not enough parameters\r\n";
                                send(fds[i].fd, str.c_str(), str.length(), 0);
                            }
                            else
                            {
                                server.set_username(fds[i].fd, words[1]);
                                server.set_hostname(fds[i].fd, words[2]);
                                server.set_servername(fds[i].fd, words[3]);
                                if (words[4].find(":") == std::string::npos)
                                {
                                    if (words.size() > 5) 
                                    {
                                        std::string str = ":" + server.get_hostnames() + " " + server.to_string(ERR_NEEDMOREPARAMS) + " " + words[0] + " :Not enough parameters\r\n";
                                        send(fds[i].fd, str.c_str(), str.length(), 0);
                                        words.clear();
                                        server.set_username(fds[i].fd, "");
                                        server.set_hostname(fds[i].fd, "");
                                        server.set_servername(fds[i].fd, "");
                                        continue;
                                    } 
                                    else 
                                    {
                                        std::string realname = words[4];
                                        server.set_realname(fds[i].fd, realname);
                                    }
                                } 
                                else 
                                {
                                    std::string realname = words[4].substr(words[4].find(":") + 1);
                                    server.set_realname(fds[i].fd, realname);
                                }
                            }

                            
                            if (server.is_nickname_exist_and_registered(server.get_nickname(fds[i].fd)) == 1 && server.is_registered(fds[i].fd) == 0)
                            {
                                std::string str = ":" +  server.get_hostnames() + " 433 " + server.get_nickname(fds[i].fd) + " :Nickname is already in use\r\n";
                                send(fds[i].fd, str.c_str(), str.length(), 0);
                                words.clear();
                                continue;
                            }
                        }
                        
                        if(join_topic_part_part(words, server, fds[i].fd, str) == 1)
                            continue;
                        if(quit_command(words, server, fds[i].fd) == 1)
                            break;

                        else if (words[0] == "INVITE")
                        {
                            server.invitecmd(words, server, fds[i].fd);
                        }
                        else if (words[0] == "MODE")
                        {
                            server.modecmd(words, server, fds[i].fd);
                        }
                        
                        
                        if(!server.get_password(fds[i].fd).empty() && !server.get_hostname(fds[i].fd).empty() && !server.get_username(fds[i].fd).empty() 
                            && !server.get_servername(fds[i].fd).empty() && !server.get_nickname(fds[i].fd).empty() && server.is_registered(fds[i].fd) == 0)
                        {
                            if( av[2] != server.get_password(fds[i].fd))
                            {
                                std::string str = ":"  + server.get_hostnames() + " 464 " + server.get_password(fds[i].fd) + " :Password incorrect\r\n";
                                send(fds[i].fd, str.c_str(), str.length(), 0);
                                close(fds[i].fd);
                                server.removeClient(fds[i].fd);
                                delete server.getClients()[fds[i].fd];
                            }
                            else
                            {
                                server.set_is_registered(fds[i].fd, 1);
                                std::string str = ":" + server.get_hostnames() +  " 001 " + server.get_nickname(fds[i].fd) + " :Welcome to the Internet Relay Network " 
                                    + server.get_username(fds[i].fd) + "!" + server.get_hostname(fds[i].fd) + "@" + server.get_servername(fds[i].fd) + "\r\n";
                                send(fds[i].fd, str.c_str(), str.length(), 0);
                                str = ":" + server.get_hostnames() +  " 002 " + server.get_nickname(fds[i].fd) + " :Your host is " + server.get_hostnames() + ", running version 1.0\r\n";
                                send(fds[i].fd, str.c_str(), str.length(), 0);
                                str = ":" + server.get_hostnames() +  " 003 " + server.get_nickname(fds[i].fd) + " :This server was created " + server.get_current_time() + "\r\n";
                                send(fds[i].fd, str.c_str(), str.length(), 0);
                            }
                        }
                        words.clear();
                        continue;
                }
            }
       
        }
    }
    
    return (0);
}

int main(int ac , char **av)
{
    try
    {
        main2(ac, av);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        std::cerr << "Error: " << strerror(errno) << std::endl;
    }
    
}
