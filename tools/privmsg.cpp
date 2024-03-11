/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 04:50:39 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/11 06:37:47 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"

void Server::send_to_channel(std::string channel_name , std::string str ,int fd)
{
    std::string message;
    std::map<std::string, Channel*>::iterator it = this->getChannels().begin();
    for (; it != this->getChannels().end(); it++)
    {
        if(it->first == channel_name)
        {
            std::set<std::string>::iterator it2 = it->second->getUsers().begin();
            for (; it2 != it->second->getUsers().end(); it2++)
            {
                int user = this->get_fd_users(*it2);
                message = ":" + this->get_nickname(fd) + "!" + this->get_username(fd) + "@" + this->get_ip_address(user) + " PRIVMSG " + channel_name + " :" + str + "\r\n";
                if(user != fd)
                send(user, message.c_str(), message.length(), 0);
            }
        }
    }
}


void Server::send_to_user(std::string nickname , std::string str ,int fd)
{
    std::string message = ":" + this->get_nickname(fd) + "!" + this->get_username(fd) + "@" + this->get_ip_address(fd) + " PRIVMSG " + nickname + " :" + str + "\r\n";
    int user = this->get_fd_users(nickname);
    send(user, message.c_str(), message.length(), 0);
}



void Server::privmsg_command(std::vector<std::string > words  , int fd , std::string str)
{
    if(words.size() > 3 && words[2].at(0) != ':')
    {
        std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " " + this->get_nickname(fd) + " PRIVMSG :Not enough parameters\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return ;
    }
    
    if(words.size() == 1)
    {
        std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NORECIPIENT) + " " + this->get_nickname(fd) + " :No recipient given (PRIVMSG)\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return ;
    }
    
    if(words.size() == 2)
    {
        std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NOTEXTTOSEND) + " " + this->get_nickname(fd) + " " + words[1] + " :No text to send\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return ;
    }
    
    std::string message = "";
    if(words.size() > 3 && words[2].at(0) == ':')
    {
        message =  str.substr(str.find(":") + 1 , str.length());
    }
    else
    {
        message = words[2];
    }
    
    std::vector<std::string> targets;
    if(words[1].find(',') != std::string::npos) 
    {
        std::stringstream ss(words[1]);
        std::string token;
        while (std::getline(ss, token, ',')) 
        {
            targets.push_back(token);
        }
        token.clear();
        ss.clear();
    } 
    else 
    {
        targets.push_back(words[1]);
    }
    
    for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it++)
    {

        if(it->at(0) == '#' || it->at(0) == '&')
        {
            if(this->getChannels().find(*it) != this->getChannels().end() && this->getChannels()[*it]->getUsers().find(this->get_nickname(fd)) != this->getChannels()[*it]->getUsers().end())
            {
                this->send_to_channel(*it, message, fd);
            }
            else
            {
                if(this->getChannels().find(*it) == this->getChannels().end())
                {
                    std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NOSUCHCHANNEL) + " " + this->get_nickname(fd) + " " + *it + " :No such channel\r\n";
                    send(fd, str.c_str(), str.length(), 0);
                    continue;
                }
                std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_CANNOTSENDTOCHAN) + " " + this->get_nickname(fd) + " " + *it + " :Cannot send to channel\r\n";
                send(fd, str.c_str(), str.length(), 0);
            }
        }
        else
        {
            if(!this->is_nickname_exist(*it) || !this->is_authenticated(*this->getClientByNickname(*it)))
            {
                std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NOSUCHNICK) + " " + this->get_nickname(fd) + " " + *it + " :No such nick\r\n";
                send(fd, str.c_str(), str.length(), 0);
                continue;
            }
            this->send_to_user(*it, message, fd);
        }
    }
    targets.clear();
    message.clear();
}