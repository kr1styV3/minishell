NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LIBFT_DIR = ./my_libft/
LIBFT = $(LIBFT_DIR)libft.a
OBJ_DIR = .obj/
INC_DIR = headers/

# Add the path for readline headers and libraries
READLINE_INC = -I/opt/homebrew/opt/readline/include
READLINE_LIB = -L/opt/homebrew/opt/readline/lib -lreadline -lhistory


# Add a list of your pipex source files here
SRC_FILES = $(addprefix src/,main.c \
	parsing.c \
	t_token_utils.c\
	operators.c\
	executor.c\
	flags.c\
	env_variables.c\
	promt.c\
	read_line.c\
	handler.c\
	get_handler.c)

BUILTIN_FILES = $(addprefix src/builtins/,ft_cd.c \
	ft_echo.c \
	ft_env.c \
	ft_exit.c \
	ft_export.c \
	ft_pwd.c \
	ft_unset.c\
	process_builtin.c)
# Define the corresponding object files and place them in $(OBJ_DIR)
OBJ_FILES = $(SRC_FILES:src/%.c=$(OBJ_DIR)%.o) $(BUILTIN_FILES:src/builtins/%.c=$(OBJ_DIR)%.o)
# Add -I$(INC_DIR) to your flags to include header files from the includes directory
CFLAGS += -I$(INC_DIR) $(READLINE_INC)


NC = \033[0m

all: pre-build $(OBJ_DIR) $(NAME)

pre-build:
	@clear
	@echo "Starting compilation..."

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Pattern rule for object files
$(OBJ_DIR)%.o: src/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $@ successfully!"

$(OBJ_DIR)%.o: src/builtins/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $@ successfully!"

$(NAME): $(LIBFT) $(OBJ_FILES)
	@echo "Creating executable $@..."
	@$(CC) $(OBJ_FILES) -L$(LIBFT_DIR) -lft $(READLINE_LIB) -o $(NAME)
	@clear
	@echo "$$SHELL_ART"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

valgrind: $(NAME)
	@echo "Debugger Mode: Running Valgrind.."
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(NAME)

clean:
	@echo "Cleaning object files..."
	@rm -f $(OBJ_DIR)*.o
	@rm -rf $(OBJ_DIR)
	@rm -f valgrind-out.txt
	@rm -f outfile
	@rm -f out
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@echo "Full clean..."
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@clear

re: fclean all

.PHONY: all clean fclean re

define SHELL_ART

           _.-''|''-._
        .-'     |     `-.
      .'\       |       /`.
    .'   \      |      /   `.
    \     \     |     /     /
     `\    \    |    /    /'
       `\   \   |   /   /'
         `\  \  |  /  /'
        _.-`\ \ | / /'-._
       {_____`\\|//'_____}
               `-'
endef
export SHELL_ART
