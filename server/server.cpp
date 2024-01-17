/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:35:24 by araiteb           #+#    #+#             */
/*   Updated: 2024/01/15 14:37:37 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(const std::string ipAdd, int port): m_ipAddress(ipAdd),m_port(port)
{
    
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
// int  Server::init()
// {
//     if (this->server_fd = socket(AF_INET, SOCK_STREAM, 0))
//     {
//         std::cerr << "socket failed" << std::endl;
//         exit (1);
//     }
//     if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &flg, sizeof(flg)))
//     {
//         std::cout << "setsockopt" << std::endl;
//         exit (1);
//     }
// }

// int Server::run()
// {
    
// }