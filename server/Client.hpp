/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 15:04:46 by araiteb           #+#    #+#             */
/*   Updated: 2024/02/06 14:45:41 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include<iostream>
#include<string>
#include <vector>
#include "server.hpp"
class Client
{
    private:
        std::string nickname;
        std::string pass;
        std::string username;
        std::string hostname;
        std::string servername;
        std::string realname;
        int fd;
    public:
        Client();
        Client(int fd);
        Client(Client &cl);
        Client& operator =(const Client &cl);
        ~Client();
        
        void    seTNick(std::string nick);
        void    seTPass(std::string pwd);
        void    setFdUser(int FdUser);
        void    seTValues(std::string str0, std::string str1, std::string str2, std::string str3);
         
        int             getFd();
        std::string     getNick();
        std::string     geTPass();
        std::string  getClient();
};


#endif