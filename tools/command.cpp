/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 03:00:11 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/17 17:42:30 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"

void Server::kick_command(std::vector<std::string > words  , int fd , std::string str)
{

    if(words.size() == 1)
    {
        std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " " + words[0] + " :Not enough parameters\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return ;
    }
    else
    {
        this->KickChannel(words, this->getChannels(), fd, this->get_nickname(fd), str);
    }
}

void Server::join_command(std::vector<std::string > words  , int fd , std::string str)
{
    if(words.size() == 1)
    {
            std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " " + words[0] + " :Not enough parameters\r\n";
            send(fd, str.c_str(), str.length(), 0);
            return ;
    }
    else
    {
        if(this->JoinChannel(words, this->get_nickname(fd), fd , str) == -1)
        {
            std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NOSUCHCHANNEL) + " " + this->get_nickname(fd) + " " + words[1] + " :No such channel\r\n";
            send(fd, str.c_str(), str.length(), 0);
        }
    }
}

void Server::topic_command(std::vector<std::string > words  , int fd)
{

    if(words.size() == 1)
    {
        std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " " + words[0] + " :Not enough parameters\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return ;
    }
    else
    {
        if(this->TopicChannel(words , this->getChannels(), fd) == -1)
        {
            std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NOSUCHCHANNEL) + " " + this->get_nickname(fd) + " " + words[1] + " :No such channel\r\n";
            send(fd, str.c_str(), str.length(), 0);
        }
    }
}

void Server::part_command(std::vector<std::string > words  , int fd)
{
    if(words.size() == 1)
    {
        std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " " + words[0] + " :Not enough parameters\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return ;
    }
    else
    {
        if(this->PartChannel(words , this->getChannels(), fd, this->get_nickname(fd)) == -1)
        {
            std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NOSUCHCHANNEL) + " " + this->get_nickname(fd) + " " + words[1] + " :No such channel\r\n";
            send(fd, str.c_str(), str.length(), 0);
        }
    }
}

void Server::quit_command(std::vector<std::string > words  , int fd)
{
    if(words[0] == "QUIT" && this->get_password(fd) != "" && this->is_registered(fd) == 1)
    {
        close(fd);
        // remove client from the channels
        this->remove_client_from_channels(fd);
        // remove client from the map
        this->removeClient(fd);
        // free the memory
        delete this->getClients()[fd];
        return ;
    }
}

void	Server::commands(Message &msg, std::vector <std::string> &SplitedMsg, std::string str)
{
    Client *c ;

    c = getClientByFd(msg.getSenderFd());
    if (!c)
        return ;
    for (int i = 0 ; SplitedMsg[0][i] ; i++)
		SplitedMsg[0][i] = toupper(SplitedMsg[0][i]);
    try
    {
        if (!SplitedMsg[0].compare("PASS"))
            cmdpass(SplitedMsg, c);
        else if (!SplitedMsg[0].compare("NICK"))
            cmdknick(SplitedMsg, c);
        else if (!SplitedMsg[0].compare("USER"))
            cmduser(c, SplitedMsg);
        else if (this->IsAuthorized(*c))
        {
            if(!SplitedMsg[0].compare("PRIVMSG"))
                cmdprivmsg(SplitedMsg, c);
            else if (!SplitedMsg[0].compare("QUIT"))
            {
                std::cout << "QUIT" << std::endl;
                close(c->getFd());
                this->remove_client_from_channels(c->getFd());
                this->removeClient(c->getFd());
                delete this->getClients()[c->getFd()];
                return ;
            }
            else if(!SplitedMsg[0].compare("JOIN") || !SplitedMsg[0].compare("TOPIC") || !SplitedMsg[0].compare("PART") || !SplitedMsg[0].compare("KICK"))
            {
                std::string buffer = str;
    
                buffer.erase(std::remove(buffer.begin(), buffer.end(), '\n'), buffer.end());
                buffer.erase(std::remove(buffer.begin(), buffer.end(), '\r'), buffer.end());
                if(buffer.empty())
                    return ;
                std::vector<std::string> words;
                std::string word;
                std::istringstream iss(buffer);
                while (iss >> word)
                {
                    words.push_back(word);
                }
                if(words.size() == 0)
                    return ;
                int fd = c->getFd();
                if(words[0] == "JOIN")
                    join_command(words, fd , buffer);
                else if(words[0] == "TOPIC")
                    topic_command(words, fd);
                else if(words[0] == "PART")
                    part_command(words, fd);
                else if(words[0] == "KICK")
                    kick_command(words, fd, buffer);
            }
            else if (!SplitedMsg[0].compare("INVITE"))
                this->invitecmd(SplitedMsg, c->getFd());
            else if (!SplitedMsg[0].compare("MODE"))
                this->modecmd(SplitedMsg, c->getFd());
            else 
                throw Myexception(ERR_UNKNOWNCOMMAND);
        }
        else
            throw Myexception(ERR_ALREADYREGISTRED);
    }
    catch (Myexception & e) 
    {
        sendResponce(c->getFd(), ":" + this->get_hostnames() + " " + int2string(e.getERROR_NO()) + c->getNick() + " "
        + SplitedMsg[0] + " " + e.what() + "\r\n");
        if (this->IsAuthorized(*c) == 2){
            this->clientLeft(c->getFd());
            this->clientLeft(c->getFd());
        }
    }		
}


