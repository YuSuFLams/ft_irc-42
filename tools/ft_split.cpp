/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:09:12 by araiteb           #+#    #+#             */
/*   Updated: 2024/02/25 03:51:50 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"

void	split(std::string &msg, std::vector<std::string> SplitedMsg)
{
	std::vector<std::string> tmpCommand;
	int 	len = tmpCommand.size();
	splitCommand (msg, ':', tmpCommand);
	splitCommand(tmpCommand[0], ' ', SplitedMsg);
	for (int i = 1; i < len; i++)
		SplitedMsg.push_back(tmpCommand[i]);
	tmpCommand.clear();
	// return 1;
}

void	initTab(std::string strs[MAX])
{
	for (int i = 0; i < MAX; i++)
		strs[i] = "";
}

void    splitCommand(std::string str, char oper, std::vector<std::string> &SplitedMsg)
{
	int ind = 0;
	int i = 0;
	int start = 0;
	int end = 0;
	int len = str.length();

	
	while(i <= len)
	{
		if ((str[i] == oper) || (i == len))
		{
			end = i;
			std::string subStr = "";
			subStr.append(str, start, end - start);
			SplitedMsg.push_back(subStr);
			ind++;
			start = end + 1;
		}
		i++;
	}
}

std::string const   int2string(int n) {
    std::stringstream    ss;

    ss << n;
    return (ss.str());
}