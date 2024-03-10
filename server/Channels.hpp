/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 03:03:46 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/09 03:34:47 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include <iostream>
#include <vector>
#include <string>
#include <set>

#define RPL_INVITING 341 
#define RPL_TOPIC 332
#define RPL_NOTOPIC 331
#define ERR_NOTONCHANNEL 442
#define ERR_NEEDMOREPARAMS 461
#define ERR_NOSUCHCHANNEL 403
#define ERR_NOTOPIC 331
#define ERR_NICKNAMEINUSE 433
#define ERR_NOSUCHNICK 401
#define ERR_NOSUCHSERVER 402
#define ERR_NOSUCHCHANNEL 403
#define ERR_CANNOTSENDTOCHAN 404
#define RPL_TOPICWHOTIME 333
#define RPL_ENDOFNAMES 366
#define ERR_BADCHANNELKEY 475
#define ERR_BADCHANMASK 476
#define RPL_NAMREPLY 353
#define ERR_USERONCHANNEL 443
#define ERR_CHANOPRIVSNEEDED 482

class Channel
{
    private:
        std::string name;
        std::string topic;
        std::string password;
        bool inviteOnly;
        bool topicRestriction;
        bool creater;
        std::string channelKey;
        std::set<std::string> operators;
        std::set<std::string> users;
        long limit;
    public:
        Channel(const std::string& channelName, const std::string& channelTopic = "no topic is set", const std::string& channelPassword = ""  , bool creater = false);
        void setChannelName(const std::string& channelName);
        void setChannelTopic(const std::string& channelTopic);
        void setChannelPassword(const std::string& channelPassword);
        void setInviteOnly(bool isInviteOnly);
        void setTopicRestriction(bool isTopicRestriction);
        bool getInviteOnly();
        void setChannelKey(const std::string& key);
        void set_topic(const std::string& topic);
        void set_creater(bool creater);
        std::string get_topic();
        void addUser(const std::string& nickname);
        void removeUser(const std::string& nickname);
        std::string getChannelName();
        std::string getChannelTopic();
        std::string getChannelPassword();
        bool isInviteOnly();
        bool isTopicRestriction();
        std::set<std::string> &getUsers();
        std::set<std::string> getOperators() ;
        bool isOperator(const std::string& nickname);
        bool isUser(const std::string& nickname);
        long getLimit();
        void removeLimit();
        void removeChannelKey();
        void setLimit(long limit);
        void print_users();
        void quit_channel(std::string nickname);
        std::string get_creator_name();
        void removeOperator(const std::string& user);
        void addOperator(const std::string& user);
        std::string getChannelKey();
        bool get_creater();
    
};





#endif