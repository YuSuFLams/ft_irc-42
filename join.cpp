/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 01:40:02 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/04 23:35:49 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


void join_broadcast_msg(std::map<std::string, Channel*>& channels , std::string msg, Server &server , std::string channelName)
{
    std::map<std::string, Channel*>::iterator it = channels.find(channelName);
    if (it != channels.end()) 
    {
        std::set<std::string>::iterator it2 = it->second->getUsers().begin();
        while (it2 != it->second->getUsers().end()) 
        {
            send(server.get_fd_users(*it2), msg.c_str(), msg.length(), 0);
            it2++;
        }
    }
}

int Server::public_channel(std::string channel_name , std::string key , int fd, Server &server)
{

    std::map<std::string, Channel*>::iterator it2 = channels.find(channel_name);
    if (it2 != channels.end() && it2->second->isUser(server.get_nickname(fd))) 
    {
        // User is already in the channel
        std::string msg = ":" + server.get_hostnames() + " 443 " + server.get_nickname(fd) + " " + channel_name + " :is already on channel\r\n";
        send(fd, msg.c_str(), msg.length(), 0);
        return (1);
    }
    // Check if the user is invited to the channel and the channel is invite only
    if(server.getInviteToChannel(fd) == true && channels[channel_name]->getInviteOnly() == true)
    {
        std::string msg = ":" + server.get_hostnames() + " 473 " + server.get_nickname(fd) + " " + channel_name + " :Cannot join channel (+i)\r\n";
        send(fd, msg.c_str(), msg.length(), 0);
        return (1);
    }
    
    // limit the number of channels a user can join
    if(server.get_limit(channel_name) != -1 && (size_t)server.get_limit(channel_name) <= channels[channel_name]->getUsers().size())
    {
        std::string msg = ":" + server.get_hostnames() + " 471 " + server.get_nickname(fd) + " " + channel_name + " :Cannot join channel (+l)\r\n";
        send(fd, msg.c_str(), msg.length(), 0);
        return (1);
    }

    if (it2 == channels.end()) 
    {
        // Channel doesn't exist, create it
        Channel* newChannel = new Channel(channel_name); // Create a new channel
        newChannel->addUser(server.get_nickname(fd)); // Add the user to the channel
        newChannel->addOperator("@" + server.get_nickname(fd)); // Make the user an operator with '@' prefix
        newChannel->set_creater(true);
        if(!key.empty())
            newChannel->setChannelKey(key); // Set the channel key
        channels[channel_name] = newChannel; // Add the channel to the map

        // reply to the user
        std::string msg = ":" + server.get_nickname(fd) + "!" + server.get_username(fd) + "@" + server.get_hostnames() + " JOIN " + channel_name + "\r\n";
        send(fd, msg.c_str(), msg.length(), 0);

        msg = ":" + server.get_hostnames() + " 313 " + server.get_nickname(fd) + " " + channel_name + " :" + server.get_topic(channel_name) + "\r\n";
        send(fd, msg.c_str(), msg.length(), 0);
        
        msg = ":" + server.get_hostnames() + " " + server.to_string(RPL_NAMREPLY) + " " + server.get_nickname(fd) + " = " + channel_name + " :@" + server.get_nickname(fd) + "\r\n"; // Prefix '@' to operator's name
        send(fd, msg.c_str(), msg.length(), 0);
        msg = ":" + server.get_hostnames() + " " + server.to_string(RPL_ENDOFNAMES) + " " + server.get_nickname(fd) + " " + channel_name + " :End of /NAMES list\r\n";
        send(fd, msg.c_str(), msg.length(), 0);
        msg.clear();
    }
    else
    {
        // Channel exists, add the user to the channel
        if (key == channels[channel_name]->getChannelKey()) 
        {
            // broadcast to all users in the channel
            std::string msg = ":" + server.get_nickname(fd) + "!" + server.get_username(fd) + "@" + server.get_hostnames() + " JOIN " + channel_name + "\r\n";
            join_broadcast_msg(channels, msg, server, channel_name);

            // add user to the channel
            it2->second->addUser(server.get_nickname(fd));
            send(fd, msg.c_str(), msg.length(), 0);

            // reply to the user
            std::string str;
            if (server.get_topic(channel_name) == "No topic is set")
            {
                str = ":" + server.get_hostnames() + " 313 " + server.get_nickname(fd) + " " + channel_name + " :No topic is set\r\n";
                send(fd, str.c_str(), str.length(), 0);
            }
            else
            {
                str = ":" + server.get_hostnames() + " " + server.to_string(RPL_TOPIC) + " " + server.get_nickname(fd) + " " + channel_name + " :" + server.get_topic(channel_name) + "\r\n";
                send(fd, str.c_str(), str.length(), 0);
            }

            str = ":" + server.get_hostnames() + " " + server.to_string(RPL_TOPICWHOTIME) + " " + server.get_nickname(fd) + " = " + channel_name + " :" + server.get_creator_name(channel_name) + "\r\n";
            send(fd, str.c_str(), str.length(), 0);

            // send the list of users in the channel
            std::set<std::string>::iterator it3 = channels[channel_name]->getUsers().begin();
            while (it3 != channels[channel_name]->getUsers().end())
            {
                std::string user = *it3;
                if (channels[channel_name]->isOperator("@" + user))
                    str = ":" + server.get_hostnames() + " " + server.to_string(RPL_NAMREPLY) + " " + server.get_nickname(fd) + " = " + channel_name + " :@" + user + "\r\n"; // Prefix '@' to operator's name
                else
                    str = ":" + server.get_hostnames() + " " + server.to_string(RPL_NAMREPLY) + " " + server.get_nickname(fd) + " = " + channel_name + " :" + user + "\r\n";
                send(fd, str.c_str(), str.length(), 0);
                it3++;
            }

            str = ":" + server.get_hostnames() + " " + server.to_string(RPL_ENDOFNAMES) + " " + server.get_nickname(fd) + " " + channel_name + " :End of /NAMES list\r\n";
            send(fd, str.c_str(), str.length(), 0);
            str.clear();
        } 
        else
        {
            // Channel has a key and the user didn't provide it
            std::string msg = ":" + server.get_hostnames() + " " + server.to_string(ERR_BADCHANNELKEY) + " " + server.get_nickname(fd) + " " + channel_name + " :Cannot join channel (+k)\r\n";
            send(fd, msg.c_str(), msg.length(), 0);
        }
       
    }
    return (0);
}

