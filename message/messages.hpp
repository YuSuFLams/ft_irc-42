/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 20:10:54 by araiteb           #+#    #+#             */
/*   Updated: 2024/02/25 05:36:22 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <iostream>
#include <string>
#include <vector>
class Message
{
    private:
        std::vector<std::string> full_msg;
        std::string message;
        int         SenderFd;
    
    public:
        Message();
        Message(int, std::string );
        ~Message();


        std::string getMessage();
        int         getSenderFd();

        void        setMessage(std::string );
        void        setSenderFd(int);
};

#endif