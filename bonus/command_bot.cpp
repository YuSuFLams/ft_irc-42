/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_bot.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 06:21:49 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/11 00:21:10 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

// void        Bot::comhelp(std::vector<std::string> &words, int fd){
//     sprint()
// // }
// void        Bot::comdBot(std::vector<std::string> &words, int fd){
    
//     // if (!words[1].compaire("help") )
//     //     this->comdhelp(words, fd)
//     if(!words[1].compare("level"))
//         this->comdBot(words, fd);
// }

std::string Bot::returntime(){
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return (buf);
}