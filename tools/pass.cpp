/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 02:01:29 by ylamsiah          #+#    #+#             */
/*   Updated: 2024/03/18 02:01:49 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

void	Server::cmdpass(std::vector<std::string>& words, Client *c , std::string str)
{
	if (words.size() < 2 || (words.size() > 2 && words[1].at(0) != ':'))
	{
        std::string passMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " PASS :Not enough parameters\r\n";
        send(c->getFd(), passMsg.c_str(), passMsg.length(), 0);
        return ;
    }
	else 
    {
        std::string pass;
        if (c->getNick() == "Bot" && words[2].compare(this->m_pass))
        {
            std::string passMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_PASSWDMISMATCH) + " PASS :Password incorrect\r\n";
            send(c->getFd(), passMsg.c_str(), passMsg.length(), 0);
            return ;
        }
        if(words.size() > 2 && words[1].at(0) == ':')
        {
            pass = str.substr(str.find(":") + 1 , str.length());
        }
        else
            pass = words[1];
		c->seTPass(pass);
        if (this->IsAuthorized(*c) && c->geTPass() == this->m_pass)
        {
            c->set_is_Registered(1);
            sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 001 " + this->get_nickname(c->getFd()) + " :Welcome to the Internet Relay Network " + this->get_nickname(c->getFd())+ "!" + this->get_username(c->getFd()) + "@" + this->get_hostnames() + "\r\n");
            sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 002 " + this->get_nickname(c->getFd()) + " :Your host is " + this->get_hostnames() + ", running version 1.0\r\n");
            sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 003 " + this->get_nickname(c->getFd()) + " :This server was created " +  this->get_current_time() + "\r\n");
        }
        else if(c->geTPass() != this->m_pass && this->IsAuthorized(*c))
        {
            std::string passMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_PASSWDMISMATCH) + " PASS :Password incorrect\r\n";
            send(c->getFd(), passMsg.c_str(), passMsg.length(), 0);
            c->seTPass("");
            return ;
        }
	}
}
