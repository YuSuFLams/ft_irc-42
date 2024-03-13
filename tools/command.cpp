/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 03:00:11 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/13 03:48:52 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

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

void Server::part_command(std::vector<std::string > words  , int fd, std::string str)
{
    if(words.size() == 1)
    {
        std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " " + words[0] + " :Not enough parameters\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return ;
    }
    else
    {
        if(this->PartChannel(words , this->getChannels(), fd, this->get_nickname(fd), str) == -1)
        {
            std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NOSUCHCHANNEL) + " " + this->get_nickname(fd) + " " + words[1] + " :No such channel\r\n";
            send(fd, str.c_str(), str.length(), 0);
        }
    }
}

void Server::join_topic_part_kick_privmsg(int fd , std::string str)
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
    if(words[0] == "JOIN")
        join_command(words, fd , buffer);
    else if(words[0] == "PRIVMSG")
        privmsg_command(words, fd, buffer);
    else if(words[0] == "TOPIC")
        topic_command(words, fd);
    else if(words[0] == "PART")
        part_command(words, fd, buffer);
    else if(words[0] == "KICK")
        kick_command(words, fd, buffer);
    
    iss.clear();
    words.clear();
    buffer.clear();
}

void Server::comdBotBot(std::string SplitedMsg)
{
    std::string buffer = SplitedMsg;
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
    std::cout <<  "words.size() = " << SplitedMsg << std::endl;
    std::cout << "==========================" << std::endl;
    for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); it++)
    {
        std::cout << "{" << *it << "}" << std::endl;
    }
    std::cout << "==========================" << std::endl;
    std::map <int, Client *>	clients = this->getClients();
    for (std::map<int, Client *>::iterator it = clients.begin(); it!= clients.end(); it++)
    {
        if (it->second->getNick() == words[1] && words[2] == "time")
        {
            std::string botMsg = "🤖: Hello `" + words[0] + "' - 🕒 Current time is: " + words[2] + ":" + words[3] + " 😄.";
            sendResponce(it->second->getFd(), botMsg + "\r\n");
        }
        else if (it->second->getNick() == words[3] && words[1] == "level" && words[2] == "mush")
        {
            std::string botMsg = ": 🤖: Too many name for searching in the list 🤔.";
            sendResponce(it->second->getFd(), botMsg + "\r\n");
            return ;
        }
        if (it->second->getNick() == words[2] && words[1] == "level")
        {
            std::string botMsg = "🤖: Please enter a name for searching 🤷.";
            sendResponce(it->second->getFd(), botMsg + "\r\n");
            return ;
        }
        if (it->second->getNick() == words[5] && words[1] == "level" && words[2] == "yes")
        {
            std::string botMsg = "🤖: Level Of `" + words[4] + "' is " + words[3] + "%, " + words[4] + " is a student of 1337-Khouribga 😄.";
            sendResponce(it->second->getFd(), botMsg + "\r\n");
            return ;
        }
        else if (it->second->getNick() == words[4] && words[1] == "level"  && words[2] == "no")
        {
            std::string botMsg = "🤖: User `" + words[3] + "' is not found in the list 😞.";
            sendResponce(it->second->getFd(), botMsg + "\r\n");
            return ;
        }
        else if (it->second->getNick() == words[1] && words[2] == ":help")
        {
            sendResponce(it->second->getFd() , "* Available commands: \n-Command: PASS / Parameters: <password> \n");
            usleep(1);
            sendResponce(it->second->getFd() , "-Command: PASS / Parameters: <password> \n");
            usleep(1);
            sendResponce(it->second->getFd() , "-Command: NICK / Parameters: <nickname> [ <hopcount> ] \n");
            usleep(1);
            sendResponce(it->second->getFd() , "-Command: USER / Parameters: <username> <hostname> <servername> <realname> \n");
            usleep(1);
            sendResponce(it->second->getFd() , "-Command: JOIN / Parameters: <channel>{,<channel>} [<key>{,<key>}] \n");
            usleep(1);
            sendResponce(it->second->getFd() , "-Command: TOPIC / Parameters: <channel> [<topic>] \n");
            usleep(1);
            sendResponce(it->second->getFd() , "-Command: INVITE / Parameters: <nickname> <channel> \n");
            usleep(1);
            sendResponce(it->second->getFd() , "-Command: KICK  / Parameters: <channel> <user> [<comment>] \n");
            usleep(1);
            sendResponce(it->second->getFd() , "-Command: PRIVMSG / Parameters: <receiver>{,<receiver>} <text to be sent> \n");
            usleep(1);
        }
        
        else if (it->second->getNick() == words[0])
        {
            std::string botMsg = "🤖: I'm sorry, I don't understand what you mean 😞.";
            sendResponce(it->second->getFd(), botMsg + "\r\n");
        }
    }
    iss.clear();
    words.clear();
}

