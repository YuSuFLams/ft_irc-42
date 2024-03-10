/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 03:15:46 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/10 08:56:22 by araiteb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

    // :/bot PRIVMSG test : Commands Comming Soon
void 	Bot::traitResvedData(std::string &msg, int client_fd) {
	std::vector<std::string> SplitedMsg;
    char message[1024];
	std::string sender;
    std::string command;
    std::vector<std::string> tmp1;
    std::vector<std::string> tmp2;
    std::cout << msg;
    splitCommand (msg, ':', tmp1);
    command = tmp1[1];
    command = command.substr(0, command.size()-1); 
    splitCommand (tmp1[0], ' ', tmp2);
    sender = tmp2[0];
    tmp1.clear();
    tmp2.clear();
    if (!command.empty() && !command.compare("time")) {
        std::cout << "sdfsdfafadsfasdfahjsdfhkldn   : " << sender << std::endl;
        // if (!command.compare("TIME"))
    //         getTime();
    //     else if (!command.compare(""))
    //         ...
        sprintf(message, "privmsg %s : Commands Comming Soon\r\n", sender.c_str());
        send(client_fd, message, strlen(message), 0);
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