/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:09:08 by abel-hid          #+#    #+#             */
/*   Updated: 2024/02/22 22:31:24 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::PartChannel(std::vector<std::string> strs ,std::map<std::string, Channel *> &channels,  int fd, std::string nickname,  Server &server)
{
    std::string channel_str = strs[1];
    std::string token;
    std::stringstream ss(channel_str);
    std::vector<std::string> all_channels;
    size_t comma_count = 0;

    // Split the string by comma
    for (size_t i = 0; i < channel_str.length(); ++i) {
        if (channel_str[i] == ',') {
            comma_count++;
        }
    }

    if (comma_count == 0) 
    {
        all_channels.push_back(channel_str);
    } 
    else 
    {
        size_t start_pos = 0;
        for (size_t i = 0; i < channel_str.length(); ++i) {
            if (channel_str[i] == ',') 
            {
                token = channel_str.substr(start_pos, i - start_pos);
                all_channels.push_back(token);
                start_pos = i + 1;
            }
        }
        token = channel_str.substr(start_pos);
        all_channels.push_back(token);
    }

    if (all_channels.size() != comma_count + 1) 
    {
        return -1;
    }

    for (size_t i = 0; i < all_channels.size(); ++i) 
    {
        if (channels[all_channels[i]]->getUsers().find(nickname) == channels[all_channels[i]]->getUsers().end()) 
        {
            std::string msg = ":" + server.get_hostnames() + " 442 " + nickname + " " + all_channels[i] + " :You're not on that channel\r\n";
            send(fd, msg.c_str(), msg.length(), 0);
            continue;
        }
        channels[all_channels[i]]->removeUser(nickname);
        std::string msg = ":" + nickname + "!" + nickname + "@" + server.get_hostnames() + " PART " + all_channels[i] + "\r\n";
        if (channels[all_channels[i]]->getUsers().empty()) 
        {
            channels.erase(all_channels[i]);
        }
        // send to all users in the channel
        for (std::set<std::string>::iterator it = channels[all_channels[i]]->getUsers().begin(); it != channels[all_channels[i]]->getUsers().end(); ++it) 
        {
            int fd = server.get_fd_users(*it);
            send(fd, msg.c_str(), msg.length(), 0);
        }
    }
    return 0;
}