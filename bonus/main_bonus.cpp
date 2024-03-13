/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 03:15:46 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/13 03:13:15 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void 	sendResponce(int fd, const std::string &responce)
{
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

std::string Bot::comdBot(std::vector<std::string> &words)
{
    std::string file = "bot.txt";
    this->setStudent_13(first_read(file));
    std::map<std::string, float> listStud = this->getStudent_13();

    std::map<std::string, float>::iterator it = listStud.find(words[4]);
    if (it != listStud.end())
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << it->second;
        std::string botMsg = "yes " + ss.str() + " " + words[4] + " ";
        return botMsg;
    }
    else
    {
        std::string botMsg = "no "+ words[4] + " ";
        return botMsg;
    }
    
}

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
    if (!words[3].empty() && !words[3].compare(":level") && words.size() > 5)
    {
        std::string mssg = "🤖:  level  mush " + sender;
        sprintf(message, "%s\r\n", mssg.c_str());
        sendResponce(client_fd, message);
    }
    else if (!words[3].empty() && !words[3].compare(":time")) {
        std::string mssg = "🤖: " + sender + " " + "time "+ " " + this->returntime();
        sprintf(message, "%s\r\n", mssg.c_str());
        sendResponce(client_fd, message);
    }
    else if (!words[3].empty() && !words[3].compare("::level") && words[4].empty())
    {
        std::string mssg = "🤖:  level " + sender;
        sprintf(message, "%s\r\n", mssg.c_str());
        sendResponce(client_fd, message);
    }
    else if (!words[3].empty() && !words[3].compare(":level"))
    {
        std::string mssg = "🤖:  level " + this->comdBot(words) + sender.c_str();
        sprintf(message, "%s\r\n", mssg.c_str());
        sendResponce(client_fd, message);
    }
    else if (!words[3].empty() && !words[3].compare("::help"))
    {
        std::string mssg = "🤖: " + sender + " " + " help";
        sprintf(message, "%s\r\n", mssg.c_str());
        sendResponce(client_fd, message);
    }
    else 
    {
        std::string mssg = "🤖: " + sender + " " + words[3];
        sprintf(message, "%s\r\n", mssg.c_str());
        sendResponce(client_fd, message);
    }
}


int main(int ac, char **av){
    
    if (ac != 3) {
        std::cerr << "Usage: ./Bot <port> <password>" << std::endl;
        return -1;
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