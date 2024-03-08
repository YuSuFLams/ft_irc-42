/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:35:28 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/06 13:20:01 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

# define MAX 1024

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
#include <cstddef> 
#include <sstream>
#include "../Clients/Client.hpp"
#include "../message/messages.hpp"
#include "../Exeption/Exception.hpp"
#include <map>

#define LIMITCNX 5000
class Client;
class Message;

class Server {
	
	private:
		char *						birthday;
		std::string					name;
		std::string					m_pass;
		char						buffer[MAX];
		int							m_port;
		int							m_socket;
		int							server_fd;
		int							flg;
		int							on;
		int							user_num;
		int							num;
		int							end_con;
		struct sockaddr_in			address;
		struct pollfd				users[LIMITCNX];
		int							len;
		long int					timeout;
		int							end_ser;
		int							compress_array;
		std::string					msgsendResponce;
		std::string					checkPiv;
		std::map <int, Client *>	clients;

	public:
		Server(const std::string ipAdd, int port);
		Server(Server &sr);
		Server& operator=(const Server &sr);
		~Server();
		
		void			seTpass(const std::string);
		void 			seTport(int p);
		void 			seTmsocket(int sock);
		void 			seTServerFd(int server);
		void 			seTmapClient(std::map<int, Client *>);

		std::string			getPass();
		int 				getport();
		int 				getsocket();
		int 				getSeverSocket();
		std::map<int, Client *> getClient();



		
		int				CreateSocket();
		int				OptionSocket();
		int				NnBlockFd();
		int				BindSocket();
		int				listenSocket();
		int 			acceptingData();
		int 			checkmsg(int i);
		void			PollingFd();
		
		void			commands(Message &, std::vector <std::string>&);
		
		void			cmduser(Client *c, std::vector<std::string> &SplitedMsg);
		void			cmdpass(std::vector<std::string>& SplitedMsg, Client &c);
		void			cmdknick(std::vector<std::string> &SplitedMsg, Client *c);
		void			cmdprivmsg(std::vector<std::string>& SplitedMsg, Client *c);

		Client*			getClientByFd(int fdUser);
		Client*			getClientByNickname(std::string nickname);
		bool			IsAuthorized(Client &);
		void			TraiteMessage(Message &);

		void 			clientLeft(int fd);
		void 			quitServer();

};

void	split(std::string msg, std::vector<std::string> &SplitedMsg);
void    splitCommand(std::string str, char oper, std::vector<std::string> &SplitedMsg);
void	initTab(std::string strs[MAX]);
void 	sendResponce(int fd, const std::string &responce);
std::string const   int2string (int n);
#endif
