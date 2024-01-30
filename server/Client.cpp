/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 15:04:50 by araiteb           #+#    #+#             */
/*   Updated: 2024/01/30 13:17:18 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
    users[0] = "";
    users[1] = "";
    users[2] = "";
    users[3] = "";
}
Client::Client(int fd)
{
    std::cout << "New User Added !" << std::endl;
    this->fd = fd;
    this->nickname = "";
    this->pass = "";
    std::fill(users.begin(), users.end(), "");
}
Client::Client(Client &cl)
{
    *this = cl;
}
Client& Client::operator=(const Client &cl)
{
    nickname = cl.nickname;
    pass = cl.pass;
    users[0] = cl.users[0];
    users[1] = cl.users[1];
    users[2] = cl.users[2];
    users[3] = cl.users[3];
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
    users[0] = str0;
    users[1] = str1;
    users[2] = str2;
    users[3] = str3;
}
std::string     Client::getNick() {
    return (nickname);
}

std::string     Client::geTPass()
{
    return (pass);
}

std::vector<std::string>  Client::getClient() {
    return (this->users);
} 

int             Client::getFd() {
    return fd;
}
// void       Client::CheckOper(std::string  buffer)
// {
//     std::string node;
    
//     // node = strtok(buffer, " ");
// }