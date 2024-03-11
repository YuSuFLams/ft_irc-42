/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 15:04:50 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/11 06:16:08 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	this->nickname = "";
	this->pass = "";
	this->username = "";
	this->hostname = "";
	this->servername = "";
	this->realname = "";
	this->ip_address = "";
}
Client::Client(int fd)
{
	this->fd = fd;
	this->nickname = "";
	this->pass = "";
	this->username = "";
	this->hostname = "";
	this->servername = "";
	this->realname = "";
	this->is_registered = 0;
	this->InviteToChannel = false;
}
Client::Client(Client &cl)
{
	*this = cl;
}
Client& Client::operator=(const Client &cl)
{
	nickname = cl.nickname;
	pass = cl.pass;
	this->username = cl.username;
	this->hostname = cl.hostname;
	this->realname = cl.realname;
	this->servername = cl.servername;
	this->is_registered = cl.is_registered;
	this->InviteToChannel = cl.InviteToChannel;
	return (*this);
}

Client::~Client() {}

void    Client::seTNick(std::string nick) { nickname = nick; }

void    Client::seTPass(std::string pwd) { pass = pwd; }

void    Client::setFdUser(int FdUser) { fd = FdUser; }

void	Client::setusename(std::string val) { this->username = val; }

void	Client::sethostname(std::string val) { this->hostname = val; }

void	Client::setrealname(std::string val) { this->realname = val; }

void	Client::setservername(std::string val) { this->servername = val; }

void	Client::setInviteToChannel(bool InviteToChannel) { this->InviteToChannel = InviteToChannel; }

void	Client::set_is_Registered(int is_registered) { this->is_registered = is_registered; }

std::string     Client::getNick() { return this->nickname; }

std::string     Client::geTPass(){ return this->pass; }

std::string		Client::getusername(){ return this->username; }

std::string		Client::gethostname(){ return this->hostname; }

std::string		Client::getservername(){ return this->servername; }

std::string		Client::getrealname(){ return this->realname; }

int             Client::getFd() { return this->fd; }

std::string		Client::getIdent(){ return (this->getNick() + "!" + this->gethostname() + "@localhost");}

bool			Client::getInviteToChannel() { return (this->InviteToChannel); }

int				Client::is_Registered() { return (this->is_registered); }
