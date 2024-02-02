/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:35:24 by araiteb           #+#    #+#             */
/*   Updated: 2024/02/02 15:30:12 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server(const std::string ipAdd, int port): m_pass(ipAdd),m_port(port)
{
	flg = 1;
	len = 1;
	end_ser = 0;
	compress_array = 0;
	num = 1;
	user_num = 1;
	timeout = 3 * 60 * 10000000;
	memset(&address, 0, sizeof(address));
	memset(users, 0, sizeof(users));
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

int     Server::CreateSocket()
{
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		return 0;
	return 1;
}
int     Server::OptionSocket()
{
	flg = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (flg < 0)
	{
		perror("setsockopt");
		close (server_fd);
		return 0;
	}
	return 1;
}

int     Server::NnBlockFd()
{
	flg = fcntl(server_fd, F_SETFL, O_NONBLOCK);
	if (flg < 0)
	{
		perror("fcntl");
		close(server_fd);
		return 0;
	}
	return 1;
}

int     Server::BindSocket()
{
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(this->m_port);
	flg = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
	if (flg < 0)
	{
		perror ("bind");
		close(server_fd);
		return 0;
	}
	return 1;
}
int     Server::listenSocket()
{
	flg = listen(server_fd, 80);
	if (flg < 0)
	{
		perror("listen");
		close(server_fd);
		return 0;
	}
	return 1;
}

void    Server::CheckNick(std::string NewNick, Client *c)
{
	Client *tmpClient;
	try{
		
	if (NewNick.empty())
		send (c->getFd(), "Error No NickName Provided !\n",sizeof("Error No NickName Provided !\n"), 0);
	else {
		tmpClient = this->getClientByNickname(NewNick, this->clients);
		if (tmpClient)
			send (c->getFd(), "Error NickName ERR_NONICKNAMEGIVEN !\n",sizeof("Error No NickName ERR_NONICKNAMEGIVE !\n"), 0);
		else
			c->seTNick(NewNick); 
	}
	}catch(...){
		std::cout << "error \n" <<std::endl;
	}
}

void       Server::PassValid(std::string pwd, Client *c)
{
	std::cout << "CHECKING PASS " << std::endl;
	try{
		if (pwd.empty())
			send (c->getFd(), "Error No PASSWORD Provided !\n",sizeof("Error No PASSWORD Provided !\n"), 0);
		else {
			if (pwd.compare(this->m_pass))
				send (c->getFd(), "Error PASSWORD ERR_ALREADYREGISTRED !\n",sizeof("Error No PASSWORD ERR_ALREADYREGISTRED!\n"), 0);
			else
			{
				c->seTPass(pwd);
				send (c->getFd(), "Welcome in server !\n",sizeof("Welcome in server !\n"), 0);
			}
		}
	}
   catch(...){
		std::cout << "error \n" <<std::endl;
	}
}
Client*   Server::getClientByFd(int fdUser, std::map <int, Client *> clients)
{
	Client *c;
	if (clients.find(fdUser) != clients.end())
	{
		c = clients.find(fdUser)->second;
		return c;
	}
	else
		std::cerr << "user not find" << std::endl;
	return NULL;
}

Client*   Server::getClientByNickname(std::string nick, std::map <int, Client *> clients)
{
	Client *c;
	std::map<int , Client *>::iterator it;

	for(it=clients.begin(); it != clients.end(); ++it)
	{
		if (it->second->getNick() == nick)
		{
			c = it->second;
			return c;
		}
	}
	return NULL;
}
void      Server::seTValueUser(Client *c, std::string strs[MAX])
{
	try {
		if (strs[1].empty() || strs[2].empty() || strs[3].empty() || strs[4].empty())
			send(c->getFd(), "ERR_NEEDMOREPARAMS !\n",sizeof("ERR_NEEDMOREPARAMS !\n"), 0);
		else 
		{
			Client *tmpClient = getClientByFd(c->getFd(), clients);	
			if (tmpClient->getClient().empty()) {
				std::cout << "HERE" << std::endl;
				send(c->getFd(), "HEY IN SERVER !\n",sizeof("HEY IN SERVER !\n"), 0);
				c->seTValues(strs[1], strs[2], strs[3], strs[4]);
			}
			else
			{
				send(c->getFd(), "ERR_ALREADYREGISTRED !\n",sizeof("ERR_ALREADYREGISTRED !\n"), 0);
			}
		}
	}
	catch(...) {
		std::cout << "error \n" <<std::endl;
	}
}

void 	Server::privMsg(std::string NewNick, std::string msg ,Client *c)
{
	c = getClientByNickname(NewNick, clients);
	send(c->getFd(), msg.c_str(), sizeof(msg), 0);
}
void    Server::commands(int fdUser, std::string strs[MAX], std::map <int, Client *> clients)
{
	Client *c;
   
	std::cout<< "Traiting Commands : " << fdUser << std::endl;
	c = getClientByFd(fdUser, clients);
	if (!this->IsAuthorized(c) && strs[0].compare("NICK") && strs[0].compare("PASS") && strs[0].compare("USER") && strs[0].compare("PRIVMSG"))
		send(fdUser, "you are not AUTHORIZED !!!\n", sizeof("you are not AUTHORIZED !!!\n"), 0);
	else if (!this->IsAuthorized(c))
	{
		if (!strs[0].compare("NICK"))
			CheckNick(strs[1],  c);
		else if (!strs[0].compare("PASS"))
			PassValid(strs[1], c);
		else if (!strs[0].compare("USER"))
			seTValueUser(c, strs);
	}
	
	std::cout << strs[0] << std::endl; 
	if (!strs[0].compare("PRIVMSG"))
		privMsg(strs[1], strs[2], UserDirection);
}
void     Server::PollingFd()
{
	std::string strs[MAX];
	users[0].fd = server_fd;
	users[0].events = POLLIN;
	do
	{
		std:: cout << "Waiting on poll()..." << std::endl;
		flg = poll(users , user_num, timeout);
		if (flg < 0)
		{
			perror("poll");
			break ;
		}
		if (flg == 0)
		{
			std::cout << "End program " << std::endl;
			break ;
		}
		num = user_num;
		for (int i = 0; i < num; i++)
		{
			if (users[i].revents == 0)
				continue;
			if (users[i].events != POLLIN)
			{
				std::cout << "error revents = " << users[i].events << std::endl;
				end_ser = 1;
				break ;
			}
			if (users[i].fd == server_fd)
			{
				std::cout << " Listening socket is readable " << std::endl;
				do
				{
				   m_socket = accept (server_fd, NULL, NULL);
				   if (m_socket < 0)
				   {
					if (errno != EWOULDBLOCK)
					{
						perror("accept");
						end_ser = 1;
					}
					break;
				   }
				   std::cout << "new connection " << m_socket << std::endl;
				   Client *c = new Client(m_socket);

				   this->clients.insert(std::pair<int, Client *>(m_socket, c));
				   users[user_num].fd = m_socket;
				   users[user_num]. events = POLLIN;
				   user_num++;
				} while (m_socket != -1);
				
			}
			else
			{
				std::cout << " Descriptor " << users[i].fd  << " is readable " << std::endl;
				close_conn = 0;
				memset(buffer, 0, sizeof(buffer));
				do
				{
					flg = recv(users[i].fd, buffer, sizeof(buffer), 0);
					buffer[flg - 1] = '\0';
					// remove delimiteur if existe \r\n
					
					if (flg < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							perror("recv");
							close_conn = 1;
						}
						break ;
					}
					if (flg == 0)
					{
						std::cout << " Connection closed " << std::endl;
						close_conn = 1;
						break ; 
					}
					len = flg;
					
				}while(1);
				std::cout << len << " bytes received " << buffer << " From :" << users[i].fd <<  std::endl;
				split(buffer, ' ', strs);
				this->commands(users[i].fd ,strs , clients);
				if (close_conn)
				{
					close(users[i].fd);
					users[i].fd = -1;
					compress_array = 1;
				}
			}
		}
		if (compress_array)
		{
			compress_array = 0;
			for (int i = 0; i < user_num; i++)
			{
				if (users[i].fd == -1)
				{
					for (int j = i; j < user_num; j++)
						users[j].fd = users[j + 1].fd;
					i--;
					user_num--;
				}
			}
		}
	} while (end_ser == 0);
	for(int i = 0;i < user_num ;i++)
	{
		if (users[i].fd >= 0)
			close(users[i].fd);
	}
}
bool 			Server::IsAuthorized(Client *client) {
	if (client->getNick().empty() || client->geTPass().empty() || client->getClient().empty())
		return 0;
	return 1;
}
