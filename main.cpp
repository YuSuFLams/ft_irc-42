/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 09:39:17 by araiteb           #+#    #+#             */
/*   Updated: 2024/02/25 03:06:38 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/server.hpp"

int main(int ac, char **av)
{
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
	return 0;
}