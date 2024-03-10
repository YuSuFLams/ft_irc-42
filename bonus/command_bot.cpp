/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_bot.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 06:21:49 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/10 08:19:40 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

std::map<std::string, float> first_read(std::string file)
{
    std::ifstream file1(file);
    if (!file1.is_open())
    {
        std::cerr << "Error: Can't open file" << std::endl;
        exit (1);
    }
    std::string line;
    std::map<std::string, float> listStud;
    while (std::getline(file1, line))
    {
        if (line.empty())
            continue ;
        std::string data;
        std::string value;

        data = line.substr(0, line.find(':'));
        data.erase(0, data.find_first_not_of(" \t\n\r\f\v"));
        data.erase(data.find_last_not_of(" \t\n\r\f\v") + 1);
        value = line.substr(line.find(':') + 1);
        value.erase(0, value.find_first_not_of(" \t\n\r\f\v"));
        value.erase(value.find_last_not_of(" \t\n\r\f\v") + 1);
        listStud[data] = std::atof(value.c_str());
    }
    file1.close();
    return listStud;
}

void Bot::comdlevel(std::vector<std::string> &words, int fd)
{
    std::string file = "bot.txt";
    // this->setStudent_13(first_read(file));
    std::map<std::string, float> listStud = this->getStudent_13();

    if (words.size() < 2) {
        std::string botMsg = "Bot: Insufficient arguments.\r\n";
        send(fd, botMsg.c_str(), botMsg.length(), 0);
        return;
    }

    std::map<std::string, float>::iterator it = listStud.find(words[1]);
    if (it != listStud.end())
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << it->second;
        std::string botMsg = "Bot: Level Of The " + words[1] + " is " + ss.str() + "%\r\n";
        std::string botMsg2 = "Bot: " + words[1] + " is a student of 1337 (Promo 5).\r\n";
        send(fd, botMsg.c_str(), botMsg.length(), 0);
        send(fd, botMsg2.c_str(), botMsg2.length(), 0);
    }
    else
    {
        std::string botMsg = "Bot: User " + words[1] + " is not found\r\n";
        send(fd, botMsg.c_str(), botMsg.length(), 0);
    }
}


// void        Bot::comhelp(std::vector<std::string> &words, int fd){
//     sprint()
// // }
// void        Bot::comdBot(std::vector<std::string> &words, int fd){
    
//     // if (!words[1].compaire("help") )
//     //     this->comdhelp(words, fd)
//     if(!words[1].compare("level"))
//         this->comdBot(words, fd);
// }