/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:35:28 by araiteb           #+#    #+#             */
/*   Updated: 2024/01/30 14:54:40 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#define MAX 10254
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
#include<map>

class Client;

class Server {
	
	public:        
		const std::string        m_pass;
		char                     buffer[1024];
		int                      m_port;
		int                      m_socket;
		int                      server_fd;
		int                      flg;
		int                      on;
		int                      user_num;
		int                      num;
		ssize_t                  sizeread;
		struct sockaddr_in       address;
		struct pollfd            users[3];
		int                      rc;
		int                      len;
		long int                     timeout;
		int                      end_ser;
		int                      compress_array;
		int                      close_conn;
		Client                   *UserDirection;
		std::string              msgSend;
		std::string              checkPiv;
		std::map <int, Client *> clients;
	protected:
		
	
	public:
		Server(const std::string ipAdd, int port);
		Server(Server &sr);
		Server& operator=(const Server &sr);
		~Server();
		
		int             CreateSocket();
		int             OptionSocket();
		int             NnBlockFd();
		int             BindSocket();
		int             listenSocket();
		void            PollingFd();
		void             PassValid(std::string pwd, Client *c);
		// void  NickClient(std::string nickname);
		void            commands(int fdUser, std::string strs[MAX], std::map <int, Client *> clients);
		void            seTValueUser(Client *c, std::string strs[MAX]);
		void            CheckNick(std::string NewNick, Client *c);

		Client*			getClientByFd(int fdUser, std::map <int, Client *> clients);
		Client*			getClientByNickname(std::string nick, std::map <int, Client *> clients);
		bool 			IsAuthorized(Client *client);
};
void    split(std::string str, char oper, std::string strs[MAX]);
#endif
