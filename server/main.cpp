/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 09:39:17 by araiteb           #+#    #+#             */
/*   Updated: 2024/01/17 14:25:23 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int main(int ac, char **av)
{
    std::string mes;
    (void)ac;
    Server S(av[2], atoi(av[1]));
    int value;
    value =  S.createStocket();
    if ( value == 1)
        exit (value);
    while (1)
    {
        int i = recv(S.server_fd, S.buffer, 1024, 0);
        if (i > 0)
            std::cout << S.buffer << std::endl;
    }
    return 0;
}