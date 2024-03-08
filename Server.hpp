/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 04:52:32 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/08 10:26:37 by abel-hid         ###   ########.fr       */
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
# include <fstream>
# include <map>
#include <iomanip>


class Server
{
    private:
        bool flagMode;
        int port;
        int server_fd;
        int new_socket;
        sockaddr_in address;
        std::map<std::string, float> Student_13;
        std::map<int, Client *> clients;
        std::map<std::string , Channel *> channels;
    public:
        std::map<std::string, float> getStudent_13() { return (this->Student_13);}
        void comdBot(Server server, std::vector<std::string> words, int fd);
        void setStudent_13(std::map<std::string, float> Student_13) { this->Student_13 = Student_13;}
        
        void setFlagMode(bool flag) { flagMode = flag; }
        bool getFlagMode() { return flagMode; }
        
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

        //-------------------//
        std::string get_hostnames();
        std::map<int, Client *> getClients();
        void addClient(int fd, Client *client);
        void removeClient(int fd);
        void setClients(std::map<int, Client *> clients);
        void set_nickname(int fd, std::string nickname);
        void set_username(int fd, std::string username);
        void set_realname(int fd, std::string realname);
        void set_hostname(int fd, std::string hostname);
        void set_servername(int fd, std::string servername);
        std::string get_password(int fd);
        std::string get_nickname(int fd);
        bool getInviteToChannel(int fd);
        std::string get_username(int fd);
        std::string get_realname(int fd);
        std::string get_hostname(int fd);
        void set_fd(int fd, int new_fd);
        void set_password(int fd, std::string password);
        void set_client(int fd, Client *client);
        void set_clients(std::map<int, Client *> clients);
        std::string get_servername(int fd);
        std::string get_current_time();
        int is_nickname_exist(std::string nickname);
        void set_is_registered(int fd, int is_registered);
        int is_registered(int fd);
        std::map<std::string, Channel *> &getChannels();
        void addChannel(std::string name, Channel *channel);
        void set_creator(std::string channel, bool creater);
        std::string get_creator_name(std::string channel);
        int is_nickname_exist_and_registered(std::string nickname);
        void set_topic(std::string channel, std::string topic);
        std::string get_topic(std::string channel);
        int get_fd_users(const std::string& nickname) const;
        void priny_users(std::string channel);
        std::string to_string(int number);
        void remove_client_from_channels(int fd);
        std::string get_users(std::string channel);
        long get_limit(std::string channel);
        //-------------------//
        int	JoinChannel(std::vector<std::string> strs , std::string nickname, int fd, Server &server , std::string str);
        int public_channel(std::string channel_name , std::string key , int fd, Server &server);
        int	PartChannel(std::vector<std::string> strs ,std::map<std::string, Channel *> &channels,  int fd, std::string nickname,  Server &server);
        void handleChannels(std::vector<std::pair<std::string, std::string> >& pairs, int fd, const std::string& nickname , Server &server);
        int	TopicChannel(std::vector<std::string> strs ,std::map<std::string, Channel *> &channels,  int fd,  Server &server);
        void KickChannel(std::vector<std::string> strs, std::map<std::string, Channel *> &channels, int fd, std::string nickname, Server &server , std::string str);
        // invite
        bool isClientExist(std::string nickname);
        bool isValidChannelName(std::string name);
        bool isChannelExist(std::string channelname);
        void invitecmd(std::vector<std::string> words, Server server, int fd, std::string str);
        bool isClientInChannel(std::string nickname, std::string channelname, std::map<std::string, Channel *> &channel);
        bool isClientOperatorInChannel(std::string clientname, std::string channelname, std::map<std::string, Channel *> &channel);
        Client*   getClientByNickname(std::string nick, std::map <int, Client *> clients);
        bool isSenderInChannel(std::string nickname, std::string channelname, std::map<std::string, Channel *> &channel);

        // mode
        void modecmd(std::vector<std::string> words, Server server, int fd);
        void addMode_I(Server server, std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        void addMode_O(int fd, std::vector<std::string> words, Server server, std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        void addMode_T(Server server, std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        void addMode_L(int fd, std::vector<std::string> words, Server server, std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        void addMode_K(int fd, std::vector<std::string> words, Server server, std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        bool isAllDigit(std::string str);
};


#endif