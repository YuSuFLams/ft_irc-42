/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 09:39:17 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/14 23:56:33 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/Server.hpp"

#define Welcome "\033[33;1m\n\
░▒▓████████▓▒░▒▓████████▓▒          ░▒▓█▓▒░▒▓███████▓▒░ ░▒▓██████▓▒░  \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        \n\
░▒▓██████▓▒░    ░▒▓█▓▒░    ▒▓███▓▒  ░▒▓█▓▒░▒▓███████▓▒░░▒▓█▓▒░        \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░  \033[0m\n\n\
\t\033[36;1mMade By: \033[0m\033[31mylamsiah \033[0m\033[32m| \033[0m\033[35mabel-hid \033[0m\033[32m|\033[0m\033[34m araiteb \033[0m \n"

int parssing_port(std::string port)
{
	int i = 0;
	if(std::atoi(port.c_str()) > 65535 || std::atoi(port.c_str()) < 1024)
		return (0);
	while (port[i])
	{
		if (!isdigit(port[i]))
			return (0);
		i++;
	}
	return (1);
}

void handl_signal(int signum)
{
	if (signum == SIGINT)
		std::cout << "\n\033[31;1mServer is shutting down...\033[0m" << std::endl;
		exit(0);
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
	signal(SIGINT, handl_signal);
	std::cout << Welcome << std::endl;
	std::cout<< "\t[\033[32;1mINFO\033[0m] \033[32;1mServer is running on port \033[0m[\033[32;1m" << av[1] << "\033[0m]\n" << std::endl;
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