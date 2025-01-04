/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:36:42 by chrlomba          #+#    #+#             */
/*   Updated: 2024/12/10 15:04:12 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include "../my_libft/headers/libft.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <signal.h>
# include "t_token.h"

extern volatile sig_atomic_t should_exit;

// t_token_utils.c
void inside_token_free(t_token *token);
t_token	*reinit_token(t_token *prev_token);
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
 * @brief Frees the memory allocated for the tokens.
 * @param token The token to be freed.
 * @return This function does not return; it frees the memory.
 */
void	free_token(t_token *token);
/**
 * @brief Moves the given token pointer to the head of the token list.
 *
 * This function traverses the token list backwards using the `prev` pointer
 * until it reaches the first token (where `prev` is `NULL`).
 *
 * @param token Pointer to the current token in the list. It will traverse
 * backwards until it reaches the head of the list.
 */
void return_to_head(t_token *token);
/**
 * @brief Frees the memory allocated for the given token and prints error messages.
 *
 * This function releases all dynamically allocated memory inside a token structure,
 * including the token string, word, here-doc string, and arguments array. It also
 * prints an error message composed of the provided `msg` and `cmd` to standard error.
 * After freeing the memory of the entire token list, it reinitializes the token by
 * calling `init_token()`.
 *
 * @param token Pointer to the token to be freed. This function will free
 * all tokens in the list starting from this one.
 * @param msg The error message to print before freeing the tokens.
 * @param cmd The command related to the error, to be printed after `msg`.
 */
void free_inside_token(t_token *token, char *msg, char *cmd);

# endif
