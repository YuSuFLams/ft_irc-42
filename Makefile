SRCS = main.cpp\
		Server.cpp\
		join.cpp\
		part.cpp\
		topic.cpp\
		kick.cpp\
		invite.cpp\
		mode.cpp\

BONUS_SRCS = bot.cpp\

OBJ = $(SRCS:.cpp=.o)
BONUS_OBJ = $(BONUS_SRCS:.cpp=.o)

CPP = c++
RM = rm -f
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 
NAME = ircserv
BONUS = bot
HEAD = Server.hpp


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