/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 03:00:11 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/17 23:28:17 by abel-hid         ###   ########.fr       */
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

void Server::topic_command(std::vector<std::string > words  , int fd , std::string str)
{

    if(words.size() == 1)
    {
        std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " " + words[0] + " :Not enough parameters\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return ;
    }
    else
    {
        if(this->TopicChannel(words , this->getChannels(), fd , str) == -1)
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
    for (int i = 0 ; words[0][i] ; i++)
		words[0][i] = toupper(words[0][i]);
    if(words[0] == "JOIN")
        join_command(words, fd , buffer);
    else if(words[0] == "PRIVMSG")
        privmsg_command(words, fd, buffer);
    else if(words[0] == "TOPIC")
        topic_command(words, fd , buffer);
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
    std::map <int, Client *>	clients = this->getClients();
    for (std::map<int, Client *>::iterator it = clients.begin(); it!= clients.end(); it++)
    {
        int fd;
        if (it->second->getNick() == "Bot")
            fd = it->second->getFd();
        if (it->second->getNick() == words[1] && words[2] == "time")
        {
            std::string botMsg = "🤖: Hello `" + words[1] + "' - 🕒 Current time is: " + words[2] + ":" + words[3] + " 😄.";
            send_to_user(it->second->getNick(), botMsg, fd);
            return ;
        }
        else if (it->second->getNick() == words[3] && words[1] == "level" && words[2] == "mush")
        {
            std::string botMsg = ": 🤖: Too many name for searching in the list 🤔.";
            send_to_user(it->second->getNick(), botMsg, fd);
            return ;
        }
        else if (it->second->getNick() == words[4] && words[1] == "level" && words[2] == "NOT" && words[3] == "Mush")
        {
            std::string botMsg = "🤖: Please enter a name for searching 🤷.";
            send_to_user(it->second->getNick(), botMsg, fd);
            return ;
        }
        if (it->second->getNick() == words[5] && words[1] == "level" && words[2] == "yes")
        {
            std::string botMsg = "🤖: Level Of `" + words[4] + "' is " + words[3] + "%, " + words[4] + " is a student of 1337-Khouribga 😄.";
            send_to_user(it->second->getNick(), botMsg, fd);
            return ;
        }
        else if (it->second->getNick() == words[4] && words[1] == "level"  && words[2] == "no")
        {
            std::string botMsg = "🤖: User `" + words[3] + "' is not found in the list 😞.";
            send_to_user(it->second->getNick(), botMsg, fd);
            return ;
        }
        else if (it->second->getNick() == words[1] && words[2] == ":help")
        {
            send_to_user(it->second->getNick(), "* Available commands: \n-Command: PASS / Parameters: <password> \n", fd);
            usleep(2);
            send_to_user(it->second->getNick(), "-Command: PASS / Parameters: <password> \n", fd);
            usleep(2);
            send_to_user(it->second->getNick(), "-Command: NICK / Parameters: <nickname> [ <hopcount> ] \n", fd);
            usleep(2);
            send_to_user(it->second->getNick(), "-Command: USER / Parameters: <username> <hostname> <servername> <realname> \n", fd);
            usleep(2);
            send_to_user(it->second->getNick(), "-Command: JOIN / Parameters: <channel>{,<channel>} [<key>{,<key>}] \n", fd);
            usleep(2);
            send_to_user(it->second->getNick(), "-Command: TOPIC / Parameters: <channel> [<topic>] \n", fd);
            usleep(2);
            send_to_user(it->second->getNick(), "-Command: INVITE / Parameters: <nickname> <channel> \n", fd);
            usleep(2);
            send_to_user(it->second->getNick(), "-Command: KICK  / Parameters: <channel> <user> [<comment>] \n", fd);
            usleep(2);
            send_to_user(it->second->getNick(), "-Command: PRIVMSG / Parameters: <receiver>{,<receiver>} <text to be sent> \n", fd);
            usleep(2);
        }
        else if (it->second->getNick() == words[1] || (it->second->getNick() == words[2] && words[1] == "level"))
        {
            std::string botMsg = "🤖: I'm sorry, I don't understand what you mean 😞.";
            send_to_user(it->second->getNick(), botMsg, fd);
        }
    }
    iss.clear();
    words.clear();
}

void Server::nickCmd1(std::string msg, Client *c)
{
    std::string buffer = msg;
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
    for (int i = 0 ; words[0][i] ; i++)
		words[0][i] = toupper(words[0][i]);
    if (!words[0].compare("NICK") && words.size() < 2)
    {
        std::string nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_ERRONEUSNICKNAME) + " NICK :Erroneous nickname\r\n";
        send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
        return ;
    }
    else if (!words[0].compare("NICK") && words.size() == 2 && words[1] == ":")
    {
        std::string nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_ERRONEUSNICKNAME) + " NICK :Erroneous nickname\r\n";
        send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
        return ;
    }
    else if (!words[0].compare("NICK") && words.size() > 1 && c->getNick() != words[1])
    {
        if(!words[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_[]\\`^{}"))
        {
            std::string nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_ERRONEUSNICKNAME) + " NICK :Erroneous nickname\r\n";
            send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
            return ;
        }
        std::string nickMsg;
        Client *tmpClient = this->getClientByNickname(words[1]);
        if (tmpClient)
            nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NICKNAMEINUSE) + " " + c->getNick() + " " + words[1] + " :Nickname is already in use\r\n";
        else
        {
            nickMsg = ":" + c->getNick() + "!" + c->getusername() + "@" + c->gethostname() + " NICK " + words[1] + "\r\n";
            c->seTNick(words[1]);
        }
        send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
        return ;
    }
    else if (words.size() > 2)
    {
        std::string nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " NICK :Not enough parameters\r\n";
        send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
        return ;
    }
}

void	Server::commands(Message &msg, std::vector <std::string> &words, std::string str)
{
    Client *c ;
    c = getClientByFd(msg.getSenderFd());
    if (!c)
        return ;
    for (int i = 0 ; words[0][i] ; i++)
		words[0][i] = toupper(words[0][i]);

    try
    {
        if (!this->IsAuthorized(*c))
        {
            if (!words[0].compare("PASS"))
                cmdpass(words, c ,str);
            else if (!words[0].compare("NICK") && !c->geTPass().empty())
                cmdknick(words, c );
            else if (!words[0].compare("USER") && !c->geTPass().empty())
                cmduser(c, words , str);
            else 
            {
                std::string nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NOTREGISTERED) + " " + c->getNick() + " :You have not registered\r\n";
                send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
                return ;
            }
        }
        else if (this->IsAuthorized(*c))
        {
            if (!words[0].compare("QUIT"))
            {
                this->quit_command(c->getFd());
                return ;
            }
            else if (!words[0].compare("PASS") || !words[0].compare("USER"))
            {
                std::string nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_ALREADYREGISTRED) + " " + c->getNick() + " :You may not reregister\r\n";
                send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
                return ;
            }
            else if (!words[0].compare("NICK"))
            {
                nickCmd1(str, c);
                return ;
            }
            else if (!words[0].compare("PONG"))
            {
                return ;
            }
            else if (!words[0].compare("🤖"))
            {
                comdBotBot(str);
            }
            else if(!words[0].compare("JOIN") || !words[0].compare("TOPIC") || !words[0].compare("PART") 
                || !words[0].compare("KICK") || !words[0].compare("PRIVMSG"))
            {
                join_topic_part_kick_privmsg(c->getFd(), str);
            }
            else if (!words[0].compare("INVITE"))
                this->invitecmd(str, c->getFd());
            else if (!words[0].compare("MODE"))
                this->modecmd(str, c->getFd());
        }
    }
    catch (Myexception & e) {}
}


void	Server::cmduser(Client *c, std::vector<std::string> &words , std::string str)
{
    
	if (words.size() < 5)
	{
        std::string userMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " USER :Not enough parameters\r\n";
        send(c->getFd(), userMsg.c_str(), userMsg.length(), 0);
        return ;
    }
    else if (words.size() > 5 && words[4].at(0) != ':')
    {
        std::string userMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " USER :Not enough parameters\r\n";
        send(c->getFd(), userMsg.c_str(), userMsg.length(), 0);
        return ;
    }
	c->setusename(words[1]);
    c->sethostname(words[2]);
    c->setservername(words[3]);
    std::string realname;
    if(words.size() > 5 && words[4].at(0) == ':')
    {
        str = str.erase(0, str.find(words[0]) + words[0].length() + 1);
        str = str.erase(0, str.find(words[1]) + words[1].length() + 1);
        str = str.erase(0, str.find(words[2]) + words[2].length() + 1);
        str = str.erase(0, str.find(words[3]) + words[3].length() + 1);
        realname = str.erase(0,1);
    }
    else
        realname = words[4];
    c->setrealname(realname);
    Client *tmpClient = this->getClientByNickname(c->getNick());
    if (tmpClient && this->IsAuthorized(*tmpClient))
    {
        std::string nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NICKNAMEINUSE) + " " + c->getNick() + " " + words[0] + " :Nickname is already in use\r\n";
        send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
        c->seTNick("");
        return ;
    }
    else
	if (this->IsAuthorized(*c) && c->geTPass() == this->m_pass)
    {
        sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 001 " + this->get_nickname(c->getFd()) + " :Welcome to the Internet Relay Network " + this->get_nickname(c->getFd())+ "!" + this->get_username(c->getFd()) + "@" + this->get_hostnames() + "\r\n");
        sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 002 " + this->get_nickname(c->getFd()) + " :Your host is " + this->get_hostnames() + ", running version 1.0\r\n");
        sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 003 " + this->get_nickname(c->getFd()) + " :This server was created " +  this->get_current_time() + "\r\n");
    }
    else if(c->geTPass() != this->m_pass && this->IsAuthorized(*c))
    {
        std::string passMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_PASSWDMISMATCH) + " " + "PASS" + " :Password incorrect\r\n";
        send(c->getFd(), passMsg.c_str(), passMsg.length(), 0);
        c->seTPass("");
        return ;
    }
}
void	Server::cmdknick(std::vector<std::string> &words, Client *c)
{
    Client *tmpClient;
    if (words.size() != 2 ||  words[1].empty())
    {
        std::string nickMsg;
        if (words.size() < 2)
            nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NONICKNAMEGIVEN) + " " + words[0] +  " :No nickname given\r\n";
        else if (words.size() > 2)
            nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_ERRONEUSNICKNAME) + " " + words[0] + " :Erroneous nickname\r\n";
        send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
        return ;
    }
    std::size_t found = words[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_[]\\`^{}");
    if (found != std::string::npos)
    {
        std::string nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_ERRONEUSNICKNAME) + " " + words[0] + " :Erroneous nickname\r\n";
        send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
        return ;
    }
    if (!words[1].empty() && words[2].empty())
    {
        tmpClient = this->getClientByNickname(words[1]);
        if ((tmpClient && tmpClient->getFd() != c->getFd() && this->IsAuthorized(*tmpClient)) || (tmpClient && !words[1].compare("Bot")))
        {
            std::string nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NICKNAMEINUSE) + " " + c->getNick() + " " + words[0] + " :Nickname is already in use\r\n";
            send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
            return ;
        }
        
        c->seTNick(words[1]);
        if (this->IsAuthorized(*c) && c->geTPass() == this->m_pass)
        {
            sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 001 " + this->get_nickname(c->getFd()) + " :Welcome to the Internet Relay Network " + this->get_nickname(c->getFd())+ "!" + this->get_username(c->getFd()) + "@" + this->get_hostnames() + "\r\n");
            sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 002 " + this->get_nickname(c->getFd()) + " :Your host is " + this->get_hostnames() + ", running version 1.0\r\n");
            sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 003 " + this->get_nickname(c->getFd()) + " :This server was created " +  this->get_current_time() + "\r\n");
        }
        else if(c->geTPass() != this->m_pass && this->IsAuthorized(*c))
        {
            std::string passMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_PASSWDMISMATCH) + " " + c->getNick() + " :Password incorrect\r\n";
            send(c->getFd(), passMsg.c_str(), passMsg.length(), 0);
            c->seTPass("");
            return ;
        }
    }
}

