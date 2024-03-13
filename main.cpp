/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 09:39:17 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/13 01:09:55 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/Server.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Usage: ./ircserv [port] [password]" << std::endl;
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
	return 0;
}