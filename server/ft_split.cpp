/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:09:12 by araiteb           #+#    #+#             */
/*   Updated: 2024/01/27 14:43:09 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void    split(std::string str, char oper, std::string strs[MAX])
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
            strs[ind] = subStr;
            ind++;
            start = end + 1;
        }
        i++;
    }
}
