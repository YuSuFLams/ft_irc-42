/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:35:24 by araiteb           #+#    #+#             */
/*   Updated: 2024/01/17 13:25:56 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server(const std::string ipAdd, int port): m_ipAddress(ipAdd),m_port(port)
{
    flg = 1;
}
Server::Server(Server &sr)
{
    *this = sr;
}
Server& Server::operator=(const Server &sr)
{
    m_port = sr.m_port;
    m_socket = sr.m_socket;
    return (*this);
    
}
Server::~Server(){}
int  Server::createStocket()
{
    
    if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "socket failed" << std::endl;
        return (1);
    }
    
    socklen_t sizeS = sizeof (flg);

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &flg, sizeS))
    {
        perror("SocketOpt");
        std::cerr << "setsockopt" << std::endl;
        return (1);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->m_port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "bind failed" << std::endl;
        return (1);
    }
    
    if (listen(server_fd, 3) < 0)
    {
        std::cerr << "listen failed" << std::endl;
        return (1);
    }
    socklen_t addrlen = sizeof(address);
    m_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    if (m_socket < 0)
    {
        perror ("accept");
        std::cerr << "accept " << std::endl;
        return (1);
    }

    // sizeread = read(m_socket, buffer, (1024 - 1));
    // std::cout << buffer << std::endl;
    // send(m_socket, "hello", strlen("hello"), 0);
    // std::cout << "Hello message sent" << std::endl;
    // close (m_socket);
    // close (server_fd);
    return 0;
}

