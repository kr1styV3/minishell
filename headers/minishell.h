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
	char	operator;
	int		fd_overwrite_output;// command > file /*
	int		fd_append_output;// command >> file	  * fd da dove prendere il file per eseguzione
	int		fd_input;// command < file	          /*
	bool	background;// command & se e vero il comando deve essere eseguito in background senno esegui normalmente
	struct s_token *next;
}   t_token;

typedef enum
{
	NORMAL,         // Default state: reading alphanumeric tokens.
	IN_WORD,        // Inside a quoted string.
	IN_OPERATOR,    // Reading an operator like |, <, >.
	SKIP_WHITESPACE // Skipping spaces.
}	t_state;

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
 * @brief Extracts an alphanumeric token from the string.
 *
 * This function reads from the beginning of the input string and extracts an
 * alphanumeric token, allocating memory for it.
 *
 * @param str The input string to extract the token from.
 * @return A pointer to the extracted token, or NULL if memory allocation fails.
 */
char *extract_token(char *str);

/**
 * @brief Extracts a word from a quoted string.
 *
 * This function extracts a word from a string enclosed by either single ('')
 * or double ("") quotes, stopping at the closing quote.
 *
 * @param str The input string starting after the opening quote.
 * @return A pointer to the extracted word, or NULL if memory allocation fails.
 */
char *extract_word(char *str);

/**
 * @brief Processes an alphanumeric token from the input string.
 *
 * This function processes an alphanumeric token, extracting it from the input
 * string and storing it in the provided `t_token` structure.
 *
 * @param token A pointer to the `t_token` structure where the token will be stored.
 * @param str The input string to extract the token from.
 * @return The number of characters processed or -1 if memory allocation fails.
 */
int process_token(t_token *token, char *str);

/**
 * @brief Processes a quoted word from the input string.
 *
 * This function processes a word enclosed in single or double quotes, extracting
 * it and storing it in the provided `t_token` structure.
 *
 * @param token A pointer to the `t_token` structure where the word will be stored.
 * @param str The input string starting with a quote.
 * @return The number of characters processed or -1 if memory allocation fails.
 */
int process_word(t_token *token, char *str);

/**
 * @brief Processes an operator character from the input string.
 *
 * This function processes operator characters such as |, <, >, and stores them
 * in the provided `t_token` structure.
 *
 * @param token A pointer to the `t_token` structure where the operator will be stored.
 * @param str The input string containing the operator.
 * @return The number of characters processed (e.g., 2 for ">>" or 1 for other operators).
 */
int process_operator(t_token *token, char *str);

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
