/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:09:08 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/15 22:45:07 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

int Server::PartChannel(std::vector<std::string> strs, std::map<std::string, Channel *> &channels, int fd, std::string nickname, std::string str)
{
    if (strs.size() < 2 )
        return -1;
    
    if((strs.size() > 3 && strs[2].at(0) != ':'))
    {
        std::string msg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " " + nickname + " PART :Not enough parameters\r\n";
        send(fd, msg.c_str(), msg.length(), 0);
        return -2;
    }

    std::stringstream ss(strs[1]);
    std::string msg;
    std::vector<std::string> all_channels;

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
    
    std::string reason = "";
    if(strs.size() > 3 && strs[2].at(0) == ':')
        reason = str.substr(str.find(":") + 1 , str.length());
    else
        reason = strs[2];

    std::cout << "Reason: " << reason << std::endl;
    for (std::vector<std::string>::iterator it = all_channels.begin(); it != all_channels.end(); ++it) 
    {
        std::string channel_name = *it;
        if (channel_name[0] != '#' && channel_name[0] != '&') 
        {
            msg = ":" + this->get_hostnames() + " " + this->to_string(ERR_BADCHANMASK) + " " + nickname + " " + channel_name + " :Bad channel mask\r\n";
            send(fd, msg.c_str(), msg.length(), 0);
            continue;
        }

       // Check if the channel exists
        if (channels.find(channel_name) == channels.end()) 
        {
            msg = ":" + this->get_hostnames() + " 403 " + nickname + " " + channel_name + " :No such channel\r\n";
            send(fd, msg.c_str(), msg.length(), 0);
            continue;
        }

        Channel *channel = channels[channel_name];

        // Check if the user is in the channel before removing them
        if (channel->getUsers().find(nickname) == channel->getUsers().end()) 
        {
            msg = ":" + this->get_hostnames() + " 442 " + nickname + " " + channel_name + " :You're not on that channel\r\n";
            send(fd, msg.c_str(), msg.length(), 0);
            continue;
        }

      
        // channel->print_users();
        if(reason.empty())
            msg = ":" + nickname + "!" + this->get_username(fd) + "@" + this->get_ip_address(fd) + " PART " + channel_name + "\r\n";
        else
            msg = ":" + nickname + "!" + this->get_username(fd) + "@" + this->get_ip_address(fd) + " PART " + channel_name + " :" + reason + "\r\n";

        // Send to all users in the channel
        for (std::set<std::string>::iterator it = channel->getUsers().begin(); it != channel->getUsers().end(); ++it) 
        {
            int user_fd = this->get_fd_users(*it);
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
            delete channel;
            channels.erase(channel_name);
        }
    }
    all_channels.clear();
    ss.clear();
    msg.clear();
    strs.clear();
    return 0;
}
