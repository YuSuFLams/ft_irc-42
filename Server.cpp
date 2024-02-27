/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 04:53:13 by abel-hid          #+#    #+#             */
/*   Updated: 2024/01/03 05:40:22 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
    this->port = 0;
    this->server_fd = 0;
    this->new_socket = 0;
}

Server::~Server()
{
    
}

int Server::getPort() const
{
    return port;
}

int Server::getServer_fd() const
{
    return server_fd;
}

int Server::getNew_socket() const
{
    return new_socket;
}

void Server::setPort(int port)
{
    this->port = port;
}

void Server::setServer_fd(int server_fd)
{
    this->server_fd = server_fd;
}
void Server::setNew_socket(int new_socket)
{
    this->new_socket = new_socket;
}

sockaddr_in Server::getAddress() const
{
    return address;
}

void Server::setAddress(sockaddr_in address)
{
    this->address = address;
}