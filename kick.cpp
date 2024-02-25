/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 19:29:10 by abel-hid          #+#    #+#             */
/*   Updated: 2024/02/25 18:13:43 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::KickChannel(std::vector<std::string> strs, std::map<std::string, Channel *> &channels, int fd, std::string nickname, Server &server)
{
    // Check if the command has enough parameters
    if (strs.size() < 3)
    {
        std::string str = ":" + server.get_hostnames() + " " + server.to_string(ERR_NEEDMOREPARAMS) + " " + server.get_nickname(fd) + " KICK :Not enough parameters\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return;
    }

    // Check if the channel exists
    if (channels.find(strs[1]) == channels.end())
    {
        std::string message = ":" + server.get_hostnames() + " 403 " + nickname + " " + strs[1] + " :No such channel\r\n";
        send(fd, message.c_str(), message.length(), 0);
        return;
    }
    
    // Check if the user is an operator
    if (channels[strs[1]]->isOperator(nickname) == false)
    {
        std::string str = ":" + server.get_hostnames() + " 482 " + server.get_nickname(fd) + " " + strs[1] + " :You're not a channel operator\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return;
    }

    // Check if the user is in the channel
    if (channels[strs[1]]->getUsers().find(strs[2]) == channels[strs[1]]->getUsers().end())
    {
        std::string str = ":" + server.get_hostnames() + " 442 " + server.get_nickname(fd) + " " + strs[1] + " :User " + strs[2] + " is not on that channel\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return;
    }

    // Prepare and send kick message to all users in the channel
    std::string message = ":" + nickname + "!" + server.get_username(fd) + "@" + server.get_hostnames() + " KICK " + strs[1] + " " + strs[2] + "\r\n";
    std::set<std::string>::iterator it = channels[strs[1]]->getUsers().begin();
    while (it != channels[strs[1]]->getUsers().end())
    {
        if (*it != strs[2])
            send(server.get_fd_users(*it), message.c_str(), message.length(), 0);
        it++;
    }

    
    // Remove the user from the channel
    channels[strs[1]]->removeUser(strs[2]);
}
