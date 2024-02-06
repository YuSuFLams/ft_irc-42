/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 15:04:50 by araiteb           #+#    #+#             */
/*   Updated: 2024/02/06 10:18:45 by araiteb          ###   ########.fr       */
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
}
Client::Client(int fd)
{
    std::cout << "New User Added !" << std::endl;
    this->fd = fd;
    this->nickname = "";
    this->pass = "";
    this->username = "";
    this->hostname = "";
    this->servername = "";
    this->realname = "";
    // std::fill(users.begin(), users.end(), "");
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
    return (*this);
}
Client::~Client()
{
    
}
void    Client::seTNick(std::string nick)
{
    nickname = nick;
}

void    Client::seTPass(std::string pwd)
{
    pass = pwd;
}
void    Client::setFdUser(int FdUser)
{
    fd = FdUser;
}
void    Client::seTValues(std::string str0, std::string str1, std::string str2, std::string str3)
{
   this->username = str0;
   this->hostname = str1;
   this->servername = str2;
   this->realname = str3;
}
std::string     Client::getNick() {
    return (nickname);
}

std::string     Client::geTPass()
{
    return (pass);
}

std::string  Client::getClient() {
    if (this->username.empty()){
        std::cout << "get info user() " << std::endl;
        return ("");
    }
    return (this->username);
} 

int             Client::getFd() {
    return fd;
}
