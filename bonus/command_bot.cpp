/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_bot.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 06:21:49 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/11 03:29:29 by ylamsiah         ###   ########.fr       */
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

