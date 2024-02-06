/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:09:12 by araiteb           #+#    #+#             */
/*   Updated: 2024/02/06 11:10:09 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int     checkDoublePoints(std::string str)
{
    int i = 0;

    while(str[i])
    {
        if (str[i] == ':')
            return i;
        i++;
    }
    return i;
}
void    split(std::string str, char oper, std::string strs[MAX])
{
    int ind = 0;
    int i = 0;
    int start = 0;
    int end = 0;
    int len = str.length();
    int SizePns = checkDoublePoints(str);
    std::cout << len << std::endl;
    if (SizePns != len)
        len = SizePns;
    while(i <= len)
    {
        if ((str[i] == oper) || (i == len))
        {
            end = i;
            std::string subStr = "";
            subStr.append(str, start, end - start);
            strs[ind] = subStr;
            ind++;
            start = end + 1;
        }
        i++;
    }
    SizePns = str.length();
    if (SizePns != len && str[i])
    {
        start = str.length();
        len = i + 1;
        std::string subStr = "";
        subStr.append(str, len, start - len);
        for (int i = 0; i < MAX; i++)
        {
            if (strs[i].empty())
                strs[i] = subStr;
        }
    }
    // std::cout << "split string " << std::endl;
}
