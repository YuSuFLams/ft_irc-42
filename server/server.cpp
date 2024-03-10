/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:35:24 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/09 21:23:02 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server(std::string ipAdd, int port): m_pass(ipAdd),m_port(port)
{
	this->name = this->get_hostnames();
	flg = 1;
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
	this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->server_fd < 0)
	{
		std::cout << "socket failed : " << errno << std::endl;
		return 0;
	}
	return 1;
}
int	Server::OptionSocket()
{
	int opt = 1;
	if(setsockopt(this->server_fd , SOL_SOCKET,SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        std::cout << "Setsockopt failed" << std::endl;
        return (0);
    }
	return 1;
}

int	Server::NnBlockFd()
{
	if (fcntl(this->server_fd , F_SETFL, O_NONBLOCK) < 0)
    {
        std::cout << "Fcntl failed" << std::endl;
        return (0);
    }
	return 1;
}

int		Server::BindSocket()
{
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
	return 1;
}
int	Server::listenSocket()
{
	flg = listen(this->server_fd, LIMITCNX);
	if (flg < 0)
	{
		std::cout << " failed : " << errno << std::endl;
		close(this->server_fd);
		return 0;
	}
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
	send(fd, responce.c_str(), responce.length(), 0);
}

void 	Server::TraiteMessage(Message &msg) {
	std::vector<std::string> SplitedMsg;

	split(msg.getMessage(), SplitedMsg);
	std::string tmp = msg.getMessage();
	this->commands(msg, SplitedMsg, tmp);
	SplitedMsg.clear();
}

void	Server::clientLeft(int fd) {
	try
	{
		std::map<int, Client *>::iterator		client;

		client = this->clients.find(fd);
		if (client != this->clients.end()) {
			delete client->second;
			this->clients.erase(client);
		}
	} catch (std::exception &e) {}
}

void Server::quitServer() {
    close (this->server_fd);
    this->~Server();
    exit (EXIT_FAILURE);
}

int 		Server::acceptingData(){
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
		// if there is space in server
		this->clients.insert(std::pair<int, Client *>(newfd, c));
		users[user_num].fd = newfd;
		users[user_num]. events = POLLIN;
		user_num++;

	} while (newfd != -1);
	return 1;
}

int 	Server::checkmsg(int i)
{

	std::string msg;

	msg = "";
	memset(buffer, 0, sizeof(buffer));
	Message mesg;
	do
	{
		flg = recv(users[i].fd, buffer, sizeof(buffer), 0);
		buffer[flg] = '\0';
		msg += buffer;
		std::cout << "Received: " << buffer << std::endl;
		if (flg < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				return 0;
			}
			continue ;
		}
		if (flg == 0)
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
		else
			break ; 
	}while(1); // end of accept function
	return 1;
}
void	Server::PollingFd()
{

	users[0].fd = server_fd;
	users[0].events = POLLIN;
	std::cout << "Server is running . . . " << std::endl;
	do
	{
		flg = poll(users , user_num, timeout);
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
			else{
				if (!this->checkmsg(i)){
					this->clientLeft(users[i].fd);
					for (int j = i; j < this->user_num; j++)
                    {
                        memcpy(&this->users[j], &this->users[j + 1], sizeof(struct pollfd));
                    }
                    this->user_num -= 1;
                    i --;
				}
			}
		}
	} while (1);
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


std::string Server::to_string(int number)
{
    std::stringstream ss;
    ss << number;
    return (ss.str());
}


std::string Server::get_hostnames()
{
    char buffer[1024] = {0};
    std::string hostnames;
    gethostname(buffer, 1024);
    hostnames = buffer;
    return (hostnames);
}
std::string Server::get_nickname(int fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        return (clients[fd]->getNick());
    return ("");
}

void Server::set_nickname(int fd, std::string nickname)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        clients[fd]->seTNick(nickname);
}

std::map<std::string, Channel *> &Server::getChannels()
{
    return (this->channels);
}

long Server::get_limit(std::string channel)
{
    std::map<std::string, Channel *>::iterator it;
    it = this->channels.find(channel);
    if(it != this->channels.end())
        return (channels[channel]->getLimit());
    return (-1);
}

