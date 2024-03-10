/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 11:56:01 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/10 12:36:25 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"


void Server::send_to_all(std::string str, int fd)
{
   std::map<std::string, Channel*>::iterator it = this->getChannels().begin();
   std::vector<int> fds;
    for (; it != this->getChannels().end(); it++)
    {
        std::set<std::string>::iterator it2 = it->second->getUsers().begin();
        for (; it2 != it->second->getUsers().end(); it2++)
        {
            int user = this->get_fd_users(*it2);
            if (user != fd && std::find(fds.begin(), fds.end(), user) == fds.end())
                send(user, str.c_str(), str.length(), 0);
            fds.push_back(user);
        }
    }
    fds.clear();
}

void Server::quit_command(int fd)
{
    std::string str =  ":" + this->get_nickname(fd) + "!" + this->get_username(fd) + "@" + this->get_hostnames() + " QUIT : QUIT Leaving...\r\n";
    this->send_to_all(str, fd);
    this->remove_client_from_channels(fd);
    this->removeClient(fd);
    delete this->clients[fd];
    this->clients.erase(fd);
    close(fd);
    str.clear();
}