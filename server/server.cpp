/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 06:26:32 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/12 12:00:33 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string ipAdd, int port): m_pass(ipAdd),m_port(port)
{
	this->name = this->get_hostnames();
	flg = 1;
	this->on = 1;
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
std::map<int, Client *>	Server::getClient()
{
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
	int rec = setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (rec < 0)
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
	int rec = fcntl(this->server_fd, F_SETFL, O_NONBLOCK);
	if (rec < 0)
	{
		std::cout << " failed : " << errno << std::endl;
		close(this->server_fd);
		return 0;
	}
	return 1;
}

int		Server::BindSocket()
{
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(this->getport());
	int flg = bind(this->server_fd, (struct sockaddr *)&address, sizeof(address));
	if (flg < 0)
	{
		std::cout << " failed : " << errno << std::endl;
		close(this->server_fd);
		return 0;
	}
	return 1;
}
int	Server::listenSocket()
{
	int flg = listen(this->server_fd, LIMITCNX);
	if (flg < 0)
	{
		std::cout << " failed : " << errno << std::endl;
		close(this->server_fd);
		return 0;
	}
	return 1;
}

bool Server::is_authenticated(Client &client)
{
    if (client.getNick().empty() || client.geTPass().empty() || client.getusername().empty())
        return 0;
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
	send(fd, responce.c_str(), responce.length(), 0);
}

void 	Server::TraiteMessage(Message &msg) 
{
	std::vector<std::string> SplitedMsg;
    if(msg.getMessage().empty() || msg.getMessage() == "\n")
        return ;

	split(msg.getMessage(), SplitedMsg);
	std::string tmp = msg.getMessage();
    if(SplitedMsg.size() == 0)
        return ;
	this->commands(msg, SplitedMsg, tmp);
	SplitedMsg.clear();
}

void	Server::clientLeft(int fd) {
	try
	{
		std::map<int, Client *>::iterator		client;

		client = this->clients.find(fd);
		if (client != this->clients.end())
        {
			this->remove_client_from_channels(fd);
            this->removeClient(fd);
            delete this->clients[fd];
            this->clients.erase(fd);
            close(fd);
		}
	} catch (std::exception &e) {}
}

void Server::quitServer() 
{
    close (this->server_fd);
	this->on = 1;
    this->~Server();
    exit (EXIT_FAILURE);
}

std::string Server::get_ip_address(int fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        return (this->clients[fd]->getipaddress());
    return ("");
}

int 		Server::acceptingData()
{
	int newfd;
	struct sockaddr_in newAddresse;
	int lenadd = sizeof(newAddresse);
	do
	{
	   	newfd = accept (this->server_fd, (struct sockaddr *)&newAddresse, (socklen_t *)&lenadd);
		if (newfd < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				std::cout << "Failed : " << errno << std::endl;
				return 0;
			}
			break ;
		}
		Client *c = new Client(newfd);
		this->clients.insert(std::pair<int, Client *>(newfd, c));
		users[user_num].fd = newfd;
		users[user_num]. events = POLLIN;
		std::string hh = inet_ntoa(newAddresse.sin_addr);
		this->clients[newfd]->set_ip_address(hh);
		std::cout << "New connection from " << this->clients[newfd]->getipaddress() << std::endl;
		user_num++;
		sendResponce(newfd, this->name + "NOTICE AUTH :*** Looking up your hostname . . .\n");
	   	sendResponce(newfd, this->name + "NOTICE AUTH :*** Found your hostname\n");

	} while (newfd != -1);
	return 1;
}

int 	Server::checkmsg(int i)
{

	std::string msg;

	msg = "";
	memset(buffer, 0, sizeof(buffer));
	Message mesg;

    int flg = recv(users[i].fd, buffer, sizeof(buffer), 0);
    buffer[flg] = '\0';
    msg += buffer;
    std::cout << "Received: " << buffer << std::endl;
    if (flg <= 0)
    {
        std::cout << "Client left. . ." << std::endl;
        return 0;
    }
    if (msg.find_first_of("\r\n") != std::string::npos && msg != "\n")
    {
        size_t pos = msg.find_last_of("\r\n");
        msg = msg.substr(0, pos);
        mesg = Message(users[i].fd, msg);
        TraiteMessage(mesg);
        return 1;
    }
    
    return 1;
}
void	Server::PollingFd()
{

	users[0].fd = server_fd;
	users[0].events = POLLIN;
	std::cout << "Server is running . . . " << std::endl;
	do
	{
		int flg = poll(users , user_num, timeout);
		if (flg < 0)
		{
			std::cout << "Failed." << errno <<std::endl;
			this->quitServer();
		}
		if (flg == 0)
		{
			std::cout << "End program " << std::endl;
			this->quitServer();
		}
		num = user_num;
		for (int i = 0; i < num; i++)
		{
			if (users[i].revents == 0)
				continue;
			if (users[i].fd == this->server_fd)
			{
				std::cout << "New connection" << std::endl;
				if (!acceptingData())
					this->quitServer();
			}
			else
            {
				if (!this->checkmsg(i))
                {
					this->clientLeft(users[i].fd);
					for (int j = i; j < this->user_num; j++)
                    {
                        memcpy(&this->users[j], &this->users[j + 1], sizeof(struct pollfd));
                    }
                    this->user_num -= 1;
                    i--;
				}
                
			}
		}
	} while (1);
}
bool	Server::IsAuthorized(Client& client) {
	if (client.getNick().empty() || client.geTPass().empty() || client.getusername().empty())
		return 0;
	return 1;
}
