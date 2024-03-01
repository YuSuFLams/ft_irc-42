/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 04:52:32 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/01 20:50:55 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
#include "Client.hpp"
#include "Channels.hpp"
# include <string>
# include <sstream>
# include <map>



class Server
{
    private:
        int port;
        int server_fd;
        int new_socket;
        sockaddr_in address;
        std::map<int, Client *> clients;
        std::map<std::string , Channel *> channels;
    public:
        Server();
        ~Server();
        int getPort() const;
        int getServer_fd() const;
        int getNew_socket() const;
        void setPort(int port);
        void setServer_fd(int server_fd);
        void setNew_socket(int new_socket);
        sockaddr_in getAddress() const;
        void setAddress(sockaddr_in address);
        std::string get_hostnames()
        {
            char buffer[1024] = {0};
            std::string hostnames;
            gethostname(buffer, 1024);
            hostnames = buffer;
            return (hostnames);
        }
        std::map<int, Client *> getClients()
        {
            return (this->clients);
        }
        void addClient(int fd, Client *client)
        {
            this->clients[fd] = client;
        }
        void removeClient(int fd)
        {
            this->clients.erase(fd);
        }

        void setClients(std::map<int, Client *> clients)
        {
            this->clients = clients;
        }
        void set_nickname(int fd, std::string nickname)
        {
            std::map<int, Client *>::iterator it;
            it = this->clients.find(fd);
            if(it != this->clients.end())
                clients[fd]->setNickname(nickname);
        }
        void set_username(int fd, std::string username)
        {
            std::map<int, Client *>::iterator it;
            it = this->clients.find(fd);
            if(it != this->clients.end())
                clients[fd]->setUsername(username);
        }
        void set_realname(int fd, std::string realname)
        {
            std::map<int, Client *>::iterator it;
            it = this->clients.find(fd);
            if(it != this->clients.end())
                clients[fd]->setRealname(realname);
        }
        void set_hostname(int fd, std::string hostname)
        {
            std::map<int, Client *>::iterator it;
            it = this->clients.find(fd);
            if(it != this->clients.end())
                clients[fd]->setHostname(hostname);
        }
        void set_servername(int fd, std::string servername)
        {
            std::map<int, Client *>::iterator it;
            it = this->clients.find(fd);
            if(it != this->clients.end())
                clients[fd]->setServername(servername);
        }
        std::string get_password(int fd)
        {
            std::map<int, Client *>::iterator it;
            it = this->clients.find(fd);
            if(it != this->clients.end())
                return (clients[fd]->getPassword());
            return ("");
        }
        std::string get_nickname(int fd)
        {
            std::map<int, Client *>::iterator it;
            it = this->clients.find(fd);
            if(it != this->clients.end())
                return (clients[fd]->getNickname());
            return ("");
        }
        
        bool getInviteToChannel(int fd)
        {
            std::map<int, Client *>::iterator it;
            it = this->clients.find(fd);
            if(it != this->clients.end())
                return (clients[fd]->getInviteToChannel());
            return (false);
        }

        std::string get_username(int fd)
        {
            std::map<int, Client *>::iterator it;
            it = this->clients.find(fd);
            if(it != this->clients.end())
                return (clients[fd]->getUsername());
            return ("");
        }
        std::string get_realname(int fd)
        {
            std::map<int, Client *>::iterator it;
            it = this->clients.find(fd);
            if(it != this->clients.end())
                return (clients[fd]->getRealname());
            return ("");
        }
        std::string get_hostname(int fd)
        {
            std::map<int, Client *>::iterator it;
            it = this->clients.find(fd);
            if(it != this->clients.end())
                return (clients[fd]->getHostname());
            return ("");
        } 
        
        void set_fd(int fd, int new_fd)
        {
            std::map<int, Client *>::iterator it;
            it = this->clients.find(fd);
            if(it != this->clients.end())
                clients[fd]->setFd(new_fd);
        }
        void set_password(int fd, std::string password)
        {
            std::map<int, Client *>::iterator it;
            it = this->clients.find(fd);
            if(it != this->clients.end())
                it->second->setPassword(password);
        }
        void set_client(int fd, Client *client)
        {
            std::map<int, Client *>::iterator it;
            it = this->clients.find(fd);
            if(it != this->clients.end())
                clients[fd] = client;
        }
        void set_clients(std::map<int, Client *> clients)
        {
            this->clients = clients;
        }

        std::string get_servername(int fd)
        {
            std::map<int, Client *>::iterator it;
            it = this->clients.find(fd);
            if(it != this->clients.end())
                return (clients[fd]->getServername());
            return ("");
        }

