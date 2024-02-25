/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:35:24 by araiteb           #+#    #+#             */
/*   Updated: 2024/02/25 05:44:44 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server(std::string ipAdd, int port): m_pass(ipAdd),m_port(port)
{
	this->name = ":42_IRC ";
	flg = 1;
	len = 1;
	end_ser = 0;
	compress_array = 0;
	num = 1;
	user_num = 1;
	timeout = 3 * 60 * 10000000;
	time_t now = time(0);
	this->birthday = ctime(&now);
	memset(&address, 0, sizeof(address));
	memset(users, 0, sizeof(users));
}
Server::Server(Server &sr)
{
	*this = sr;
}
Server&	Server::operator=(const Server &sr)
{
	m_port = sr.m_port;
	m_socket = sr.m_socket;
	return (*this);
	
}
Server::~Server(){}

void 	Server::seTpass(std::string pass)
{
	this->m_pass = pass;
}
void 	Server::seTport(int p)
{
	this->m_port = p;
}
void 	Server::seTmsocket(int sock)
{
	this->m_socket = sock;
}
void 	Server::seTServerFd(int fd_serv)
{
	this->server_fd = fd_serv;
}
void 	Server::seTmapClient(std::map<int, Client *> mapClient)
{
	this->clients = mapClient;
}

std::string		Server::getPass()
{
	return (this->m_pass);
}
int 	Server::getport()
{
	return this->m_port;
}
int 	Server::getsocket()
{
	return this->m_socket;
}
int 	Server::getSeverSocket()
{
	return this->server_fd;
}
std::map<int, Client *>	Server::getClient(){
	return this->clients;
}
int	Server::CreateSocket()
{
	std::cout << "building socket ... " ;
	this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->server_fd < 0)
	{
		std::cout << " failed : " << errno << std::endl;
		return 0;
	}
	std::cout << " created" << std::endl;
	return 1;
}
int	Server::OptionSocket()
{
	std::cout << "making socket reusable ... ";
	flg = setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (flg < 0)
	{
		std::cout << " failed : " << errno << std::endl;
		close (this->server_fd);
		return 0;
	}
		std::cout << " Finished." << std::endl;
	return 1;
}

int	Server::NnBlockFd()
{
	std::cout << "making socket non bloking ... ";
	flg = fcntl(this->server_fd, F_SETFL, O_NONBLOCK);
	if (flg < 0)
	{
		std::cout << " failed : " << errno << std::endl;
		close(this->server_fd);
		return 0;
	}
	std::cout << " Finished." << std::endl;
	return 1;
}

int		Server::BindSocket()
{
	std::cout << "binding socket . . . ";
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(this->getport());
	flg = bind(this->server_fd, (struct sockaddr *)&address, sizeof(address));
	if (flg < 0)
	{
		std::cout << " failed : " << errno << std::endl;
		close(this->server_fd);
		return 0;
	}
	std::cout << " Finished." << std::endl;
	return 1;
}
int	Server::listenSocket()
{
	std::cout <<  "Making socket lisening. . . ";
	flg = listen(this->server_fd, LIMITCNX);
	if (flg < 0)
	{
		std::cout << " failed : " << errno << std::endl;
		close(this->server_fd);
		return 0;
	}
	std::cout << " Finished." << std::endl;
	return 1;
}


Client*	Server::getClientByFd(int fdUser)
{
	Client *c;
	if (this->clients.find(fdUser) != this->clients.end())
	{
		c = this->clients.find(fdUser)->second;
		return c;
	}
	std::cout << "getting : " << fdUser << std::endl;
	return NULL;
}

Client*	Server::getClientByNickname(std::string nickname)
{
	std::map<int , Client *>::iterator it;

	for(it = this->clients.begin(); it != this->clients.end(); ++it)
	{
		if (it->second->getNick() == nickname)
			return (it->second);
	}
	return NULL;
}



void 	sendResponce(int fd, const std::string &responce)
{
	size_t total = 0;
    while (total != responce.length())
    {
        ssize_t nb = send(fd,responce.c_str() + total,responce.length() - total, 0);
        if (nb == -1)
            std::cout << "sending error" << std::endl; // to check later 
        total += nb;
    }
}

void 	Server::TraiteMessage(Message &msg) {
	std::vector<std::string> SplitedMsg;

	splitCommand(msg.getMessage(), ' ', SplitedMsg);
	this->commands(msg, SplitedMsg);
	SplitedMsg.clear();
}

void	Server::PollingFd()
{

	users[0].fd = server_fd;
	users[0].events = POLLIN;
	do
	{
		std:: cout << "Waiting for a request . . ." << std::endl;
		flg = poll(users , user_num, timeout);
		if (flg < 0)
		{
			std::cout << "Failed." << errno <<std::endl;
			break ;
		}
		if (flg == 0)
		{
			std::cout << "End program " << std::endl;
			close(this->server_fd);
			break ;
		}
		num = user_num;
		for (int i = 0; i < num; i++)
		{
			if (users[i].revents == 0)
				continue;
			struct sockaddr_in newAddresse;
			int lenadd = sizeof(newAddresse);

			if (users[i].fd == this->server_fd)
			{
				std::cout << "waiting for accept new connections" << std::endl;
				int newfd;
				do
				{
				   	newfd = accept (this->server_fd, (struct sockaddr *)&newAddresse, (socklen_t *)&lenadd);
				   	if (newfd < 0)
				   	{
						if (errno != EWOULDBLOCK)
						{
							std::cout << "Failed :" << errno << std::endl;
							end_ser = 1;
						}
						break;
				   	}
				   std::cout << "new connection : " << newfd << std::endl;
				   Client *c = new Client(newfd);
					// if there is space in server
				   this->clients.insert(std::pair<int, Client *>(newfd, c));
				   users[user_num].fd = newfd;
				   users[user_num]. events = POLLIN;
				   user_num++;
				   sendResponce(newfd, this->name + "NOTICE AUTH :*** Looking up your hostname . . .\n");
				   sendResponce(newfd, this->name + "NOTICE AUTH :*** Found your hostname\n");
					// else 
					//sendResponce(newfd, ":42_IRC ERROR ERROR :*** SORRY ! NO SPACE LEFT ON SERVER\n", ":42_IRC ERROR ERROR :*** SORRY ! NO SPACE LEFT ON SERVER\n".length(), 0);
				} while (newfd != -1);
			}
			else
			{
				std::string msg;
				std::cout << " Receiving msg . . . " ;
				close_conn = 0;
				// msg += user->getMsgRemainder();
				msg = "";
				memset(buffer, 0, sizeof(buffer));
				Message mesg;
				do
				{
					// check if the msg is splited
					flg = recv(users[i].fd, buffer, sizeof(buffer), 0); // check max size for receive
					// trait_msg(bfl, flg);
					buffer[flg] = '\0';
					msg += buffer;
					// std::cout << msg <<  std::endl;
					if (flg < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							std::cout << "Failed at receiving msg : " << errno << std::endl; 
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
					if (msg.find_first_of("\r\n") != std::string::npos)
					{
						size_t pos = msg.find_last_of("\r\n");
						msg = msg.substr(0, pos);
					// 	// user->setMsgRemain(remain);
						mesg = Message(users[i].fd, msg);
					} 
					// else 
					// {
						// user->setMsgRemain(remain);
					// }
				}while(1); // end of accept function
				std::cout << "out of loop : " << mesg.getMessage() << std::endl;
				TraiteMessage(mesg);
				// std::cout << "received : " << buffer << " From : " << users[i].fd <<  std::endl;
				
				// end of TraiteMessage
				if (close_conn) // manage disconnect issue
				{
					// Manage Disconnected Users
					close(users[i].fd); // moved to Discconected fct
					users[i].fd = -1;	// moved to Discconected fct
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
			}
			//
		}
	} while (end_ser == 0);
	for(int i = 0;i < user_num ;i++)// moved to Discconected fct
	{
		if (users[i].fd >= 0)// moved to Discconected fct
			close(users[i].fd);// moved to Discconected fct
	}// moved to Discconected fct
}
bool	Server::IsAuthorized(Client& client) {
	if (client.getNick().empty() || client.geTPass().empty() || client.getusername().empty())
		return 0;
	return 1;
}

