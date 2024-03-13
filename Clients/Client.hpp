/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 15:04:46 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/13 01:06:34 by ylamsiah         ###   ########.fr       */
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
};


#endif