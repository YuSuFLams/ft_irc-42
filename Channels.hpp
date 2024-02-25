/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 03:03:46 by abel-hid          #+#    #+#             */
/*   Updated: 2024/02/24 21:22:46 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include <iostream>
#include <vector>
#include <string>
#include <set>
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
    public:
        Channel(const std::string& channelName, const std::string& channelTopic = "no topic is set", const std::string& channelPassword = ""  , bool creater = false)
        : name(channelName), topic(channelTopic), password(channelPassword), inviteOnly(false), topicRestriction(false) , creater(creater)
        {
        }

        // ~Channel();
        // Channel(Channel &ch);
        // Channel& operator=(const Channel &ch);

        // setters
        void setChannelName(const std::string& channelName) 
        { 
            name = channelName; 
        }
        void setChannelTopic(const std::string& channelTopic) 
        { 
            topic = channelTopic; 
        }
        void setChannelPassword(const std::string& channelPassword) 
        { 
            password = channelPassword; 
        }
        void setInviteOnly(bool isInviteOnly) 
        { 
            inviteOnly = isInviteOnly; 
        }
        void setTopicRestriction(bool isTopicRestriction) 
        { 
            topicRestriction = isTopicRestriction; 
        }
        void setChannelKey(const std::string& key) 
        { 
            channelKey = key; 
        }
        
        // getters
        std::string getChannelName() const 
        { 
            return name; 
        }
        std::string getChannelTopic() const 
        { 
            return topic; 
        }
        std::string getChannelPassword() const 
        { 
            return password; 
        }
        bool isInviteOnly() const 
        { 
            return inviteOnly; 
        }
        bool isTopicRestriction() const 
        { 
            return topicRestriction; 
        }
      
        std::string getChannelKey() const 
        { 
            return channelKey; 
        }
        std::set<std::string> getOperators() const 
        { 
            return operators; 
        }
        // channel operators
        void addOperator(const std::string& user) 
        { 
            operators.insert(user); 
        }
        void removeOperator(const std::string& user) 
        { 
            operators.erase(user); 
        }
        bool isOperator(const std::string& user) const 
        { 
            return operators.find(user) != operators.end(); 
        }
        
        void addUser(const std::string& user) 
        { 
            users.insert(user); 
        }
        void removeUser(const std::string& user) 
        { 
            users.erase(user); 
        }
        bool isUser(const std::string& user) const 
        { 
            return users.find(user) != users.end(); 
        }
        std::set<std::string> &getUsers()
        { 
            return users; 
        }

        std::string get_topic()
        {
            return (topic);
        }
        void set_topic(std::string topic)
        {
            this->topic = topic;
        }
        void set_creater(bool creater)
        {
            this->creater = creater;
        }
        bool get_creater()
        {
            return (creater);
        }

        std::string get_creator_name()
        {
            std::set<std::string>::iterator it = operators.begin();
            while(it != operators.end())
            {
                if (creater == true)
                    return (*it);
                it++;
            }
            if(operators.size() > 0)
                return (*operators.begin());
            return ("");
        }
        
        void quit_channel(std::string nickname)
        {
            if (users.find(nickname) != users.end())
                users.erase(nickname);
            if (operators.find(nickname) != operators.end())
                operators.erase(nickname);
        }
        
        void print_users()
        {
            std::set<std::string>::iterator it = users.begin();
            while(it != users.end())
            {
                std::cout << *it << std::endl;
                it++;
            }
        }
    
};





#endif