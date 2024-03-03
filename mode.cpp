#include "Server.hpp"
#include "message.hpp"

std::vector<std::string> get_modes(std::string word)
{
    std::string::iterator it = word.begin();
    std::vector<std::string> modes;

    if (*it == '+' || *it == '-')
        it++;
    for (; it != word.end(); it++)
        modes.push_back(std::string(1, *it));
    return modes;
}

bool Server::isValidMode(const std::string& word) {
    if (word.size() < 2)
        return false;

    std::string::const_iterator it = word.begin();
    if (*it != '+' && *it != '-')
        return false;
    else
        it++;

    for (; it != word.end(); it++) {
        if (!std::isalpha(static_cast<unsigned char>(*it)))
            return false;
    }
    return true;
}

void Server::addMode_I(Server server, std::map<std::string, Channel *> &channel, std::string channelname, std::string modeType, bool add)
{
    for (std::map<std::string, Channel *>::iterator it = channel.begin(); it != channel.end(); it++)
    {
        if (it->first == channelname)
        {
            it->second->setInviteOnly(add);
            std::set<std::string> usersInChannel = it->second->getUsers();
            for (std::set<std::string>::iterator it2 = usersInChannel.begin(); it2 != usersInChannel.end(); it2++)
            {
                int fdRe;
                std::string name;
                for (std::map<int , Client *>::iterator it1 = this->clients.begin(); it1 != this->clients.end(); it1++)
                {
                    fdRe = (*it1).first;
                    name = (*it1).second->getNickname();
                    if (name == *it2)
                    {
                        std::string mtype = ((add)? "+": "-") + modeType;
                        std::string mode = RPL_CHANNELMODEIS_222(server.get_hostnames(), server.get_nickname(fdRe), channelname, mtype);
                        send(fdRe, mode.c_str(), mode.length(), 0);
                    }
                }
            }
        }
    }
}

void Server::addMode_T(Server server, std::map<std::string, Channel *> &channel, std::string channelname, std::string modeType, bool add)
{
    for (std::map<std::string, Channel *>::iterator it = channel.begin(); it != channel.end(); it++)
    {
        if (it->first == channelname)
        {
            it->second->setTopicRestriction(add);
            std::set<std::string> usersInChannel = it->second->getUsers();
            for (std::set<std::string>::iterator it2 = usersInChannel.begin(); it2 != usersInChannel.end(); it2++)
            {
                int fdRe;
                std::string name;
                for (std::map<int , Client *>::iterator it1 = this->clients.begin(); it1 != this->clients.end(); it1++)
                {
                    fdRe = (*it1).first;
                    name = (*it1).second->getNickname();
                    if (name == *it2)
                    {
                        std::string mtype = ((add)? "+": "-") + modeType;
                        std::string mode = RPL_CHANNELMODEIS_222(server.get_hostnames(), server.get_nickname(fdRe), channelname, mtype);
                        send(fdRe, mode.c_str(), mode.length(), 0);
                    }
                }
            }
        }
    }
}

void Server::addMode_O(int fd, std::vector<std::string> words, Server server, std::map<std::string, Channel *> &channel, std::string channelname, std::string modeType, bool add)
{
    if (words.size() != 4)
    {
        std::string errorMode = ERR_NEEDMOREPARAMS_111(server.get_hostnames(), server.get_nickname(fd), words[0]);
        send(fd, errorMode.c_str(), errorMode.length(), 0);
        return ;
    }
    if (!server.isClientExist(words[3]))
    {
        std::string errorMode = ERR_NOSUCHNICK_111(server.get_hostnames(), server.get_nickname(fd), words[3]);
        send(fd, errorMode.c_str(), errorMode.length(), 0);
        return ;
    }
    if (!server.isClientInChannel(words[3], channelname, channel))
    {
        std::string errorMode = ERR_USERNOTINCHANNEL_111(server.get_hostnames(), server.get_nickname(fd), words[3], channelname);
        send(fd, errorMode.c_str(), errorMode.length(), 0);
        return ;
    }
    for (std::map<std::string, Channel *>::iterator it = channel.begin(); it != channel.end(); it++)
    {
        if (it->first == channelname)
        {
            std::string nickname = "@" + words[3];
            if (add)
            {
                it->second->addOperator(nickname);
                std::string addop = ":" + server.get_hostnames() + " MODE " + channelname + " +o " + words[3] + "\r\n";
                // send to all users in the channel
                for (std::set<std::string>::iterator it2 = it->second->getUsers().begin(); it2 != it->second->getUsers().end(); it2++)
                {
                    int fdRe;
                    std::string name;
                    for (std::map<int , Client *>::iterator it1 = this->clients.begin(); it1 != this->clients.end(); it1++)
                    {
                        fdRe = (*it1).first;
                        name = (*it1).second->getNickname();
                        if (name == *it2)
                            send(fdRe, addop.c_str(), addop.length(), 0);
                    }
                }
            }
            else
            {
                it->second->removeOperator(nickname);
                std::string removeop = ":" + server.get_hostnames() + " MODE " + channelname + " -o " + words[3] + "\r\n";
                // send to all users in the channel
                for (std::set<std::string>::iterator it2 = it->second->getUsers().begin(); it2 != it->second->getUsers().end(); it2++)
                {
                    int fdRe;
                    std::string name;
                    for (std::map<int , Client *>::iterator it1 = this->clients.begin(); it1 != this->clients.end(); it1++)
                    {
                        fdRe = (*it1).first;
                        name = (*it1).second->getNickname();
                        if (name == *it2)
                            send(fdRe, removeop.c_str(), removeop.length(), 0);
                    }
                }
            }
            std::set<std::string> usersInChannel = it->second->getUsers();
            for (std::set<std::string>::iterator it2 = usersInChannel.begin(); it2 != usersInChannel.end(); it2++)
            {
                int fdRe;
                std::string name;
                for (std::map<int , Client *>::iterator it1 = this->clients.begin(); it1 != this->clients.end(); it1++)
                {
                    fdRe = (*it1).first;
                    name = (*it1).second->getNickname();
                    if (name == *it2)
                    {
                        std::string mtype = ((add)? "+": "-") + modeType;
                    }
                }
            }
        }
    }
}



