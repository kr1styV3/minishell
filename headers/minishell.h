#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "../my_libft/headers/libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <signal.h>

typedef struct s_token
{
	char	*token;
	char	*word;
	char	operator;
	int		fd_overwrite_output;// command > file /*
	int		fd_append_output;// command >> file	  * fd da dove prendere il file per eseguzione
	int		fd_input;// command < file	          /*
	char	*here_doc;
	bool 	next_is_pipe;// command | command
	bool	background;// command & se e vero il comando deve essere eseguito in background senno esegui normalmente
	struct s_token *next;
}   t_token;

typedef enum
{
	NORMAL,         // Default state: reading alphanumeric tokens.
	IN_BUILTIN,     // Inside a builtin command.
	IN_WORD,        // Inside a quoted string.
	IN_OPERATOR,    // Reading an operator like |, <, >.
	SKIP_WHITESPACE // Skipping spaces.
}	t_state;


// t_token_utils.c
/**
 * @brief Initializes the token structure
 * @return A pointer to the newly allocated token.
 */
t_token	*init_token(void);
/**
 * @brief Frees the memory allocated for the tokens and prints an error message.
 * @param str The string to be freed.
 * @param token The token to be freed.
 * @param error_message The error message to be printed.
 * @return This function does not return; it frees the memory and exits the program.
 */
void	free_tokens_line(char *str, t_token *token, char *error_message);
/**
 * @brief Prints the tokens to the standard output.
 * @param token The token to be printed.
 * @return This function does not return; it writes directly to the standard output.
 */
void	print_tokens(t_token *token);
/**
 * @brief Frees the memory allocated for the tokens.
 * @param token The token to be freed.
 * @return This function does not return; it frees the memory.
 */
void	free_token(t_token *token);

# endif
