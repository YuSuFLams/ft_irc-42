/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:59:22 by abel-hid          #+#    #+#             */
/*   Updated: 2024/02/27 17:35:16 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void topic_broadcast_msg(std::map<std::string, Channel*>& channels, const std::string& channelName, const std::string& nickname, Server &server)
{
    std::map<std::string, Channel*>::iterator it = channels.find(channelName);
    if (it != channels.end()) 
    {
        std::set<std::string>::iterator it2 = it->second->getUsers().begin();
        while (it2 != it->second->getUsers().end()) 
        {
                std::string msg = ":" + nickname + "!" + nickname + "@" + server.get_hostnames() + " TOPIC " + channelName + "\r\n";
                send(server.get_fd_users(*it2), msg.c_str(), msg.length(), 0);
            it2++;
        }
    }
}

int Server::TopicChannel(std::vector<std::string> strs, std::map<std::string, Channel *> &channels, int fd, Server &server) 
{
    if (strs.size() >= 2 && strs[0] == "TOPIC") 
    {
        if(strs.size() > 1 && (strs[1].compare(":") == 0 || strs[1].compare("::") == 0))
        {
            std::string str = ":" + server.get_hostnames() + " " + server.to_string(ERR_BADCHANMASK) + " " + server.get_nickname(fd) + " " + strs[1] + " :Bad Channel Mask\r\n";
            send(fd, str.c_str(), str.length(), 0);
            return -2;
        }
        if (channels.find(strs[1]) == channels.end()) 
        {
            // Channel doesn't exist
            return -1;
        }
        else 
        {
            // Channel exists and user is in it
            if (channels[strs[1]]->getUsers().find(server.get_nickname(fd)) == channels[strs[1]]->getUsers().end())
            {
                std::string str = ":" + server.get_hostnames() + " 442 " + server.get_nickname(fd) + " " + strs[1] + " :You're not on that channel\r\n";
                send(fd, str.c_str(), str.length(), 0);
                return -2;
            }
            else
            {
                if (strs.size() == 2) 
                {
                    // Get the topic
                    if (channels[strs[1]]->get_topic() == "No topic is set") 
                    {
                        // No topic is set
                        std::string str = ":" + server.get_hostnames() + " 331 " + server.get_nickname(fd) + " " + strs[1] + " :No topic is set\r\n";
                        send(fd, str.c_str(), str.length(), 0);
                    }
                    else
                    {
                        // Topic is set
                        std::string str = ":" + server.get_hostnames() + " 332 " + server.get_nickname(fd) + " " + strs[1] + " :" + channels[strs[1]]->get_topic() + "\r\n";
                        send(fd, str.c_str(), str.length(), 0);
                    }
                } 
                else if (strs.size() >= 3) 
                {
                    if (strs[2] == ":" && strs.size() == 3 && strs[2].size() == 1)
                    {
                        // Clear the topic
                        channels[strs[1]]->set_topic("No topic is set");
                    } 
                    else if (strs[2][0] == ':' && strs.size() >= 3)
                    {
                        // Set a new topic
                        std::string new_topic = strs[2].substr(1);
                            
                        for (size_t i = 3; i < strs.size(); ++i) 
                        {
                            new_topic += " " + strs[i];
                        }
                        channels[strs[1]]->set_topic(new_topic);
                        topic_broadcast_msg(channels, strs[1], server.get_nickname(fd), server);
                        // Send the new topic to all users in the channel
                    }
                    else
                    {
                        channels[strs[1]]->set_topic(strs[2]);
                        topic_broadcast_msg(channels, strs[1], server.get_nickname(fd), server);
                    }
                }
            }
        }
    }
    return 0;
}