void Server::addMode_L(int fd, std::vector<std::string> words, Server server, std::map<std::string, Channel *> &channel, std::string channelname, std::string modeType, bool add)
{
    if (add)
    {
        if (words.size() != 4)
        {
            std::string errorMode = ERR_NEEDMOREPARAMS_111(server.get_hostnames(), server.get_nickname(fd), words[0]);
            send(fd, errorMode.c_str(), errorMode.length(), 0);
            return ;
        }
        if (!server.isAllDigit(words[3]))
        {
            std::string errorMode = ":" + server.get_hostnames() + " 461 "  + words[0] + " : Invalid parameter\r\n" ;
            send(fd, errorMode.c_str(), errorMode.length(), 0);
            return ;
        }
    }
    else
    {
        if (words.size() != 3)
        {
            std::string errorMode = ERR_NEEDMOREPARAMS_111(server.get_hostnames(), server.get_nickname(fd), words[0]);
            send(fd, errorMode.c_str(), errorMode.length(), 0);
            return ;
        }
    }
    for (std::map<std::string, Channel *>::iterator it = channel.begin(); it != channel.end(); it++)
    {
        if (it->first == channelname)
        {
            if (add)
                it->second->setLimit(std::atol(words[3].c_str()));
            else
                it->second->removeLimit();
            std::set<std::string> usersInChannel = it->second->getUsers();
            for (std::set<std::string>::iterator it2 = usersInChannel.begin(); it2 != usersInChannel.end(); it2++)
            {
                int fdRe;
                std::string name;
                for (std::map<int , Client *>::iterator it1 = this->clients.begin(); it1 != this->clients.end(); it1++)
                {
                    fdRe = (*it1).first;
                    name = (*it1).second->getNickname();
                    if (name == *it2)
                    {
                        std::string mtype = ((add)? "+": "-") + modeType;
                        std::string mode = RPL_CHANNELMODEIS_222(server.get_hostnames(), server.get_nickname(fdRe), channelname, mtype);
                        send(fdRe, mode.c_str(), mode.length(), 0);
                    }
                }
            }
        }
    }
}

bool Server::isAllDigit(std::string str)
{
    for (std::string::iterator it = str.begin(); it != str.end(); it++)
    {
        if (!std::isdigit(*it))
            return false;
    }
    return true;
}



