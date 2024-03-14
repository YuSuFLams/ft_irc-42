/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 15:04:46 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/14 04:56:08 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include<iostream>
#include<string>
#include <vector>
#include "../server/Server.hpp"

class Client
{
	private:
		std::string		nickname;
		std::string		pass;
		std::string		username;
		std::string		hostname;
		std::string		servername;
		std::string		realname;
		int 			fd;
		int 			is_registered;
		bool 			InviteToChannel;
		std::string		ip_address;
		std::vector<std::string> str;
	public:
		Client();
		Client(int fd);
		Client(Client &cl);
		Client& operator =(const Client &cl);
		~Client();
	
		void			seTNick(std::string nick);
		void			seTPass(std::string pwd);
		void			setFdUser(int FdUser);
		void			setusename(std::string);
		void			sethostname(std::string);
		void			setservername(std::string);
		void			setrealname(std::string);
		void			set_is_Registered(int is_registered);
		void 			setInviteToChannel(bool InviteToChannel);


		int				getFd();
		std::string		getNick();
		std::string		geTPass();
		std::string 	getIdent();
		std::string		getusername();
		std::string		gethostname();
		std::string		getrealname();
		std::string		getservername();
		bool			getInviteToChannel();
		
		int 			is_Registered();

		std::string		getipaddress()
		{
			return (this->ip_address);
		}
		void			set_ip_address(std::string ip)
		{
			this->ip_address = ip;
		}
		std::vector<std::string> getStr()
		{
			return (this->str);
		}
		void setStr(std::string str)
		{
			this->str.push_back(str);
		}
		void clearStr()
		{
			this->str.clear();
		}
		
};


#endif