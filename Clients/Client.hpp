/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 15:04:46 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/09 00:49:55 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include<iostream>
#include<string>
#include <vector>
#include "../server/server.hpp"
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
};


#endif