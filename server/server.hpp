/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:35:28 by araiteb           #+#    #+#             */
/*   Updated: 2024/01/15 14:37:30 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include<iostream>
#include<string>
#include <sys/socket.h>
#include <sstream>

class Server {
    
    private:
        const std::string m_ipAddress;
        int               m_port;
        int               m_socket;
        int               server_fd;
        int               flg = 1;
    protected:
        
    
    public:
        Server(const std::string ipAdd, int port);
        Server(Server &sr);
        Server& operator=(const Server &sr);
        ~Server();

    
        // int init();
        // int run();
};

#endif