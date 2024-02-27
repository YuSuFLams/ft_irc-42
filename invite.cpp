/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 20:23:08 by ylamsiah          #+#    #+#             */
/*   Updated: 2024/02/27 01:23:00 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

// * * * * * * * * * * * * * * * * * //
// just add case Channel INVITE ONLY //
// * * * * * * * * * * * * * * * * * //

#define ERR_NEEDMOREPARAMS_111(nickUser, command) ("461 " + nickUser + " " + command + " :Not enough parameters\r\n")

#define ERR_NOSUCHNICK_111(nickUser, nick) ("401 " + nickUser + " " + nick + " :No such nick\r\n")

#define ERR_NOSUCHCHANNEL_111(nickUser, channel) ("403 " + nickUser + " " + channel + " :No such channel\r\n")

#define ERR_NOTONCHANNEL_111(nickUser, channel) ("442 " + nickUser + " " + channel + " :You're not on that channel\r\n")

#define ERR_CHANOPRIVSNEEDED_111(nickUser, channel) ("482 " + nickUser + " " + channel + " :You're not channel operator\r\n")

#define ERR_USERONCHANNEL_111(nickUser, nick, channel) ("443 " + nickUser + " " + nick + " " + channel + " :is already on channel\r\n")

#define RPL_INVITING 341 

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
        if (it->second->getNickname() == nickname)
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

bool Server::isClientInChannel(std::string nickname, std::map<std::string, Channel *> &channels)
{
    std::map<std::string, Channel *>::iterator it;
    for (it = channels.begin(); it != channels.end(); it++)
    {
        if (it->second->isUser(nickname))
            return (true);
    }
    return (false);
}

bool Server::isClientOperatorInChannel(std::string nickname, std::string channelname, std::map<std::string, Channel *> &channels)
{
    std::map<std::string, Channel *>::iterator it;
    for (it = channels.begin(); it != channels.end(); it++)
    {
        if (it->first == channelname)
        {
            if (it->second->isOperator(nickname))
                return (true);
        }
    }
    return (false);
}

bool isSenderInChannel(std::string nickname, std::string channelname, std::map<std::string, Channel *> &channel)
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
        if (it->second->getNickname() == nick)
            return it->second;
    return NULL;
}

void Server::invitecmd(std::vector<std::string> words, Server server, int fd)
{
    if (words[1].empty() || words[2].empty())
    {
        std::string errorMsg = ERR_NEEDMOREPARAMS_111(server.get_hostnames(), words[0]);
        send(fd, errorMsg.c_str(), errorMsg.length(), 0);
    }
    else 
    {
        // client does not exist
        if (!server.isClientExist(words[1]))
        {
            std::string errorMsg = ERR_NOSUCHNICK_111(server.get_hostnames(), words[1]);
            send(fd, errorMsg.c_str(), errorMsg.length(), 0);
            return ;
        }
        // channel name is not valid or channel does not exist
        if (!server.isChannelExist(words[2]) || !isValidChannelName(words[2]))
        {
            std::string errorMsg = ERR_NOSUCHCHANNEL_111(server.get_hostnames(), words[2]);
            send(fd, errorMsg.c_str(), errorMsg.length(), 0);
            return ;
        }
        // sender is exist in channnel
        if (!isSenderInChannel(server.get_nickname(fd), words[2], server.getChannels()))
        {
            std::string errorMsg = ERR_NOTONCHANNEL_111(server.get_hostnames(), words[2]);
            send(fd, errorMsg.c_str(), errorMsg.length(), 0);
            return ;
        }
        // client is already in the channel
        if (server.isClientInChannel(words[1], server.getChannels()))
        {
            std::string errorMsg = ERR_USERONCHANNEL_111(server.get_hostnames(), words[1], words[2]);
            send(fd, errorMsg.c_str(), errorMsg.length(), 0);
            return ;
        }
        // client is Operator in that Channel
        if (!server.isClientOperatorInChannel(server.get_nickname(fd), words[2], server.getChannels()))
        {
            std::string errorMsg = ERR_CHANOPRIVSNEEDED_111(server.get_hostnames(), words[2]);
            send(fd, errorMsg.c_str(), errorMsg.length(), 0);
            return ;
        }
        else
        {
            int fdRe;
            std::string name;
            for (std::map<int , Client *>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
            {
                if ((*it).second->getNickname() == words[1]){
                    fdRe = (*it).first;
                    name = (*it).second->getNickname();
                    break;
                }
            }
            std::string inviteMsg = ":" + server.get_nickname(fd) + " INVITE " + words[1] + " to :" + words[2] + "\r\n";
            send(fdRe, inviteMsg.c_str(), inviteMsg.length(), 0);
            std::string inviteSent = ":" + server.get_hostnames() + " " + to_string(RPL_INVITING) + " " + server.get_nickname(fd) + " :Inviting " + words[1] + " to " + words[2] + "\r\n";
			send(fd, inviteSent.c_str(), inviteSent.size(), 0);
        }
    }
}