void	Server::cmdpass(std::vector<std::string>& words, Client *c , std::string str)
{
	if (words.size() < 2 || (words.size() > 2 && words[1].at(0) != ':'))
	{
        std::string passMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " PASS :Not enough parameters\r\n";
        send(c->getFd(), passMsg.c_str(), passMsg.length(), 0);
        return ;
    }
	else 
    {
        std::string pass;
        if (c->getNick() == "Bot" && words[2].compare(this->m_pass))
        {
            std::string passMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_PASSWDMISMATCH) + " PASS :Password incorrect\r\n";
            send(c->getFd(), passMsg.c_str(), passMsg.length(), 0);
            return ;
        }
        if(words.size() > 2 && words[1].at(0) == ':')
        {
            pass = str.substr(str.find(":") + 1 , str.length());
        }
        else
            pass = words[1];
		c->seTPass(pass);
        if (this->IsAuthorized(*c) && c->geTPass() == this->m_pass)
        {
            sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 001 " + this->get_nickname(c->getFd()) + " :Welcome to the Internet Relay Network " + this->get_nickname(c->getFd())+ "!" + this->get_username(c->getFd()) + "@" + this->get_hostnames() + "\r\n");
            sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 002 " + this->get_nickname(c->getFd()) + " :Your host is " + this->get_hostnames() + ", running version 1.0\r\n");
            sendResponce(c->getFd(), ":" + this->get_hostnames() +  " 003 " + this->get_nickname(c->getFd()) + " :This server was created " +  this->get_current_time() + "\r\n");
        }
        else if(c->geTPass() != this->m_pass && this->IsAuthorized(*c))
        {
            std::string passMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_PASSWDMISMATCH) + " PASS :Password incorrect\r\n";
            send(c->getFd(), passMsg.c_str(), passMsg.length(), 0);
            c->seTPass("");
            return ;
        }
	}
}
