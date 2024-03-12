# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/17 10:32:03 by araiteb           #+#    #+#              #
#    Updated: 2024/03/12 06:28:02 by araiteb          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

EXEC = ircserv
INCLUDE = ./server/Server.hpp ./Clients/Client.hpp ./message/messages.hpp ./server/Channels.hpp ./Exeption/Exception.hpp
SRCS = main.cpp ./Clients/Client.cpp ./server/Server.cpp ./tools/ft_split.cpp ./tools/bot.cpp ./message/messages.cpp ./Exeption/Exception.cpp \
	./tools/command.cpp ./tools/invite.cpp ./tools/topic.cpp ./tools/mode.cpp ./tools/part.cpp ./tools/kick.cpp ./tools/join.cpp \
	./server/Channels.cpp  ./tools/privmsg.cpp ./tools/quit.cpp ./tools/methods.cpp
	
CC = c++ 
CFLAGS = -Wall -Wextra -Werror -std=c++98
OBJS = $(SRCS:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp $(INCLUDE)
	$(CC) $(CFLAGS) -c  $< -o $@

clean:
	rm -f $(OBJS) 

fclean: clean
	rm -f $(EXEC)

re : fclean all
.PHONY: all clean fclean