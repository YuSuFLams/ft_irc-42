/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 20:11:39 by araiteb           #+#    #+#             */
/*   Updated: 2024/02/25 05:37:11 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "messages.hpp"

Message::Message(){
    
}
Message::Message(int fd, std::string msg){
    char *ptr;
    std::string tmp;

    this->SenderFd = fd;
    ptr = strtok((char *)msg.c_str(),"\r\n");
    while (ptr != NULL)
    {
        this->full_msg.push_back(ptr);
        ptr = strtok(NULL,"\r\n");
    }
    this->message = full_msg[0];
}

Message::~Message(){

}
void    Message::setMessage(std::string msg){
    this->message = msg;
}
std::string     Message::getMessage(){
    return this->message;
}

int         Message::getSenderFd() 
{
    return (this->SenderFd);
}

void        Message::setSenderFd(int fd)
{
    this->SenderFd = fd;
}