/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:35:28 by araiteb           #+#    #+#             */
/*   Updated: 2024/01/25 15:46:54 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <cstring>
#include <fcntl.h>
#include "Client.hpp"
class Server {
    
    public:        
        const std::string m_pass;
        std::string       buffer[1024];
        int               m_port;
        int               m_socket;
        int               server_fd;
        int               flg;
        int               on;
        int               user_num;
        int               num;
        ssize_t            sizeread;
        struct sockaddr_in address;
        struct pollfd      users[3];
        int                rc;
        int                len;
        int                timeout;
        int                end_ser;
        int                compress_array;
        int                close_conn;
        std::string        checkPiv;

    protected:
        
    
    public:
        Server(const std::string ipAdd, int port);
        Server(Server &sr);
        Server& operator=(const Server &sr);
        ~Server();

    
        int CreateSocket();
        int OptionSocket();
        int NnBlockFd();
        int BindSocket();
        int listenSocket();
        void PollingFd();
        int  PassValid(std::string pwd);
        void  NickClient(std::string nickname);
        // int run();
};

#endif