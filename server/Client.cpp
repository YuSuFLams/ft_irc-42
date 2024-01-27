/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 15:04:50 by araiteb           #+#    #+#             */
/*   Updated: 2024/01/27 12:04:00 by araiteb          ###   ########.fr       */
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
    fd = fd;
    nickname = "";
    pass = "";
    users[0] = "";
    users[1] = "";
    users[2] = "";
    users[3] = "";
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
std::string     Client::geTNick()
{
    return (nickname);
}

std::string     Client::geTPass()
{
    return (pass);
}
Client      Client::seTValue(std::string nick, std::string pwd, int NewSocket)
{
    Client c;
    
    c.seTNick(nick);
    c.seTPass(pwd);
    c.fd = NewSocket;
    std::cout << "set a username :" << std::endl;
    std::cin >> c.users[0] ;
    std::cout << "set a hostname : " << std::endl;
    std::cin >> c.users[1];
    std::cout << "set a servername : " << std::endl;
    std::cin >> c.users[2];
    std::cout << "set a realname : " << std::endl;
    std::cin >> c.users[3];
    return c;
}

void    Client::CheckNick(std::string NewNick, std::map <int, Client *> cls)
{
    if (!this->nickname.empty())
    {
        if (nickname.compare(NewNick))
            nickname = NewNick;
    }
    // Client *c = cls.find(this->fd);
    // if (clients.find(this->fd)->nickname == )
    nickname = NewNick;
}
void       Client::CheckOper(std::string  buffer)
{
    std::string node;
    
    // node = strtok(buffer, " ");
}