/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 03:00:11 by araiteb           #+#    #+#             */
/*   Updated: 2024/02/25 05:50:36 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"

void	Server::commands(Message &msg, std::vector <std::string> &SplitedMsg)
{
	Client *c ;

	c = getClientByFd(msg.getSenderFd());
	if (!c)
		return ;
	for (int i = 0 ; SplitedMsg[0][i] ; i++)
		SplitedMsg[0][i] = toupper(SplitedMsg[0][i]);
	std::cout << "[" << SplitedMsg[0] << "]" <<  " [" << SplitedMsg[1] << "]" << std::endl;
// 		// All Command Should be Compaired here 

		if (!SplitedMsg[0].compare("PASS"))
			cmdpass(SplitedMsg, *c);
		else if (!SplitedMsg[0].compare("NICK"))
			cmdknick(SplitedMsg, c);
		else if (!SplitedMsg[0].compare("USER"))
			cmduser(c, SplitedMsg);
		// else if (!IsAuthorized(c))
		// 	throw "ERR_NOTREGISTERED";
		else if(!SplitedMsg[0].compare("PRIVMSG"))
			cmdprivmsg(SplitedMsg, c);	
}

void	Server::cmduser(Client *c, std::vector<std::string> &SplitedMsg)
{
	try {
        if (this->IsAuthorized(*c))
            throw ERR_ALREADYREGISTRED;
		if (SplitedMsg[1].empty() || SplitedMsg[2].empty() || SplitedMsg[3].empty() || SplitedMsg[4].empty())
			throw ERR_NEEDMOREPARAMS;
		c->setusename(SplitedMsg[1]);
        c->sethostname(SplitedMsg[2]);
        c->setservername(SplitedMsg[3]);
        c->setrealname(SplitedMsg[4]);
		if (this->IsAuthorized(*c)) {
                sendResponce(c->getFd(), this->name + "001 "
                    + c->getNick() + " :Welcome to the Internet Relay Network "
                    + c->getIdent() + "\n");
                sendResponce(c->getFd(), this->name + "002 "
                    + c->getNick() + " :Your host is "
                    + this->name.substr(1) + "running on version 0.1 \n");
                sendResponce(c->getFd(), this->name + "003 "
                    + c->getNick() + " :This server was created "
                    + this->birthday);
		    }
	}
	catch(Myexception & e) {
        sendResponce(c->getFd(), this->name
            + int2string(e.getERROR_NO()) + " "
            + c->getNick() + " "
            + SplitedMsg[0] + " "
            + e.what() + "\n");
    }
}
void	Server::cmdknick(std::vector<std::string> &SplitedMsg, Client *c)
{
	Client *tmpClient;

	try{
        if (this->IsAuthorized(*c))
            throw ERR_ALREADYREGISTRED;
		if (!SplitedMsg[2].empty())
			throw ERR_ERRONEUSNICKNAME;
		if (SplitedMsg[1].empty())
			throw ERR_NONICKNAMEGIVEN;
		if (!SplitedMsg[1].empty() && SplitedMsg[2].empty()) {
			tmpClient = this->getClientByNickname(SplitedMsg[1]);
			if (tmpClient)
				throw ERR_NICKNAMEINUSE;
				c->seTNick(SplitedMsg[1]);
            if (this->IsAuthorized(*c)) {
                sendResponce(c->getFd(), this->name + "001 "
                    + c->getNick() + " :Welcome to the Internet Relay Network "
                    + c->getIdent() + "\n");
                sendResponce(c->getFd(), this->name + "002 "
                    + c->getNick() + " :Your host is "
                    + this->name.substr(1) + "running on version 0.1 \n");
                sendResponce(c->getFd(), this->name + "003 "
                    + c->getNick() + " :This server was created "
                    + this->birthday);
		    }
        }
    }
    catch(Myexception & e) {
        sendResponce(c->getFd(), this->name
            + int2string(e.getERROR_NO()) + " "
            + c->getNick() + " "
            + SplitedMsg[0] + " "
            + e.what() + "\n");
    }
}

void	Server::cmdpass(std::vector<std::string>& SplitedMsg, Client &c)
{
    std::cout << "PASS COMMAND " << std::endl;
	try{
        if (this->IsAuthorized(c))
            throw ERR_ALREADYREGISTRED;
		if (SplitedMsg[1].empty())
			throw ERR_NEEDMOREPARAMS;
		else {
			if (SplitedMsg[1].compare(this->m_pass))
				throw ERR_ALREADYREGISTRED;
			c.seTPass(SplitedMsg[1]);
			}
		}
   catch(Myexception & e) {
        sendResponce(c.getFd(), this->name
            + int2string(e.getERROR_NO()) + " "
            + c.getNick() + " "
            + SplitedMsg[0] + " "
            + e.what() + "\n");
    }
}


void	Server::cmdprivmsg(std::vector<std::string>& SplitedMsg, Client *c)
{
	try{
		Client *newClient;
		newClient = getClientByNickname(SplitedMsg[1]);
		if (!newClient) {
			sendResponce(c->getFd(), "user not fond \n");
			return ;
		} 
		else if (!SplitedMsg[2].empty()) {
			std::cout<< "[" << SplitedMsg[2] <<"]" << std::endl;
			std::string msg = ":" + c->getNick() + " " + SplitedMsg[0] + " " + SplitedMsg[1] + " :" + SplitedMsg[2];
			sendResponce(newClient->getFd(), msg);
			sendResponce(newClient->getFd(), "\n");
		}
	}
	catch(...) {
		std::cout << "error \n" <<std::endl;
	}
}