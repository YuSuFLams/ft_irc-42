/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 02:50:47 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/13 20:41:00 by ylamsiah         ###   ########.fr       */
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
		std::string allstring;

    public:
        Bot();
        ~Bot();
        Bot(Bot &b);
        Bot& operator=(Bot &b);
        
        void    ConnetToServer(int , std::string &);
        void setStudent_13(std::map<std::string, float> Student_13) { this->Student_13 = Student_13;}
        std::map<std::string, float> getStudent_13() { return (this->Student_13);}
        std::map<std::string, float> first_read(std::string file);
        void 	traitResvedData(std::string &msg, int client_fd);
        void 	sendResponce(int fd, const std::string &responce);
        std::string   returntime();
        std::string comdBot(std::vector<std::string> &words);
        
};
void	split(std::string msg, std::vector<std::string> &SplitedMsg);
void    splitCommand(std::string str, char oper, std::vector<std::string> &SplitedMsg);
#endif