void	Server::cmduser(Client *c, std::vector<std::string> &SplitedMsg)
{
      if (this->IsAuthorized(*c) || !c->getusername().empty())
        throw Myexception(ERR_ALREADYREGISTRED);
	if (SplitedMsg.size() != 5)
		throw Myexception(ERR_NEEDMOREPARAMS);
    std::size_t found = SplitedMsg[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]`|/^{}");
    if (found != std::string::npos)
        throw Myexception(ERR_ERRONEUSNICKNAME);
	c->setusename(SplitedMsg[1]);
    c->sethostname(SplitedMsg[2]);
    c->setservername(SplitedMsg[3]);
    c->setrealname(SplitedMsg[4]);
    if (this->IsAuthorized(*c) == 2){
        throw  Myexception(ERR_PASSWDMISMATCH);
        this->clientLeft(c->getFd());
    }
	if (this->IsAuthorized(*c) == 1) 
    {
        sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 001 " + this->get_nickname(c->getFd()) + " :Welcome to the Internet Relay Network " + this->get_nickname(c->getFd())+ "!" + this->get_username(c->getFd()) + "@" + this->get_hostnames() + "\r\n");
        sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 002 " + this->get_nickname(c->getFd()) + " :Your host is " + this->get_servername(c->getFd()) + ", running version 1.0\r\n");
        sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 003 " + this->get_nickname(c->getFd()) + " :This server was created " +  this->get_current_time() + "\r\n");
    }
}
void	Server::cmdknick(std::vector<std::string> &SplitedMsg, Client *c)
{
   Client *tmpClient;
    int flag = 0;

    if (!c->getNick().empty())
        flag = 1;
    if (SplitedMsg.size() != 2 ||  SplitedMsg[1].empty())
        throw Myexception(ERR_NONICKNAMEGIVEN);
    std::size_t found = SplitedMsg[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]`|/^{}");
    if (found != std::string::npos)
        throw Myexception(ERR_ERRONEUSNICKNAME);
    if (!SplitedMsg[1].empty() && SplitedMsg[2].empty())
    {
        tmpClient = this->getClientByNickname(SplitedMsg[1]);
        if (tmpClient && (tmpClient->getFd() != c->getFd() || !SplitedMsg[1].compare("Bot")))
            throw Myexception(ERR_NICKNAMEINUSE);
        c->seTNick(SplitedMsg[1]);
        if (this->IsAuthorized(*c) == 2)
            throw  Myexception(ERR_PASSWDMISMATCH);
        if (this->IsAuthorized(*c) == 1 && !flag)
        {
            sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 001 " + this->get_nickname(c->getFd()) + " :Welcome to the Internet Relay Network " + this->get_nickname(c->getFd())+ "!" + this->get_username(c->getFd()) + "@" + this->get_hostnames() + "\r\n");
            sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 002 " + this->get_nickname(c->getFd()) + " :Your host is " + this->get_servername(c->getFd()) + ", running version 1.0\r\n");
            sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 003 " + this->get_nickname(c->getFd()) + " :This server was created " +  this->get_current_time() + "\r\n");
        }
    }
}

void	Server::cmdpass(std::vector<std::string>& SplitedMsg, Client *c)
{
    if (this->IsAuthorized(*c))
        throw Myexception(ERR_ALREADYREGISTRED);
	if (SplitedMsg.size() != 2)
		throw Myexception(ERR_NEEDMOREPARAMS);
	c->seTPass(SplitedMsg[1]);
}


void	Server::cmdprivmsg(std::vector<std::string>& SplitedMsg, Client *c)
{
	Client *newClient;
    
	newClient = getClientByNickname(SplitedMsg[1]);
	if (!newClient)
		throw Myexception(ERR_NOSUCHNICK);
    else if (SplitedMsg.size() < 3)
        throw Myexception(ERR_NEEDMOREPARAMS);
    else if (SplitedMsg.size() == 1)
        throw Myexception(ERR_NORECIPIENT);
	else if (!SplitedMsg[2].empty()) {
        if (!c->getNick().compare("/Bot")){
            if (!SplitedMsg[3].empty() && !SplitedMsg[4].empty()){
                std::string msg = ":" + c->getNick() + " " + SplitedMsg[0] + " " + SplitedMsg[1] + " :" + SplitedMsg[2] +":" +SplitedMsg[3] + ":" + SplitedMsg[4];
                sendResponce(newClient->getFd(), msg);
            }
            else{
                std::string msg = ":" + c->getNick() + " " + SplitedMsg[0] + " " + SplitedMsg[1] + " :" + SplitedMsg[2];
		        sendResponce(newClient->getFd(), msg);
            }
        }
        else{
		    std::string msg = ":" + c->getNick() + " " + SplitedMsg[0] + " " + SplitedMsg[1] + " :" + SplitedMsg[2];
		    sendResponce(newClient->getFd(), msg);
            
        }
        sendResponce(newClient->getFd(), "\n");
	}
}