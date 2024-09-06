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

typedef struct s_token
{
	char	*value;
	int		fd_overwrite_output;// command > file /*
	int		fd_append_output;// command >> file	  * fd da dove prendere il file per eseguzione
	int		fd_input;// command < file	          /*
	bool	background;// command & se e vero il comando deve essere eseguito in background senno esegui normalmente
	struct s_token *next;
}   t_token;

/**
 * @brief Reads a line from the user
 * @param token The token to be filled with the user input.
 * @return This function does not return; it writes directly to the token.
 */
void	read_line_from_user(t_token *token);

/**
 * @brief Initializes the token structure
 * @return A pointer to the newly allocated token.
 */
t_token	*init_token(void);

# endif
