/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:59:22 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/09 10:15:25 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"

void Server::topic_broadcast_msg(std::map<std::string, Channel*>& channels, const std::string& channelName, const std::string& nickname)
{
    std::map<std::string, Channel*>::iterator it = channels.find(channelName);
    if (it != channels.end()) 
    {
        std::set<std::string>::iterator it2 = it->second->getUsers().begin();
        while (it2 != it->second->getUsers().end()) 
        {
            int fd = this->get_fd_users(*it2);
            std::string msg = ":" + nickname + "!" + this->get_username(fd) + "@" + this->get_hostnames() + " TOPIC " + channelName + " :" + it->second->get_topic() + "\r\n";
            send(this->get_fd_users(*it2), msg.c_str(), msg.length(), 0);
            it2++;
        }
    }
}

int Server::TopicChannel(std::vector<std::string> strs, std::map<std::string, Channel *> &channels, int fd) 
{
    if (strs.size() >= 2 && strs[0] == "TOPIC") 
    {
        if(strs.size() > 1 && (strs[1].compare(":") == 0 || strs[1].compare("::") == 0))
        {
            std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_BADCHANMASK) + " " + this->get_nickname(fd) + " " + strs[1] + " :Bad Channel Mask\r\n";
            send(fd, str.c_str(), str.length(), 0);
            return -2;
        }
        if(strs.size() > 3 && (strs[2].find(":") == std::string::npos && strs[2].size() != 1))
        {
            std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " " + this->get_nickname(fd) + " " + strs[1] + " :Not enough parameters\r\n";
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
            if (channels[strs[1]]->getUsers().find(this->get_nickname(fd)) == channels[strs[1]]->getUsers().end())
            {
                std::string str = ":" + this->get_hostnames() + " 442 " + this->get_nickname(fd) + " " + strs[1] + " :You're not on that channel\r\n";
                send(fd, str.c_str(), str.length(), 0);
                return -2;
            }
            
            // Channel exists and mode is set to private
            if(channels[strs[1]]->isTopicRestriction() == true  && channels[strs[1]]->isOperator("@" + this->get_nickname(fd)) == false)
            {
                std::string str = ":" + this->get_hostnames() + " " + "481 " + this->get_nickname(fd) + " " + strs[1] + " :Permission Denied- You're not an operator\r\n";
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
                        std::string str = ":" + this->get_hostnames() + " 331 " + this->get_nickname(fd) + " " + strs[1] + " :No topic is set\r\n";
                        send(fd, str.c_str(), str.length(), 0);
                    }
                    else
                    {
                        // Topic is set
                        std::string str = ":" + this->get_hostnames() + " 332 " + this->get_nickname(fd) + " " + strs[1] + " :" + channels[strs[1]]->get_topic() + "\r\n";
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
                        topic_broadcast_msg(channels, strs[1], this->get_nickname(fd));
                        // Send the new topic to all users in the channel
                    }
                    else
                    {
                        channels[strs[1]]->set_topic(strs[2]);
                        topic_broadcast_msg(channels, strs[1], this->get_nickname(fd));
                    }
                }
            }
        }
    }
    return 0;
}