        std::string get_current_time()
        {
            time_t now = time(0);
            struct tm tstruct;
            char buf[80];
            tstruct = *localtime(&now);
            strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
            return (buf);
        }

        int is_nickname_exist(std::string nickname)
        {
            std::map<int, Client *>::iterator it;
            for (it = this->clients.begin(); it != this->clients.end(); it++)
            {
                if (it->second->getNickname() == nickname)
                    return (1);
            }
            return (0);
        }
        
        void set_is_registered(int fd, int is_registered)
        {
            std::map<int, Client *>::iterator it;
            it = this->clients.find(fd);
            if(it != this->clients.end())
                it->second->set_is_Registered(is_registered);
        }
        int is_registered(int fd)
        {
            std::map<int, Client *>::iterator it;
            it = this->clients.find(fd);
            if(it != this->clients.end())
                return (it->second->is_Registered());
            return (0);
        }
        std::map<std::string, Channel *> &getChannels()
        {
            return (this->channels);
        }
        void addChannel(std::string name, Channel *channel)
        {
            this->channels[name] = channel;
        }
        void set_creator(std::string channel, bool creater)
        {
            std::map<std::string, Channel *>::iterator it;
            it = this->channels.find(channel);
            if(it != this->channels.end())
                channels[channel]->set_creater(creater);
        }
       
        std::string get_creator_name(std::string channel)
        {
            std::map<std::string, Channel *>::iterator it;
            it = this->channels.find(channel);
            if(it != this->channels.end())
                return (channels[channel]->get_creator_name());
            return ("");
        }
        int is_nickname_exist_and_registered(std::string nickname)
        {
            std::map<int, Client *>::iterator it;
            for (it = this->clients.begin(); it != this->clients.end(); it++)
            {
                if (it->second->getNickname() == nickname && it->second->is_Registered())
                    return (1);
            }
            return (0);
        }

        void set_topic(std::string channel, std::string topic)
        {
            std::map<std::string, Channel *>::iterator it;
            it = this->channels.find(channel);
            if(it != this->channels.end())
                channels[channel]->set_topic(topic);
        }

        std::string get_topic(std::string channel)
        {
            std::map<std::string, Channel *>::iterator it;
            it = this->channels.find(channel);
            if(it != this->channels.end())
                return (channels[channel]->get_topic());
            return ("No topic is set");
        }
        
        int	JoinChannel(std::vector<std::string> strs , std::string nickname, int fd, Server &server , std::string str);
        int public_channel(std::string channel_name , std::string key , int fd, Server &server);
        int	PartChannel(std::vector<std::string> strs ,std::map<std::string, Channel *> &channels,  int fd, std::string nickname,  Server &server);
        void handleChannels(std::vector<std::pair<std::string, std::string> >& pairs, int fd, const std::string& nickname , Server &server);
        int	TopicChannel(std::vector<std::string> strs ,std::map<std::string, Channel *> &channels,  int fd,  Server &server);
        void KickChannel(std::vector<std::string> strs, std::map<std::string, Channel *> &channels, int fd, std::string nickname, Server &server , std::string str);
        
        int get_fd_users(const std::string& nickname) const 
        {
            std::map<int, Client *>::const_iterator it;
            for (it = this->clients.begin(); it != this->clients.end(); it++)
            {
                if (it->second->getNickname() == nickname)
                    return (it->first);
            }
            return (-1);
        }

        void priny_users(std::string channel)
        {
            std::map<std::string, Channel *>::iterator it;
            it = this->channels.find(channel);
            if(it != this->channels.end())
                channels[channel]->print_users();
        }

        void send_reply(int fd, const std::string& server_name, int reply_code, const std::string& nickname, const std::string& channel, const std::string& message)
        {
            std::string reply = ":" + server_name + " " + std::to_string(reply_code) + " " + nickname + " " + channel + " :" + message + "\r\n";
            send(fd, reply.c_str(), reply.length(), 0);
        }


        std::string to_string(int number)
        {
            std::stringstream ss;
            ss << number;
            return (ss.str());
        }

        void remove_client_from_channels(int fd)
        {
            std::map<std::string, Channel *>::iterator it;
            for (it = this->channels.begin(); it != this->channels.end(); it++)
            {
                it->second->quit_channel(this->get_nickname(fd));
            }
        }
    
    std::string get_users(std::string channel)
    {
        std::map<std::string, Channel *>::iterator it;
        it = this->channels.find(channel);
        if(it != this->channels.end())
        {
            std::set<std::string>::iterator it = channels[channel]->getUsers().begin();
            std::string users;
            while(it != channels[channel]->getUsers().end())
            {
                users += *it + " ";
                it++;
            }
            return (users);
        }
        return ("");
    }
};





#endif