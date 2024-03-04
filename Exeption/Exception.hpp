/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 23:09:36 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/04 08:46:42 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  EXCEPTION_HPP
# define EXCEPTION_HPP

#include<iostream>
#include<string>

enum e_rronum{
    ERR_NEEDMOREPARAMS = 461,
    ERR_ALREADYREGISTRED = 462,
    
    ERR_NONICKNAMEGIVEN = 431,
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433,
    ERR_NICKCOLLISION = 436,


    ERR_NORECIPIENT = 411,
    ERR_CANNOTSENDTOCHAN = 404,
    ERR_WILDTOPLEVEL = 414,
    ERR_NOSUCHNICK = 401,
    ERR_NOTEXTTOSEND = 412,
    ERR_NOTOPLEVEL = 413,
    ERR_TOOMANYTARGETS = 407,
    ERR_PASSWDMISMATCH = 464,
    ERR_UNKNOWNCOMMAND = 421
};

enum  e_repl{
    RPL_AWAY = 301
};

class Myexception : public std::exception{
         private:
            int err_num;
            std::string err_msg;
        public:
            Myexception(std::string msg) : err_msg(msg) {}
            Myexception(int err_number) {
                err_num = err_number;
                err_msg = msgError(err_num);
            }
            virtual ~Myexception() throw() {}
            

            std::string msgError(int num);
            char const *    what() const throw() {
                return (err_msg.c_str());
            }
            int                getERROR_NO() {
                return (err_num);
            }
};
#endif