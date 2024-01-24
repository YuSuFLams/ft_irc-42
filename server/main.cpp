/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 09:39:17 by araiteb           #+#    #+#             */
/*   Updated: 2024/01/22 12:01:35 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

// void      SplitCmd(std::string cmd)
// {
    
// }
int main(int ac, char **av)
{
    std::string cmd;
    std::string token;

    if (ac != 3)
    {
        std::cerr << "prob in arguments" << std::endl;
        return 0;
    }
    Server S(av[2], atoi(av[1]));
    if (!S.CreateSocket())
        exit (1);
    if (!S.OptionSocket())
        exit (1);
    if (!S.NnBlockFd())
        exit (1);
    if (!S.BindSocket())
        exit (1);
    if (!S.listenSocket())
        exit (1);
    S.PollingFd();
    // token = strtok(cmd, " ");
    // while(std::string token != NULL)
    // {
        
    // }
    return 0;
}