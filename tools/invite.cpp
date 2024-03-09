/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 20:23:08 by ylamsiah          #+#    #+#             */
/*   Updated: 2024/03/09 02:07:13 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"

bool Server::isValidChannelName(std::string name)
{
    if (name.empty() || name.length() > 50 || name.find(' ') != std::string::npos)
        return false;
    if (name[0] != '&' && name[0] != '#' && name[0] != '+' && name[0] != '!')
        return false;
    if (name.find(',') != std::string::npos || name.find(7) != std::string::npos)
        return false;
    return true;
}

bool Server::isClientExist(std::string nickname)
{
    for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if (it->second->getNick() == nickname)
            return (true);
    }
    return false;
}

bool Server::isChannelExist(std::string channelname)
{
    for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if (it->first == channelname)
            return (true);
    }
    return false;
}

bool Server::isClientInChannel(std::string nickname, std::string channelname, std::map<std::string, Channel *> &channels)
{
    std::map<std::string, Channel *>::iterator it;
    for (it = channels.begin(); it != channels.end(); it++)
    {
        if (it->first == channelname)
            if (it->second->isUser(nickname))
                return (true);
    }
    return (false);
}

bool Server::isClientOperatorInChannel(std::string nickname, std::string channelname, std::map<std::string, Channel *> &channels)
{
    std::string new_nickname = "@" + nickname;
    std::map<std::string, Channel *>::iterator it;
    for (it = channels.begin(); it != channels.end(); it++)
    {
        if (it->first == channelname)
        {
            if (it->second->isOperator(new_nickname))
                return (true);
        }
    }
    return (false);
}

bool Server::isSenderInChannel(std::string nickname, std::string channelname, std::map<std::string, Channel *> &channel)
{
    for (std::map<std::string, Channel *>::iterator it = channel.begin(); it != channel.end(); it++)
    {
        if (it->first == channelname)
        {
            if (it->second->isUser(nickname))
                return (true);
        }
    }
    return false;
} 

Client*   Server::getClientByNickname(std::string nick, std::map<int , Client *> clients)
{
    for(std::map<int , Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
        if (it->second->getNick() == nick)
            return it->second;
    return NULL;
}

void Server::invitecmd(std::vector<std::string> words, Server server, int fd)
{
    if (words.size() != 3)
    {
        std::string errorMsg = ":" + server.get_hostnames() + " " + to_string(ERR_NEEDMOREPARAMS) + " " + server.get_nickname(fd) + " INVITE :Not enough parameters\r\n";
        send(fd, errorMsg.c_str(), errorMsg.length(), 0);
    }
    else 
    {
        // client does not exist
        if (!server.isClientExist(words[1]))
        {
            std::string errorMsg = ":" + server.get_hostnames() + " " + to_string(ERR_NOSUCHNICK) + " " + server.get_nickname(fd) + " " + words[1] + " :No such nick\r\n";
            send(fd, errorMsg.c_str(), errorMsg.length(), 0);
            return ;
        }
        // channel name is not valid or channel does not exist
        if (!server.isChannelExist(words[2]) || !isValidChannelName(words[2]))
        {
            std::string errorMsg = ":" + server.get_hostnames() + " " + to_string(ERR_NOSUCHCHANNEL) + " " + server.get_nickname(fd) + " " + words[2] + " :No such channel\r\n";
            send(fd, errorMsg.c_str(), errorMsg.length(), 0);
            return ;
        }
        // sender is exist in channnel
        if (!server.isSenderInChannel(server.get_nickname(fd), words[2], server.getChannels()))
        {
            std::string errorMsg = ":" + server.get_hostnames() + " " + to_string(ERR_NOTONCHANNEL) + " " + server.get_nickname(fd) + " " + words[2] + " :You're not on that channel\r\n";
            send(fd, errorMsg.c_str(), errorMsg.length(), 0);
            return ;
        }
        // client is already in the channel
        if (server.isClientInChannel(words[1], words[2], server.getChannels()))
        {
            std::string errorMsg = ":" + server.get_hostnames() + " " + to_string(ERR_USERONCHANNEL) + " " + server.get_nickname(fd) + " " + words[1] + " " + words[2] + " :is already on channel\r\n";
            send(fd, errorMsg.c_str(), errorMsg.length(), 0);
            return ;
        }
        // client is Operator in that Channel
        if (!server.isClientOperatorInChannel(server.get_nickname(fd), words[2], server.getChannels()))
        {
            std::string errorMsg = ":" + server.get_hostnames() + " " + to_string(ERR_CHANOPRIVSNEEDED) + " " + server.get_nickname(fd) + " " + words[2] + " :You're not channel operator\r\n";
            send(fd, errorMsg.c_str(), errorMsg.length(), 0);
            return ;
        }
        else
        {
            int fdRe = 0;
            std::string name;
            for (std::map<int , Client *>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
            {
                if ((*it).second->getNick() == words[1]){
                    fdRe = (*it).first;
                    name = (*it).second->getNick();
                    (*it).second->setInviteToChannel(true);
                    break;
                }
            }
            std::string inviteMsg = ":" + server.get_nickname(fd) + " INVITE " + words[1] + " " + words[2] + "\r\n";
            send(fdRe, inviteMsg.c_str(), inviteMsg.length(), 0);
            std::string inviteSent = ":" + server.get_nickname(fd) + " " + to_string(RPL_INVITING) + " " + server.get_nickname(fd) + " " + words[1] + " " + words[2] + "\r\n";
			send(fd, inviteSent.c_str(), inviteSent.size(), 0);
        }
    }
}
