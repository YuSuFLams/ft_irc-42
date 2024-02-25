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


void Server::handleChannels(std::vector<std::pair<std::string, std::string> >& pairs, int fd, const std::string& nickname , Server &server)
{
    std::map<std::string, Channel*>& channels = server.getChannels();
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

            std::string msg = ":" + server.get_hostnames() + " " + server.to_string(ERR_NOSUCHCHANNEL) + " " + nickname + " " + it->first + " :No such channel\r\n";
            send(fd, msg.c_str(), msg.length(), 0);
            continue;
        } 
        else
        {
            if(it->first.at(0) == '#')
            {
                std::map<std::string, Channel*>::iterator it2 = channels.find(it->first);
                if (it2 != channels.end() && it2->second->isUser(nickname)) 
                {
                    // User is already in the channel
                    std::string msg = ":" + server.get_hostnames() + " 443 " + nickname + " " + it->first + " :is already on channel\r\n";
                    send(fd, msg.c_str(), msg.length(), 0);
                    continue;
                }
                
                if (it2 == channels.end()) 
                {
                    // Channel doesn't exist, create it
                    Channel* newChannel = new Channel(it->first);
                    newChannel->addUser(nickname); // Add user to the channel
                    newChannel->addOperator(nickname); // Make the user an operator
                    newChannel->set_creater(true);
                    if(!it->second.empty())
                        newChannel->setChannelKey(it->second); // Set the channel key
                    channels[it->first] = newChannel; // Add the channel to the map

                    // reply to the user
                    std::string msg = ":" + nickname + "!" + server.get_username(fd) + "@" + server.get_hostnames() + " JOIN " + it->first + "\r\n";
                    send(fd, msg.c_str(), msg.length(), 0);
                    std::string topicMessage = ":" + server.get_hostnames() + " 313 " + nickname + " " + it->first + " :" + server.get_topic(it->first) + "\r\n";
                    send(fd, topicMessage.c_str(), topicMessage.length(), 0);
                    
                    std::string creatorMessage = ":" + server.get_hostnames() + " " + server.to_string(RPL_TOPICWHOTIME) + " " + nickname + " = " + it->first + " :@" + server.get_creator_name(it->first) + "\r\n";
                    send(fd, creatorMessage.c_str(), creatorMessage.length(), 0);

                    std::string endOfNamesMessage = ":" + server.get_hostnames() + " " + server.to_string(RPL_ENDOFNAMES) + " " + nickname + " " + it->first + " :End of /NAMES list\r\n";
                    send(fd, endOfNamesMessage.c_str(), endOfNamesMessage.length(), 0);

                } 
                else 
                {
                    // Channel exists, add the user to the channel
                    if(it->second == channels[it->first]->getChannelKey()) 
                    {
                        // broadcast to all users in the channel
                        std::string msg = ":" + nickname + "!" + server.get_username(fd) + "@" + server.get_hostnames() + " JOIN " + it->first + "\r\n";
                        join_broadcast_msg(channels, msg, server, it->first);

                        // add user to the channel
                        it2->second->addUser(nickname);
                    
                        // reply to the user
                        std::string str;
                    if(server.get_topic(it->first) == "No topic is set")
                    {
                         str = ":" + server.get_hostnames() + " 313 " + nickname + " " + it->first + " :No topic is set\r\n";
                        send(fd, str.c_str(), str.length(), 0);
                    }
                    else
                    {
                        std::string str = ":" + server.get_hostnames() + " " + server.to_string(RPL_TOPIC) + " " + nickname + " " + it->first + " :" + server.get_topic(it->first) + "\r\n";
                        send(fd, str.c_str(), str.length(), 0);
                    }

                    
                    str = ":" + server.get_hostnames() + " " + server.to_string(RPL_TOPICWHOTIME) + " " + nickname + " = " + it->first + " :@" + server.get_creator_name(it->first) + "\r\n";
                    send(fd, str.c_str(), str.length(), 0);
                
                    str =  ":" + server.get_hostnames() + " " + server.to_string(RPL_ENDOFNAMES) + " " + nickname + " " + it->first + " :End of /NAMES list\r\n";
                    send(fd, str.c_str(), str.length(), 0);
                        
                    } 
                    else 
                    {
                        // invalid key
                       std::string msg = ":" + server.get_hostnames() + " " + server.to_string(ERR_BADCHANNELKEY) + " " + nickname + " " + it->first + " :Cannot join channel (+k)\r\n";
                       send(fd, msg.c_str(), msg.length(), 0);
                    }
                }
            } 
            else
            {
                if(it->first.at(0) == '&') 
                {
                    std::map<std::string, Channel*>::iterator it2 = channels.find(it->first);
                    if (it2 != channels.end() && it2->second->isUser(nickname)) 
                    {
                        // User is already in the channel
                        std::string msg = ":" + server.get_hostnames() + " 443 " + nickname + " " + it->first + " :is already on channel\r\n";
                        send(fd, msg.c_str(), msg.length(), 0);
                        continue;
                    }
                    if (it2 == channels.end()) 
                    {
                        // Channel doesn't exist, create it
                        Channel* newChannel = new Channel(it->first);

                         std::string msg = ":" + nickname + "!" + server.get_username(fd) + "@" + server.get_hostnames() + " JOIN " + it->first + "\r\n";
                        join_broadcast_msg(channels, msg, server, it->first);

                        newChannel->addUser(nickname); // Add user to the channel
                        newChannel->addOperator(nickname); // Make the user an operator
                        newChannel->setChannelKey(it->second);
                        channels[it->first] = newChannel;
                        // reply to the user
                        std::string str = ":" + nickname + "!" + server.get_username(fd) + "@" + server.get_hostnames() + " JOIN " + it->first + "\r\n";
                        send(fd, str.c_str(), str.length(), 0);
                        
                        str = ":" + server.get_hostnames() + " 331 " + nickname + " " + it->first + " :" + server.get_topic(it->first) + "\r\n";
                        send(fd, str.c_str(), str.length(), 0);

                        str = ":" + server.get_nickname(fd) + "!" + server.get_username(fd) + "@" + server.get_hostnames() + " JOIN " + it->first + "\r\n";
                        str = ":" + server.get_hostnames() + " " + server.to_string(RPL_TOPICWHOTIME) + " " + nickname + " = " + it->first + " :@" + server.get_creator_name(it->first) + "\r\n";
                        send(fd, str.c_str(), str.length(), 0);
                    
                        str =  ":" + server.get_hostnames() + " " + server.to_string(RPL_ENDOFNAMES) + " " + nickname + " " + it->first + " :End of /NAMES list\r\n";
                        send(fd, str.c_str(), str.length(), 0);
                    } 
                    else 
                    {
                        // Channel exists, add the user to the channel
                        if(it->second == channels[it->first]->getChannelKey()) 
                        {
                             std::string msg = ":" + nickname + "!" + server.get_username(fd) + "@" + server.get_hostnames() + " JOIN " + it->first + "\r\n";
                            join_broadcast_msg(channels, msg, server, it->first);
                            
                            it2->second->addUser(nickname);
                            std::string str;

                            if(server.get_topic(it->first) == "No topic is set")
                            {
                                str = ":" + server.get_hostnames() + " 331 " + nickname + " " + it->first + " :No topic is set\r\n";
                                send(fd, str.c_str(), str.length(), 0);
                            }
                            else
                            {
                                 str = ":" + server.get_hostnames() + " " + server.to_string(RPL_TOPIC) + " " + nickname + " " + it->first + " :" + server.get_topic(it->first) + "\r\n";
                                send(fd, str.c_str(), str.length(), 0);
                            }

                            send(fd, str.c_str(), str.length(), 0);
                            str = ":" + server.get_hostnames() + " " + server.to_string(RPL_TOPICWHOTIME) + " " + nickname + " = " + it->first + " :@" + server.get_creator_name(it->first) + "\r\n";
                            send(fd, str.c_str(), str.length(), 0);
                        
                            str =  ":" + server.get_hostnames() + " " + server.to_string(RPL_ENDOFNAMES) + " " + nickname + " " + it->first + " :End of /NAMES list\r\n";
                            send(fd, str.c_str(), str.length(), 0);
                        } 
                        else 
                        {
                            std::string msg = ":" + server.get_hostnames() + " " + server.to_string(ERR_BADCHANNELKEY) + " " + nickname + " " + it->first + " :Cannot join channel (+k)\r\n";
                            send(fd, msg.c_str(), msg.length(), 0);
                        }
                    }
                }
            }
        }
    }
}

		
int Server::JoinChannel(std::vector<std::string > strs , std::string nickname, int fd, Server &server)
{ 
    std::vector<std::string> channels;
    std::vector<std::string> keys;

    std::vector<std::pair<std::string, std::string> > pair;
    
    std::stringstream ss(strs[1]);
    if (strs.size() >= 3) 
    {
        if (!strs[2].empty() && strs[2].find(',') != std::string::npos) 
        {
            std::stringstream ss2(strs[2]);
            std::string token_keys;
            while (std::getline(ss2, token_keys, ',')) 
            {
                keys.push_back(token_keys);
            }
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
    //     std::cout << "pair.first: " << it->first << " pair.second: " << it->second << std::endl;
    //     it++;
    // }

    handleChannels(pair, fd, nickname, server);
    return 0;
}