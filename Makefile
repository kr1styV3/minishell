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
	tokinize_operators.c\
	executor.c\
	flags.c\
	env_variables.c\
	promt.c\
	read_line.c\
	env.c\
	loop.c\
	signals.c\
	tokenize.c\
	tokenize_handle_state.c\
	here_doc.c\
	here_doc_helpers.c\
	tokenize_arg_check_helpers.c\
	tokenize_arg_check.c\
	tokenize_arg_len_check.c\
	readline_check_syntax.c\
	util_extracting.c\
	executor_utils.c\
	executor_redirections.c\
	executor_pipeline.c\
	here_doc_init.c\
	env_exec_capture.c\
	env_process_variable.c\
	token_init.c\
	token_init_utils.c\
	token_free.c\
	util_exctracting.c\
	tokenize_checkvar.c\
	tokenize_checkvar_util.c\
	util_exctract_files.c)

BUILTIN_FILES = $(addprefix src/builtins/,ft_cd.c \
	ft_echo.c \
	ft_env.c \
	ft_exit.c \
	ft_export.c \
	ft_pwd.c \
	ft_unset.c\
	process_builtin.c\
	echo_utils.c)
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
	@valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --error-limit=no --log-file=valgrind-out.txt ./$(NAME)

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
