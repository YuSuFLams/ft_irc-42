/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 19:29:10 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/03 01:12:44 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::KickChannel(std::vector<std::string> strs, std::map<std::string, Channel *> &channels, int fd, std::string nickname, Server &server , std::string str)
{
    // Check if the command has enough parameters
    if (strs.size() < 3)
    {
        std::string str = ":" + server.get_hostnames() + " " + server.to_string(ERR_NEEDMOREPARAMS) + " " + server.get_nickname(fd) + " KICK :Not enough parameters\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return;
    }
    else if(strs.size() > 4 && strs[3].at(0) != ':')
    {
        std::string str = ":" + server.get_hostnames() + " " + server.to_string(ERR_NEEDMOREPARAMS) + " " + server.get_nickname(fd) + " KICK :Not enough parameters\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return;
    }
    std::vector<std::string> kick_channel;
    std::vector<std::string> users;
    if(strs[1].find(',') != std::string::npos) 
    {
        std::stringstream ss(strs[1]);
        std::string token;
        while (std::getline(ss, token, ',')) 
        {
            kick_channel.push_back(token);
        }
    } 
    else 
    {
        kick_channel.push_back(strs[1]);
    }
    if(strs[2].find(',') != std::string::npos) 
    {
        std::stringstream ss(strs[2]);
        std::string token;
        while (std::getline(ss, token, ',')) 
        {
            users.push_back(token);
        }
    } 
    else 
    {
        users.push_back(strs[2]);
    }
    // Check if the channel exists
    for(std::vector<std::string>::iterator it = kick_channel.begin(); it != kick_channel.end(); it++)
    {
        if (channels.find(*it) == channels.end())
        {
            std::string message = ":" + server.get_hostnames() + " 403 " + nickname + " " + *it + " :No such channel\r\n";
            send(fd, message.c_str(), message.length(), 0);
            continue;
        }
        // Check if the user is in the channel
        if(server.isClientInChannel(nickname, *it, channels) == false)
        {
            std::string message = ":" + server.get_hostnames() + " 442 " + nickname + " " + *it + " :You're not on that channel\r\n";
            send(fd, message.c_str(), message.length(), 0);
            continue;
        }
        
        for(std::vector<std::string>::iterator it1 = users.begin(); it1 != users.end(); it1++)
        {
            // Check if the user is an operator
            // Check if the user is in the channel
            if (channels[*it]->getUsers().find(*it1) == channels[*it]->getUsers().end())
            {
                std::string str = ":" + server.get_hostnames() + " 442 "  + *it + " :User " + *it1 + " is not on that channel\r\n";
                send(fd, str.c_str(), str.length(), 0);
                continue;
            }
            // print the operators
            if (channels[*it]->isOperator("@" + server.get_nickname(fd)) == false)
            {
                std::string str = ":" + server.get_hostnames() + " 482 " + nickname + " " + *it + " :You're not a channel operator\r\n";
                send(fd, str.c_str(), str.length(), 0);
                continue;
            }

            // reason for the kick
            std::string message;
            if(!strs[3].empty())
            {
                if(strs[3].at(0) == ':')
                strs[3] = str.substr(str.find(":") + 1 , str.length());
                message = ":" + nickname + "!" + server.get_username(fd) + "@" + server.get_hostnames() + " KICK " + *it + " " + *it1 + " :" + strs[3] + "\r\n";
            }
            else
                message = ":" + nickname + "!" + server.get_username(fd) + "@" + server.get_hostnames() + " KICK " + *it + " " + *it1 + "\r\n";

            // Send the message to the user
            std::set<std::string>::iterator it2 = channels[*it]->getUsers().begin();
            while (it2 != channels[*it]->getUsers().end())
            {
                send(server.get_fd_users(*it2), message.c_str(), message.length(), 0);
                it2++;
            }
            // Remove the user from the channel
            channels[*it]->removeUser(*it1);
            if(channels[*it]->get_creater() == true)
                channels[*it]->set_creater(false);
            // if operator remove him from the operators
            if (channels[*it]->isOperator("@" + *it1))
                channels[*it]->removeOperator("@" + *it1);
        }
    }
    kick_channel.clear();
    users.clear();
}
