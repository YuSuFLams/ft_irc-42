/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 09:39:17 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/14 10:48:47 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/Server.hpp"

int parssing_port(std::string port)
{
	if(std::atoi(port.c_str()) > 65535 || std::atoi(port.c_str()) < 1024)
		return (0);
	int i = 0;
	while (port[i])
	{
		if (!isdigit(port[i]))
			return (0);
		i++;
	}
	return (1);
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Usage: ./ircserv [port] [password]" << std::endl;
		return 0;
	}

	if (!parssing_port(av[1]))
	{
		std::cout << "Invalid port" << std::endl;
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