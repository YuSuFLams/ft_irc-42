/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:09:12 by araiteb           #+#    #+#             */
/*   Updated: 2024/01/26 15:21:06 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

std::string strs[8];
void    split(std::string str, char oper)
{
    int ind = 0;
    int i = 0;
    int start = 0;
    int end = 0;
    
    while(i <= str.length())
    {
        if (str[i] == oper || i == str.length())
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
