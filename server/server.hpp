/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:35:28 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/17 17:34:35 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

# define MAX 1024

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <cstring>
#include <fcntl.h>
#include <cstddef> 
#include <sstream>
#include "../Clients/Client.hpp"
#include "../message/messages.hpp"
#include "../Exeption/Exception.hpp"
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Channels.hpp"
#include <arpa/inet.h>

#define LIMITCNX 5000
class Client;
class Channel;
class Message;

class Server {
	
	private:
		char *						birthday;
		std::string					name;
		std::string					m_pass;
		char						buffer[MAX];
		int							m_port;
		int							m_socket;
		int							server_fd;
		int							on;
		int							user_num;
		int							num;
		int							end_con;
		sockaddr_in					address;
		struct pollfd				users[LIMITCNX];
		int							len;
		long int					timeout;
		int							end_ser;
		int							compress_array;
		std::string					msgsendResponce;
		std::string					checkPiv;
		std::map <int, Client *>	clients;
		

		/////////////////////////  TOOLS  //////////////////////////
		std::map<std::string , Channel *> channels;
		std::map<std::string, float> Student_13;
		bool flagMode;
		std::string allstring;

	public:
		std::string &get_allstring() { return allstring; }
		void set_allstring(std::string str) { allstring = str; }

		
		
		Server(const std::string ipAdd, int port);
		Server(Server &sr);
		Server& operator=(const Server &sr);
		~Server();
		
		void			seTpass(const std::string);
		void 			seTport(int p);
		void 			seTmsocket(int sock);
		void 			seTServerFd(int server);
		void 			seTmapClient(std::map<int, Client *>);

		std::string			getPass();
		int 				getport();
		int 				getsocket();
		int 				getSeverSocket();
		std::map<int, Client *> getClient();

		int				CreateSocket();
		int				OptionSocket();
		int				NnBlockFd();
		int				BindSocket();
		int				listenSocket();
		int 			acceptingData();
		int 			checkmsg(int i);
		void			PollingFd();
		
		void			commands(Message &, std::vector <std::string>&, std::string str);
		
		void			cmduser(Client *c, std::vector<std::string> &SplitedMsg);
		void			cmdpass(std::vector<std::string>& SplitedMsg, Client *c);
		void			cmdknick(std::vector<std::string> &SplitedMsg, Client *c);
		void			cmdprivmsg(std::vector<std::string>& SplitedMsg, Client *c);

		Client*			getClientByFd(int fdUser);
		Client*			getClientByNickname(std::string nickname);
		int			IsAuthorized(Client &);
		void			TraiteMessage(Message &);

		void 			clientLeft(int fd);
		void 			quitServer();


		void topic_command(std::vector<std::string > words , int fd);
		void quit_command(std::vector<std::string > words , int fd);
		void part_command(std::vector<std::string > words , int fd);
		void join_command(std::vector<std::string > words , int fd , std::string str);
		void kick_command(std::vector<std::string > words , int fd , std::string str);
		/////////////////////////  CHANNELS  //////////////////////////
		void addClient(int fd, Client *client);
        void removeClient(int fd);
        void setClients(std::map<int, Client *> clients);
        void set_realname(int fd, std::string realname);
        void set_hostname(int fd, std::string hostname);
        void set_servername(int fd, std::string servername);
        std::string get_password(int fd);
        bool getInviteToChannel(int fd);
        
        std::string get_realname(int fd);
        void set_fd(int fd, int new_fd);
        void set_password(int fd, std::string password);
        void set_client(int fd, Client *client);
        void set_clients(std::map<int, Client *> clients);
        std::string get_servername(int fd);
        std::string get_current_time();
        int is_nickname_exist(std::string nickname);
        void set_is_registered(int fd, int is_registered);
        int is_registered(int fd);
        void addChannel(std::string name, Channel *channel);
        void set_creator(std::string channel, bool creater);
        std::string get_creator_name(std::string channel);
        int is_nickname_exist_and_registered(std::string nickname);
        void set_topic(std::string channel, std::string topic);
        std::string get_topic(std::string channel);
        void priny_users(std::string channel);
        std::string to_string(int number);
        void remove_client_from_channels(int fd);
        std::string get_users(std::string channel);
        long get_limit(std::string channel);
        std::map<int, Client *> &getClients();
		int get_fd_users(const std::string& nickname) const;
        std::string get_username(int fd);
		std::string get_hostname(int fd);
        void set_username(int fd, std::string username);
		
		/////////////////////////  TOOLS  //////////////////////////
		std::string get_hostnames();
        std::string get_nickname(int fd);
		void set_nickname(int fd, std::string nickname);
        std::map<std::string, Channel *> &getChannels();
		void setFlagMode(bool flag) { flagMode = flag; }
        bool getFlagMode() { return flagMode; }
		// invite
        bool isClientExist(std::string nickname);
        bool isValidChannelName(std::string name);
        bool isChannelExist(std::string channelname);
        void invitecmd(std::vector<std::string> words, int fd);
        bool isClientInChannel(std::string nickname, std::string channelname, std::map<std::string, Channel *> &channel);
        bool isClientOperatorInChannel(std::string clientname, std::string channelname, std::map<std::string, Channel *> &channel);
        Client*   getClientByNickname(std::string nick, std::map <int, Client *> clients);
        bool isSenderInChannel(std::string nickname, std::string channelname, std::map<std::string, Channel *> &channel);
		//-------------------//
        int	JoinChannel(std::vector<std::string> strs , std::string nickname, int fd, std::string str);
        int public_channel(std::string channel_name , std::string key , int fd);
		void join_broadcast_msg(std::map<std::string, Channel*>& channels , std::string msg, std::string channelName);
        int	PartChannel(std::vector<std::string> strs ,std::map<std::string, Channel *> &channels,  int fd, std::string nickname);
        void topic_broadcast_msg(std::map<std::string, Channel*>& channels, const std::string& channelName, const std::string& nickname);
		void handleChannels(std::vector<std::pair<std::string, std::string> >& pairs, int fd, const std::string& nickname);
        int	TopicChannel(std::vector<std::string> strs ,std::map<std::string, Channel *> &channels,  int fd);
        void KickChannel(std::vector<std::string> strs, std::map<std::string, Channel *> &channels, int fd, std::string nickname, std::string str);
		// mode
        void modecmd(std::vector<std::string> words, int fd);
        void addMode_I(std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        void addMode_O(int fd, std::vector<std::string> words, std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        void addMode_T(std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        void addMode_L(int fd, std::vector<std::string> words, std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        void addMode_K(int fd, std::vector<std::string> words, std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        bool isAllDigit(std::string str);
};

void	split(std::string msg, std::vector<std::string> &SplitedMsg);
void    splitCommand(std::string str, char oper, std::vector<std::string> &SplitedMsg);
void	initTab(std::string strs[MAX]);
void 	sendResponce(int fd, const std::string &responce);
std::string const   int2string (int n);
#endif