void	Server::commands(Message &msg, std::vector <std::string> &SplitedMsg, std::string str)
{
    Client *c ;
    c = getClientByFd(msg.getSenderFd());
    if (!c)
        return ;

    for (std::vector<std::string>::iterator it = SplitedMsg.begin(); it != SplitedMsg.end(); it++)
    {
        std::cout << "{-" << *it << "}" << std::endl;
    }
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
            if (!SplitedMsg[0].compare("QUIT"))
            {
                this->quit_command(c->getFd());
                return ;
            }
            else if (!SplitedMsg[0].compare("🤖"))
            {
                comdBotBot(msg.getMessage());
            }
            else if(!SplitedMsg[0].compare("JOIN") || !SplitedMsg[0].compare("TOPIC") || !SplitedMsg[0].compare("PART") 
                || !SplitedMsg[0].compare("KICK") || !SplitedMsg[0].compare("PRIVMSG"))
            {
                join_topic_part_kick_privmsg(c->getFd(), str);
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
    }
    
}


void	Server::cmduser(Client *c, std::vector<std::string> &SplitedMsg)
{
    if (this->IsAuthorized(*c))
        throw Myexception(ERR_ALREADYREGISTRED);
	if (SplitedMsg.size() != 5)
		throw Myexception(ERR_NEEDMOREPARAMS);
	c->setusename(SplitedMsg[1]);
    c->sethostname(SplitedMsg[2]);
    c->setservername(SplitedMsg[3]);
    c->setrealname(SplitedMsg[4]);
    
	if (this->IsAuthorized(*c)) 
    {
        sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 001 " + this->get_nickname(c->getFd()) + " :Welcome to the Internet Relay Network " + this->get_nickname(c->getFd())+ "!" + this->get_username(c->getFd()) + "@" + this->get_hostnames() + "\r\n");
        sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 002 " + this->get_nickname(c->getFd()) + " :Your host is " + this->get_servername(c->getFd()) + ", running version 1.0\r\n");
        sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 003 " + this->get_nickname(c->getFd()) + " :This server was created " +  this->get_current_time() + "\r\n");
    }

	
}
void	Server::cmdknick(std::vector<std::string> &SplitedMsg, Client *c)
{
    Client *tmpClient;


    if (SplitedMsg.size() != 2 ||  SplitedMsg[1].empty())
        throw Myexception(ERR_NONICKNAMEGIVEN);
    std::size_t found = SplitedMsg[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]`|/^{}");
    if (found != std::string::npos)
        throw Myexception(ERR_ERRONEUSNICKNAME);
    if (!SplitedMsg[1].empty() && SplitedMsg[2].empty())
    {
        tmpClient = this->getClientByNickname(SplitedMsg[1]);
        if (tmpClient && tmpClient->getFd() != c->getFd())
            throw Myexception(ERR_NICKNAMEINUSE);
        c->seTNick(SplitedMsg[1]);
        if (this->IsAuthorized(*c))
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
	else 
    {
		if (SplitedMsg[1].compare(this->m_pass))
			throw Myexception(ERR_PASSWDMISMATCH);
		c->seTPass(SplitedMsg[1]);
	}
}
