SRCS = main.cpp\
		Server.cpp\
		join.cpp\
		part.cpp\
		topic.cpp\
		kick.cpp\
		invite.cpp  


OBJ = $(SRCS:.cpp=.o)
CPP = c++
RM = rm -f
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 
NAME = ircserv
HEAD = Server.hpp


all: $(NAME) clean
$(NAME): $(OBJ) $(HEAD)
	$(CPP) $(CPPFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(HEAD)
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all