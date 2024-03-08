# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/08 09:43:23 by abel-hid          #+#    #+#              #
#    Updated: 2024/03/08 09:59:10 by abel-hid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = main.cpp\
		Server.cpp\
		join.cpp\
		part.cpp\
		topic.cpp\
		kick.cpp\
		invite.cpp\
		mode.cpp\
		bot.cpp\
		Channels.cpp\

BONUS_SRCS = bot.cpp\

OBJ = $(SRCS:.cpp=.o)
BONUS_OBJ = $(BONUS_SRCS:.cpp=.o)

CPP = c++
RM = rm -f
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 
NAME = ircserv
BONUS = bot
HEAD = Server.hpp Channels.hpp


all: $(NAME)
$(NAME): $(OBJ) $(HEAD)
	$(CPP) $(CPPFLAGS) $(OBJ) -o $(NAME)
bonus: $(BONUS)
$(BONUS): $(BONUS_OBJ) $(HEAD)
	$(CPP) $(CPPFLAGS) $(BONUS_OBJ) -o $(BONUS)

%.o: %.cpp 
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all