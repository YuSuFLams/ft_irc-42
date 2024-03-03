/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:09:08 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/02 23:03:45 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::PartChannel(std::vector<std::string> strs, std::map<std::string, Channel *> &channels, int fd, std::string nickname, Server &server) 
{
    if (strs.size() < 2)
        return -1;

    std::stringstream ss(strs[1]);
    std::string msg;
    std::vector<std::string> all_channels;

    // If the user is trying to leave multiple channels
    if (strs[1].find(',') != std::string::npos) 
    {
        std::string token;
        while (std::getline(ss, token, ',')) 
        {
            all_channels.push_back(token);
        }
        token.clear();
    } 
    else 
        all_channels.push_back(strs[1]);

    for (std::vector<std::string>::iterator it = all_channels.begin(); it != all_channels.end(); ++it) 
    {
        std::string channel_name = *it;
        if (channel_name[0] != '#' && channel_name[0] != '&') 
        {
            msg = ":" + server.get_hostnames() + " " + server.to_string(ERR_BADCHANMASK) + " " + nickname + " " + channel_name + " :Bad channel mask\r\n";
            send(fd, msg.c_str(), msg.length(), 0);
            continue;
        }

       // Check if the channel exists
        if (channels.find(channel_name) == channels.end()) 
        {
            msg = ":" + server.get_hostnames() + " 403 " + nickname + " " + channel_name + " :No such channel\r\n";
            send(fd, msg.c_str(), msg.length(), 0);
            continue;
        }

        Channel *channel = channels[channel_name];

        // Check if the user is in the channel before removing them
        if (channel->getUsers().find(nickname) == channel->getUsers().end()) 
        {
            msg = ":" + server.get_hostnames() + " 442 " + nickname + " " + channel_name + " :You're not on that channel\r\n";
            send(fd, msg.c_str(), msg.length(), 0);
            continue;
        }

      
        // channel->print_users();
        msg = ":" + server.get_nickname(fd) + "!" + server.get_realname(fd) + "@" + server.get_hostnames() + " PART " + channel_name + "\r\n";

        // Send to all users in the channel
        for (std::set<std::string>::iterator it = channel->getUsers().begin(); it != channel->getUsers().end(); ++it) 
        {
            int user_fd = server.get_fd_users(*it);
            send(user_fd, msg.c_str(), msg.length(), 0);
        }
        msg.clear();

        // Remove the user from the channel
        channel->removeUser(nickname);
        if(channel->isOperator("@" + nickname))
            channel->removeOperator("@" + nickname);
        // If the channel is empty after the user leaves, remove it from the map
        if (channel->getUsers().empty()) 
        {
            delete channel;  // Free the memory if the channel is empty
            channels.erase(channel_name);
        }
    }
    all_channels.clear();
    ss.clear();
    msg.clear();
    return 0;
}
