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
	char	*token;
	char	*word;
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

// parsing.c
/**
 * @brief Skips the whitespaces in the given string.
 * @param str The string from which the whitespaces are skipped.
 * @return The number of characters skipped.
 */
int	skip_whitespaces(char *str);
/**
 * @brief Extracts a word from the given string, stopping at either a double quote (") or a single quote (').
 * @param str The string from which the word is extracted.
 * @return A newly allocated string containing the word, or NULL if memory allocation fails.
 */
char	*load_word(char *str);
/**
 * @brief Extracts a token from the given string consisting of alphanumeric characters.
 * @param str The string from which the token is extracted.
 * @return A newly allocated string containing the token, or NULL if memory allocation fails.
 */
char	*load_token(char *str);
/**
 * @brief Parses the string and fills the token structure.
 * @param str The string to be parsed.
 * @param token The token to be filled.
 * @return The number of characters read.
 */
int	understand_load_token(char *str, t_token *token);
/**
 * @brief Parses the string and fills the token structure.
 * @param str The string to be parsed.
 * @param token The token to be filled.
 * @return This function does not return; it writes directly to the token.
 */
void	tokenizer(char *str, t_token *token);

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

# endif