void Server::handleChannels(std::vector<std::pair<std::string, std::string> >& pairs, int fd, const std::string& nickname , Server &server)
{
    for(std::vector<std::pair<std::string, std::string> >::iterator it = pairs.begin(); it != pairs.end(); ++it) 
    {
        if(it->first.compare("#") == 0 || it->first.compare("&") == 0)
        {
            std::string msg = ":" + server.get_hostnames() + " " + server.to_string(ERR_BADCHANMASK) + " " + nickname + " " + it->first + " :Bad Channel Mask\r\n";
            send(fd, msg.c_str(), msg.length(), 0);
            continue;
        }
        if(it->first.at(0) != '#' && it->first.at(0) != '&') 
        {
            // Channel name is invalid
            std::string msg = ":" + server.get_hostnames() + " " + server.to_string(ERR_BADCHANMASK) + " " + nickname + " " + it->first + " :Bad Channel Mask\r\n";
            send(fd, msg.c_str(), msg.length(), 0);
            continue;
        } 
        else
        {
            if(it->first.at(0) == '#' || it->first.at(0) == '&')
            {
                if(public_channel(it->first, it->second, fd, server) == 1)
                    continue;
            }
        }
    // std::set<std::string>::iterator it3 = channels[it->first]->getUsers().begin();
    // while(it3 != channels[it->first]->getUsers().end())
    // {
    //     std::cout << "users in the channel: " << *it3 << std::endl;
    //     it3++;
    // }
    }
    pairs.clear();
}

		
int Server::JoinChannel(std::vector<std::string > strs , std::string nickname, int fd, Server &server , std::string str)
{ 
    std::vector<std::string> channels;
    std::vector<std::string> keys;

    // pair of channel and key
    if(strs.size() > 3 && strs[2].at(0) != ':')
    {
        std::string str = ":" + server.get_hostnames() + " " + server.to_string(ERR_NEEDMOREPARAMS) + " " + server.get_nickname(fd) + " JOIN :Not enough parameters\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return -2;
    }
    std::vector<std::pair<std::string, std::string> > pair;
    
    std::stringstream ss(strs[1]);


    if (strs.size() >= 3) 
    {
        if(!strs[2].empty() && strs[2].at(0) == ':')
        {
            // get the key after the ':'
            keys.push_back(str.substr(str.find(":") + 1 , str.length()));
            for (size_t i = 3; i < strs.size(); i++)
            {
                strs.pop_back();
            }
        }
        else if (!strs[2].empty() && strs[2].find(',') != std::string::npos) 
        {
            std::stringstream ss2(strs[2]);
            std::string token_keys;
            while (std::getline(ss2, token_keys, ',')) 
            {
                keys.push_back(token_keys);
            }
            token_keys.clear();
        } 
        else 
        {
            keys.push_back(strs[2]);
        }
    }
   
    
    if(strs[1].find(',') != std::string::npos) 
    {
        std::string token;
        while (std::getline(ss, token, ',')) 
        {
            channels.push_back(token);
        }
        token.clear();
    } 
    else 
    {
        channels.push_back(strs[1]);
    }


    if (channels.size() > 0) 
    {
        // std::cout << "-----------------channels------------------" << std::endl;
        std::vector<std::string>::iterator it = channels.begin();
        std::vector<std::string>::iterator it2 = keys.begin();
        while (it != channels.end() && it2 != keys.end()) 
        {
            pair.push_back(std::make_pair(*it, *it2));
            it++;
            it2++;
        }
        if(it != channels.end())
        {
            while(it != channels.end())
            {
                pair.push_back(std::make_pair(*it, ""));
                it++;
            }
        }
    }
    // std::cout << "-----------------pair------------------" << std::endl;
    // std::vector<std::pair<std::string, std::string> >::iterator it = pair.begin();
    // while(it != pair.end())
    // {
    //     std::cout << it->first << " | " << it->second << std::endl;
    //     it++;
    // }
    // std::cout << "-----------------------------------------" << std::endl;

    handleChannels(pair, fd, nickname, server);
    channels.clear();
    keys.clear();
    return 0;
}