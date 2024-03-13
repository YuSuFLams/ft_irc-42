NAME = ircserv
NAME_BONUS = Bot

C_BLUE  = \033[1;34m
C_CYAN  = \033[1;36m
C_RESET = \033[0m

CC      = g++

RM      = rm -rf

CFLAGS  = -Wall -Wextra -Werror -std=c++98

HEADER  = server/server.hpp Clients/Client.hpp message/messages.hpp server/Channels.hpp Exeption/Exception.hpp

O_DIR   := OBJ_DER

B_DIR	:= OBJ_DER_BONUS

TOOLS = $(addprefix tools/, ft_split.cpp command.cpp invite.cpp topic.cpp mode.cpp part.cpp kick.cpp join.cpp privmsg.cpp quit.cpp methods.cpp)

SERVER = $(addprefix server/, server.cpp Channels.cpp)

MESSAGE = $(addprefix message/, messages.cpp)

CLIENTS = $(addprefix Clients/, Client.cpp)

EXEP = $(addprefix Exeption/, Exception.cpp)

M_SRCS = main.cpp $(TOOLS) $(SERVER) $(MESSAGE) $(CLIENTS) $(EXEP)

M_OBJS = $(patsubst %.cpp,$(O_DIR)/%.o,$(M_SRCS))

BONUS = $(addprefix bonus/, Bot.cpp command_bot.cpp main_bonus.cpp)

B_SRCS = $(BONUS)

B_OBJS = $(patsubst %.cpp,$(B_DIR)/%.o,$(B_SRCS))

HEADER_BONUS = bonus/Bot.hpp

all: $(NAME)

$(NAME): $(M_OBJS) $(HEADER)
	@$(CC) $(CFLAGS) -o $(NAME) $(M_OBJS)
	@echo "$(C_BLUE)       ✅ Compilation completed successfully!$(C_RESET)"

$(O_DIR)/%.o: %.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

clean:
	@$(RM) $(O_DIR)
	@echo "$(C_CYAN)🗑️  Object Directory has been removed......$(C_RESET)"

fclean: clean
	@$(RM) $(NAME) $(NAME_BONUS)
	@echo "$(C_CYAN)     🚀 'ircserv' executable has been removed.....$(C_RESET)"

re: fclean all

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(B_OBJS) $(HEADER_BONUS)
	@$(CC) $(CFLAGS) -o $(NAME_BONUS) $(B_OBJS)
	@echo "$(C_BLUE)       ✅ Compilation completed successfully (bonus)!$(C_RESET)"

$(B_DIR)/%.o: %.cpp $(HEADER_BONUS)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

clean_bonus:
	@$(RM) $(B_DIR)
	@echo "$(C_CYAN)🗑️  Object Directory has been removed (bonus)......$(C_RESET)"

fclean_bonus: clean_bonus
	@$(RM) $(NAME_BONUS)
	@echo "$(C_CYAN)     🚀 'Bot' executable has been removed (bonus).....$(C_RESET)"

.PHONY: all clean fclean re bonus clean_bonus fclean_bonus
