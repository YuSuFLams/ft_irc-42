/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 02:01:59 by ylamsiah          #+#    #+#             */
/*   Updated: 2024/03/18 20:24:36 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

void	Server::cmduser(Client *c, std::vector<std::string> &words , std::string str)
{
    
	if (words.size() < 5)
	{
        std::string userMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " USER :Not enough parameters\r\n";
        send(c->getFd(), userMsg.c_str(), userMsg.length(), 0);
        return ;
    }
    else if (words.size() > 5 && words[4].at(0) != ':')
    {
        std::string userMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " USER :Not enough parameters\r\n";
        send(c->getFd(), userMsg.c_str(), userMsg.length(), 0);
        return ;
    }
	c->setusename(words[1]);
    c->sethostname(words[2]);
    c->setservername(words[3]);
    std::string realname;
    if(words.size() > 5 && words[4].at(0) == ':')
    {
        str = str.erase(0, str.find(words[0]) + words[0].length() + 1);
        str = str.erase(0, str.find(words[1]) + words[1].length() + 1);
        str = str.erase(0, str.find(words[2]) + words[2].length() + 1);
        str = str.erase(0, str.find(words[3]) + words[3].length() + 1);
        realname = str.erase(0,1);
    }
    else
        realname = words[4];
    c->setrealname(realname);

    if (this->is_nickname_exist_and_registered(c->getFd() , c->getNick()))
    {
        std::string nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NICKNAMEINUSE) + " " + c->getNick() + " NICK :Nickname is already in use\r\n";
        send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
        c->seTNick("");
        return ;
    }
	if (this->IsAuthorized(*c) && c->geTPass() == this->m_pass)
    {
        c->set_is_Registered(1);
        sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 001 " + this->get_nickname(c->getFd()) + " :Welcome to the Internet Relay Network " + this->get_nickname(c->getFd())+ "!" + this->get_username(c->getFd()) + "@" + this->get_hostnames() + "\r\n");
        sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 002 " + this->get_nickname(c->getFd()) + " :Your host is " + this->get_hostnames() + ", running version 1.0\r\n");
        sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 003 " + this->get_nickname(c->getFd()) + " :This server was created " +  this->get_current_time() + "\r\n");
    }
    else if(c->geTPass() != this->m_pass && this->IsAuthorized(*c))
    {
        std::string passMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_PASSWDMISMATCH) + " " + "PASS" + " :Password incorrect\r\n";
        send(c->getFd(), passMsg.c_str(), passMsg.length(), 0);
        c->seTPass("");
        return ;
    }
}
