/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 12:39:59 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/13 01:08:26 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

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