/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 15:04:46 by araiteb           #+#    #+#             */
/*   Updated: 2024/01/26 15:21:40 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include<iostream>
#include<string>
#include <vector>
class Client
{
    private:
        std::string nickname;
        std::string pass;
        std::vector<std::string> users;
        int fd;
    public:
        Client();
        Client(Client &cl);
        Client& operator =(const Client &cl);
        ~Client();
        
        void   seTNick(std::string nick);
        void    seTPass(std::string pwd);
        Client   seTValue(std::string nick, std::string pwd);
         
        std::string     geTNick();
        std::string     geTPass();
        
        void            CheckNick(std::string NewNick);
        void            CheckOper(std::string buffer);
};


#endif