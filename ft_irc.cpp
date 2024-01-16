/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 18:17:40 by ylamsiah          #+#    #+#             */
/*   Updated: 2024/01/16 18:44:30 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_irc.hpp"

int main(int ac, char **str)
{
    if (ac != 3)
    {
        std::cout << "Usage: ./ft_irc <host> <port>" << std::endl;
        return (0);
    }
    std::cout << "Host: " << str[1] << std::endl;
}