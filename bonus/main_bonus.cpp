/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 03:15:46 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/11 18:30:52 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void 	sendResponce(int fd, const std::string &responce)
{
    std::cout << responce << std::endl;
	send(fd, responce.c_str(), responce.length(), 0);
}
std::map<std::string, float> first_read(std::string file)
{
    std::ifstream file1(file);
    if (!file1.is_open())
    {
        std::cerr << "Error: Can't open file" << std::endl;
        exit (1);
    }
    std::string line;
    std::map<std::string, float> listStud;
    while (std::getline(file1, line))
    {
        if (line.empty())
            continue ;
        std::string data;
        std::string value;

        data = line.substr(0, line.find(':'));
        data.erase(0, data.find_first_not_of(" \t\n\r\f\v"));
        data.erase(data.find_last_not_of(" \t\n\r\f\v") + 1);
        value = line.substr(line.find(':') + 1);
        value.erase(0, value.find_first_not_of(" \t\n\r\f\v"));
        value.erase(value.find_last_not_of(" \t\n\r\f\v") + 1);
        listStud[data] = std::atof(value.c_str());
    }
    file1.close();
    return listStud;
}

std::string Bot::comdBot(std::vector<std::string> &words, int fd)
{
    (void)fd;
    std::string file = "bot.txt";
    this->setStudent_13(first_read(file));
    std::map<std::string, float> listStud = this->getStudent_13();

    std::map<std::string, float>::iterator it = listStud.find(words[4]);
    if (it != listStud.end())
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << it->second;
        std::string botMsg = " Level Of " + words[4] + " is " + ss.str() + "%, " + words[4] + " is a student of 1337-Khouribga 😄.";
        // sendResponce(fd, botMsg);
        return botMsg;
    }
    else
    {
        std::string botMsg = "User " + words[4] + " is not found in the list 😞.";
        return botMsg;
        // sendResponce(fd, botMsg);
    }
    
}

std::string removeNewlineFromEnd(const std::string& inputString) {
    std::string result = inputString;

    // Remove newline character from the end of the string
    while (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    return result;
}
    // :/bot PRIVMSG test : Commands Comming Soon
void 	Bot::traitResvedData(std::string &msg, int client_fd) {
    char message[1548];
    std::cout << msg;
    std::string sender;
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
    sender = words[0].substr(1, words[0].find("!") - 1);
    if (!words[3].empty() && !words[3].compare("::time")) {
        std::string mssg = "🤖: Hello `" + sender + "' - 🕒 Current time is: " + this->returntime() + " 😄.";
        sprintf(message, "%s\r\n", mssg.c_str());
        sendResponce(client_fd, message);
    }
    else if (!words[3].empty() && !words[3].compare(":level"))
    {
        std::string mssg = "🤖: " + this->comdBot(words, client_fd);
        sprintf(message, "%s\r\n", mssg.c_str());
        sendResponce(client_fd, message);
    }
    else if (!words[3].empty() && !words[3].compare("::help"))
    {
        sendResponce(client_fd , "* Available commands: \n-Command: PASS / Parameters: <password> \n");
        usleep(1);
        sendResponce(client_fd , "-Command: PASS / Parameters: <password> \n");
        usleep(1);
        sendResponce(client_fd , "-Command: NICK / Parameters: <nickname> [ <hopcount> ] \n");
        usleep(1);
        sendResponce(client_fd , "-Command: USER / Parameters: <username> <hostname> <servername> <realname> \n");
        usleep(1);
        sendResponce(client_fd , "-Command: JOIN / Parameters: <channel>{,<channel>} [<key>{,<key>}] \n");
        usleep(1);
        sendResponce(client_fd , "-Command: TOPIC / Parameters: <channel> [<topic>] \n");
        usleep(1);
        sendResponce(client_fd , "-Command: INVITE / Parameters: <nickname> <channel> \n");
        usleep(1);
        sendResponce(client_fd , "-Command: KICK  / Parameters: <channel> <user> [<comment>] \n");
        usleep(1);
        sendResponce(client_fd , "-Command: PRIVMSG / Parameters: <receiver>{,<receiver>} <text to be sent> \n");
        usleep(1);
        return ;
    }
}


int main(int ac, char **av){
    
    if(ac != 3){
        std::cerr << "prob in arguments" << std::endl;
        return 0;
    }
    struct sockaddr_in serv_addr;
    int port = atoi(av[1]);
    std::string pass = av[2];
    Bot *betbot = new Bot();
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Socket creation failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
 
    if ((connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        std::cout << "Connection Failed  " << std::endl;;
        return -1;
    }
    betbot->ConnetToServer(sockfd, pass);
    delete betbot;
    close(sockfd); 
}