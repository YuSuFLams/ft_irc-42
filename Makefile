# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: araiteb <araiteb@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/17 10:32:03 by araiteb           #+#    #+#              #
#    Updated: 2024/03/10 23:46:15 by araiteb          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

EXEC = ircserv
INCLUDE = ./server/server.hpp ./Clients/Client.hpp ./message/messages.hpp ./server/Channels.hpp ./Exeption/Exception.hpp
SRCS = main.cpp ./Clients/Client.cpp ./server/server.cpp ./tools/ft_split.cpp  ./message/messages.cpp ./Exeption/Exception.cpp \
	./tools/command.cpp ./tools/invite.cpp ./tools/topic.cpp ./tools/mode.cpp ./tools/part.cpp ./tools/kick.cpp ./tools/join.cpp \
	./server/Channels.cpp
	
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