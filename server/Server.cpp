/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 06:26:32 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/16 02:41:31 by ylamsiah         ###   ########.fr       */
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

Server::~Server()
{
	std::map<int, Client *>::iterator it;
	it = this->clients.begin();
	while (it != this->clients.end())
	{
		close(it->first);
		delete it->second;
		it++;
	}
	this->clients.clear();
	std::map<std::string, Channel *>::iterator it1;
	it1 = this->channels.begin();
	while (it1 != this->channels.end())
	{
		delete it1->second;
		it1++;
	}
	this->channels.clear();
}

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
	this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->server_fd < 0)
	{
		std::cout << " failed : " << errno << std::endl;
		return 0;
	}
	return 1;
}
int	Server::OptionSocket()
{
	int rec = setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (rec < 0)
	{
		std::cout << " failed : " << errno << std::endl;
		close (this->server_fd);
		return 0;
	}
	return 1;
}

int	Server::NnBlockFd()
{
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
		std::string add = inet_ntoa(newAddresse.sin_addr);
		this->clients[newfd]->set_ip_address(add);
		this->pport[newfd] = ntohs(newAddresse.sin_port);
		this->setPport(newfd, ntohs(newAddresse.sin_port));
		std::cout << "\n[\033[32;1mINFO\033[0m] \033[33;1mNew client connected with ip address\033[0m [\033[32;1m" << add << "\033[0m] \033[33;1mFrom port\033[0m [\033[32;1m" \
		<< ntohs(newAddresse.sin_port) << "\033[0m]" << std::endl;
		user_num++;
	} while (newfd != -1);
	return 1;
}
std::string update_str(std::string str)
{
    if(str[0] == ':')
    {
        size_t space_pos = str.find(" ");
        if (space_pos != std::string::npos) 
		{
            str = str.substr(space_pos + 1);
        }
		else
		{
			str = "";
		}
    }
    return str;
}
void Server::setFlagMode(bool flag) 
{
	flagMode = flag;
}

bool Server::getFlagMode() 
{
	return flagMode;
}

int Server::checkmsg(int i)
{
    std::string msg;
    char buffer[1024];

    memset(buffer, 0, sizeof(buffer));
    Message mesg;

    int flg = recv(users[i].fd, buffer, sizeof(buffer) - 1, 0);
    if (flg <= 0)
    {
        std::cout << "[\033[31;1mINFO\033[0m] \033[31;1mClient Disconnected\033[0m" << std::endl;
		send(users[i].fd, "\033[31;1mYou are Disconnected 😞.\r\n\033[0m", 40, 0);
        return 0;
    }

    buffer[flg] = '\0';
    msg = buffer;

    size_t newline_pos = msg.find_first_of("\r\n");
    if (newline_pos == std::string::npos)
    {
        this->clients[users[i].fd]->setStr(msg);
		return 1;
    }
    else
    {
		std::string tmp = "";
        if (!this->clients[users[i].fd]->getStr().empty())
        {
           for (size_t j = 0; j < this->clients[users[i].fd]->getStr().size(); j++)
			  tmp += this->clients[users[i].fd]->getStr()[j];
            this->clients[users[i].fd]->clearStr();
        }
        msg = tmp + msg;
		msg  = update_str(msg);
        std::string tmp1 = msg;
		std::string up = tmp1.substr(0, tmp1.find_first_of(" "));
		for (int i = 0 ; up[i] ; i++)
			up[i] = toupper(up[i]);
		up.erase(std::remove(up.begin(), up.end(), '\n'), up.end());
		up.erase(std::remove(up.begin(), up.end(), '\r'), up.end());
		std::cout << "Received : <<" << tmp1.erase(tmp1.find_first_of("\r\n")) << ">>" << std::endl;
		std::cout << "[\033[33;1mCMD\033[0m] \033[32;1m" << up << "\033[0m \033[33;1mFrom\033[0m [\033[32;1m" << this->get_ip_address(users[i].fd) \
		<< "\033[0m]\033[0m \033[33;1mFrom port\033[0m [\033[32;1m" << this->getPport(users[i].fd) << "\033[0m]" << std::endl;
        if (msg.find_first_of("\r\n") != std::string::npos && msg != "\n")
		{
			size_t pos = msg.find_last_of("\r\n");
			msg = msg.substr(0, pos);
			mesg = Message(users[i].fd, msg);
			TraiteMessage(mesg);
			return 1;
    	}
    }
    return 1;
}
void	Server::PollingFd()
{

	users[0].fd = server_fd;
	users[0].events = POLLIN;
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
