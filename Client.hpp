/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 17:41:59 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/01 20:55:35 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>

class Client
{
    private:
        int fd;
        std::string password;
        std::string nickname;
        std::string username;
        std::string realname;
        std::string hostname;
        std::string servername;
        int is_registered;
        bool InviteToChannel;
    public:
        Client()
        {
            this->fd = 0;
            this->password = "";
            this->nickname = "";
            this->username = "";
            this->realname = "";
            this->hostname = "";
            this->servername = "";
            this->is_registered = 0;
            this->InviteToChannel = false;
        }
        int getFd()
        {
            return (this->fd);
        }
        bool getInviteToChannel()
        {
            return (this->InviteToChannel);
        }
        void setInviteToChannel(bool InviteToChannel)
        {
            this->InviteToChannel = InviteToChannel;
        }
        
        std::string getPassword()
        {
            return (this->password);
        }
        std::string getNickname()
        {
            return (this->nickname);
        }
        std::string getUsername()
        {
            return (this->username);
        }
        std::string getRealname()
        {
            return (this->realname);
        }

        std::string getServername()
        {
            return (this->servername);
        }
        std::string getHostname()
        {
            return (this->hostname);
        }
        void setNickname(std::string nickname)
        {
            this->nickname = nickname;
        }
        void setServername(std::string servername)
        {
            this->servername = servername;
        }
        void setUsername(std::string username)
        {
            this->username = username;
        }
        void setRealname(std::string realname)
        {
            this->realname = realname;
        }
        void setHostname(std::string hostname)
        {
            this->hostname = hostname;
        }
        void setFd(int fd)
        {
            this->fd = fd;
        }
        void setPassword(std::string password)
        {
            this->password = password;
        }

        int is_Registered()
        {
            return (this->is_registered);
        }
        void set_is_Registered(int is_registered)
        {
            this->is_registered = is_registered;
        }
       
};

#endif