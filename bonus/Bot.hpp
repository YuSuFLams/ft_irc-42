/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 02:50:47 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/10 08:47:10 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
#ifndef BOT_HPP
#define BOT_HPP

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <cstring>
#include <fcntl.h>
#include <cstddef> 
#include <sstream>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <arpa/inet.h>
class Bot  {
    private:
        std::string name;
        std::string nickname;
        std::string username;
        std::string fullname;
        std::string servername;
        std::string hostname;

        std::map<std::string, float> Student_13;
        bool flagMode;
		std::string allstring;

    public:
        Bot();
        ~Bot();
        Bot(Bot &b);
        Bot& operator=(Bot &b);
        
        void    ConnetToServer(int , std::string &);
        std::string &get_allstring() { return allstring; }
		void set_allstring(std::string str) { allstring = str; }
        std::map<std::string, float> getStudent_13() { return (this->Student_13);}

        void 	traitResvedData(std::string &msg, int client_fd);
        void    comdBot(std::vector<std::string> &words, int fd);
        void    comdlevel(std::vector<std::string> &words, int fd);
        void    comdhelp(std::vector<std::string> &words, int fd);
        
};
void	split(std::string msg, std::vector<std::string> &SplitedMsg);
void    splitCommand(std::string str, char oper, std::vector<std::string> &SplitedMsg);
#endif