int Server::get_fd_users(const std::string& nickname) const
{
    std::map<int, Client *>::const_iterator it;
    for (it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if (it->second->getNick() == nickname)
            return (it->first);
    }
    return (-1);
}
std::string Server::get_hostname(int fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        return (clients[fd]->gethostname());
    return ("");
} 
int Server::is_nickname_exist_and_registered(std::string nickname)
{
    std::map<int, Client *>::iterator it;
    for (it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if (it->second->getNick() == nickname && it->second->is_Registered())
            return (1);
    }
    return (0);
}

void Server::set_topic(std::string channel, std::string topic)
{
    std::map<std::string, Channel *>::iterator it;
    it = this->channels.find(channel);
    if(it != this->channels.end())
        channels[channel]->set_topic(topic);
}

std::string Server::get_topic(std::string channel)
{
    std::map<std::string, Channel *>::iterator it;
    it = this->channels.find(channel);
    if(it != this->channels.end())
        return (channels[channel]->get_topic());
    return ("No topic is set");
}

void Server::priny_users(std::string channel)
{
    std::map<std::string, Channel *>::iterator it;
    it = this->channels.find(channel);
    if(it != this->channels.end())
        channels[channel]->print_users();
}


bool Server::getInviteToChannel(int fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        return (clients[fd]->getInviteToChannel());
    return (false);
}

std::string Server::get_username(int fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        return (clients[fd]->getusername());
    return ("");
}
std::string Server::get_realname(int fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        return (clients[fd]->getrealname());
    return ("");
}


std::string Server::get_servername(int fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        return (clients[fd]->getservername());
    return ("");
}

std::string Server::get_current_time()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return (buf);
}

int Server::is_nickname_exist(std::string nickname)
{
    std::map<int, Client *>::iterator it;
    for (it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if (it->second->getNick() == nickname)
            return (1);
    }
    return (0);
}

void Server::set_is_registered(int fd, int is_registered)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        it->second->set_is_Registered(is_registered);
}
int Server::is_registered(int fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        return (it->second->is_Registered());
    return (0);
}

void Server::addChannel(std::string name, Channel *channel)
{
    this->channels[name] = channel;
}
void Server::set_creator(std::string channel, bool creater)
{
    std::map<std::string, Channel *>::iterator it;
    it = this->channels.find(channel);
    if(it != this->channels.end())
        channels[channel]->set_creater(creater);
}

std::string Server::get_creator_name(std::string channel)
{
    std::map<std::string, Channel *>::iterator it;
    it = this->channels.find(channel);
    if(it != this->channels.end())
        return (channels[channel]->get_creator_name());
    return ("");
}

void Server::set_fd(int fd, int new_fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        clients[fd]->setFdUser(new_fd);
}
void Server::set_password(int fd, std::string password)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        it->second->seTPass(password);
}
void Server::set_client(int fd, Client *client)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        clients[fd] = client;
}
void Server::set_clients(std::map<int, Client *> clients)
{
    this->clients = clients;
}
void Server::remove_client_from_channels(int fd)
{
    std::map<std::string, Channel *>::iterator it;
    for (it = this->channels.begin(); it != this->channels.end(); it++)
    {
        it->second->quit_channel(this->get_nickname(fd));
    }

    it = this->channels.begin();
    while(it != this->channels.end())
    {
        if (it->second->getUsers().size() == 0)
        {
            delete it->second;
            this->channels.erase(it++);
        }
        else
            it++;
    }
}


void Server::set_username(int fd, std::string username)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        clients[fd]->setusename(username);
}
void Server::set_realname(int fd, std::string realname)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        clients[fd]->setrealname(realname);
}
void Server::set_hostname(int fd, std::string hostname)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        clients[fd]->sethostname(hostname);
}
void Server::set_servername(int fd, std::string servername)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        clients[fd]->setservername(servername);
}
std::string Server::get_password(int fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        return (clients[fd]->geTPass());
    return ("");
}
std::map<int, Client *> &Server::getClients()
{
    return (this->clients);
}
void Server::addClient(int fd, Client *client)
{
    this->clients[fd] = client;
}
void Server::removeClient(int fd)
{
    this->clients.erase(fd);
}

void Server::setClients(std::map<int, Client *> clients)
{
    this->clients = clients;
}
std::string Server::get_users(std::string channel)
{
    std::map<std::string, Channel *>::iterator it;
    it = this->channels.find(channel);
    if(it != this->channels.end())
    {
        std::set<std::string>::iterator it = channels[channel]->getUsers().begin();
        std::string users;
        while(it != channels[channel]->getUsers().end())
        {
            users += *it + " ";
            it++;
        }
        return (users);
    }
    return ("");
}