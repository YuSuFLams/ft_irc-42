NAME = ircserv

C_BLUE  = \033[1;34m
C_CYAN  = \033[1;36m
C_RESET = \033[34m

CC      = c++

RM      = rm -rf

CFLAGS  = -Wall -Wextra -Werror -std=c++98

HEADER  = includes/ft_irc.hpp

O_DIR   := Exec_Obj

EXEC = $(addprefix src/, )

SRCS = $(EXEC) ft_irc.cpp
OBJS = $(patsubst %.cpp,$(O_DIR)/%.o,$(SRCS))

all: $(NAME)

$(NAME): $(OBJS) $(HEADER)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(C_BLUE)		✅ 'ft_irc' Compilation completed successfully!$(C_RESET)"

$(O_DIR)/%.o: %.cpp $(HEADER) $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(O_DIR)
	@echo "$(C_CYAN)🗑️  Object Directory has been removed......$(C_RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(C_CYAN)	🚀 'ft_irc' executable has been removed.....$(C_RESET)"

re: fclean all

.PHONY: all clean fclean re bonus