void Server::addMode_K(int fd, std::vector<std::string> words, Server server, std::map<std::string, Channel *> &channel, std::string channelname, std::string modeType, bool add)
{
    if (add)
    {
        if (words.size() != 4)
        {
            std::string errorMode = ERR_NEEDMOREPARAMS_111(server.get_hostnames(), server.get_nickname(fd), words[0]);
            send(fd, errorMode.c_str(), errorMode.length(), 0);
            return ;
        }
    }
    else
    {
        if (words.size() != 3)
        {
            std::string errorMode = ERR_NEEDMOREPARAMS_111(server.get_hostnames(), server.get_nickname(fd), words[0]);
            send(fd, errorMode.c_str(), errorMode.length(), 0);
            return ;
        }
    }
    for (std::map<std::string, Channel *>::iterator it = channel.begin(); it != channel.end(); it++)
    {
        if (it->first == channelname)
        {
            if (add)
            {
                if (it->second->getChannelKey().empty())
                    it->second->setChannelKey(words[3]);
                else
                {
                    std::string errorMode = ERR_KEYSET_111(server.get_hostnames(), server.get_nickname(fd), channelname);
                    send(fd, errorMode.c_str(), errorMode.length(), 0);
                    return ;
                }
            }
            else
            {
                if (it->second->getChannelKey() == words[3])
                    it->second->removeChannelKey();
                else
                {
                    std::string errorMode = ERR_BADCHANNELKEY_111(server.get_hostnames(), server.get_nickname(fd), channelname);
                    send(fd, errorMode.c_str(), errorMode.length(), 0);
                    return ;
                }
            }
            std::set<std::string> usersInChannel = it->second->getUsers();
            for (std::set<std::string>::iterator it2 = usersInChannel.begin(); it2 != usersInChannel.end(); it2++)
            {
                int fdRe;
                std::string name;
                for (std::map<int , Client *>::iterator it1 = this->clients.begin(); it1 != this->clients.end(); it1++)
                {
                    fdRe = (*it1).first;
                    name = (*it1).second->getNickname();
                    if (name == *it2)
                    {
                        std::string mtype = ((add)? "+": "-") + modeType;
                        std::string mode = RPL_CHANNELMODEIS_222(server.get_hostnames(), server.get_nickname(fdRe), channelname, mtype);
                        send(fdRe, mode.c_str(), mode.length(), 0);
                    }
                }
            }
        }
    }
}






void Server::modecmd(std::vector<std::string> words, Server server, int fd)
{
    //check is enough parameters
    if (words.size() < 2 || words.size() > 4)
    {
        std::string errorMode;
        if (words.size() < 2)
            errorMode = ERR_NEEDMOREPARAMS_111(server.get_hostnames(), server.get_nickname(fd), words[0]);
        else
            errorMode = ":" + server.get_hostnames() + " 461 " + server.get_nickname(fd) + " " + words[0] + " :Too Many parameters\r\n";
        send(fd, errorMode.c_str(), errorMode.length(), 0);
        return;
    }
    else 
    {
        // check if channel exist && valid name of channel
        if (!server.isChannelExist(words[1]) || !server.isValidChannelName(words[1]))
        {
            std::string errorMode = ERR_NOSUCHCHANNEL_111(server.get_hostnames(), server.get_nickname(fd), words[1]);
            send(fd, errorMode.c_str(), errorMode.length(), 0);
            return ;
        }
        // check is sender in channel 
        if (!server.isSenderInChannel(server.get_nickname(fd), words[1], server.getChannels()))
        {
            std::string errorMode = ERR_NOTONCHANNEL_111(server.get_hostnames(), server.get_nickname(fd), words[1]);
            send(fd, errorMode.c_str(), errorMode.length(), 0);
            return ;
        }
        // check if sender is operator in channel
        if (!server.isClientOperatorInChannel(server.get_nickname(fd), words[1], server.getChannels()))
        {
            std::string errorMode = ERR_CHANOPRIVSNEEDED_111(server.get_hostnames(), server.get_nickname(fd), words[1]);
            send(fd, errorMode.c_str(), errorMode.length(), 0);
            return ;
        }
        // check if mode is valid
        if (!server.isValidMode(words[2]))
        {
            std::string errorMode = ERR_NEEDMOREPARAMS_111(server.get_hostnames(), server.get_nickname(fd), words[0]);
            send(fd, errorMode.c_str(), errorMode.length(), 0);
            return ;
        }
        else
        {
            std::vector<std::string> modes = get_modes(words[2]);
            bool add;
            if (words[2][0] == '+')
                add = true;
            else
                add = false;
            for (std::vector<std::string>::iterator it = modes.begin(); it != modes.end(); it++)
            {
                switch ((*it)[0])
                {
                    case 'i':
                    {
                        server.addMode_I(server, server.getChannels(), words[1], *it, add);
                        break;
                    }
                    case 't':
                    {
                        server.addMode_T(server, server.getChannels(), words[1], *it, add);
                        break;
                    }
                    case 'o':
                    {
                        server.addMode_O(fd, words, server, server.getChannels(), words[1], *it, add);
                        break;
                    }
                    case 'l':
                    {
                        server.addMode_L(fd, words, server, server.getChannels(), words[1], *it, add);
                        break;
                    }
                    case 'k':
                    {
                        server.addMode_K(fd, words, server, server.getChannels(), words[1], *it, add);
                        break;
                    }
                    default:
                    {
                        std::string errorMode = ERR_UNKNOWNMODE_222(server.get_hostnames(), server.get_nickname(fd), *it);
                        send(fd, errorMode.c_str(), errorMode.length(), 0);
                        return ;
                    }
                }
            }
        }
    }
}

