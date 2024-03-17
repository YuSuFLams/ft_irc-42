/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:09:12 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/17 14:18:30 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

void	split(std::string msg, std::vector<std::string> &SplitedMsg)
{
	std::vector<std::string> tmpCommand;
	std::string tmp;

	int len = msg.size();
	int pos2poin = msg.find_first_of (":");
	if (pos2poin == -1)
		pos2poin = len;
	tmp = msg.substr(0, pos2poin);
	if (!tmp.empty())
		tmpCommand.push_back(tmp);
	tmp.clear();
	if ( pos2poin < len)
		tmp= msg.substr(pos2poin + 1);
	if (!tmp.empty())
		tmpCommand.push_back(tmp);
	splitCommand(tmpCommand[0], ' ', SplitedMsg);
	tmpCommand.clear();
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
			if (!subStr.empty())
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
