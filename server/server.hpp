/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:35:28 by araiteb           #+#    #+#             */
/*   Updated: 2024/01/17 12:19:15 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#define PORT 6666
#include<iostream>
#include<string>
#include <sys/socket.h>
#include <sstream>
#include <netinet/in.h>
#include <unistd.h>
class Server {
    
    public:
        const std::string m_ipAddress;
        std::string buffer[1024];
        int               m_port;
        int               m_socket;
        int               server_fd;
        int               flg;
        ssize_t             sizeread;
        struct sockaddr_in address;
        struct sockaddr_in address2;
    protected:
        
    
    public:
        Server(const std::string ipAdd, int port);
        Server(Server &sr);
        Server& operator=(const Server &sr);
        ~Server();

    
        int createStocket();
        // int run();
};

#endif