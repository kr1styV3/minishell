/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:36:42 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/10 16:27:25 by chrlomba         ###   ########.fr       */
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
# include "executor.h"
# include "state.h"

extern volatile sig_atomic_t	g_should_exit;

# define OPERATORS "<>|"

// t_token_utils.c
char		*extract_word_with_dollasign(char *str, char quote,
				t_token *token, t_bau_args *cazzo_miao);
int			process_file_cmd(t_token *token, char *str, int i);
int			get_dolpos(char *line);
char		*extract_until_not_alfanum(char *str);
void		update_state_from_char(t_tokenizer_ctx *ctx);
int			var_cleanup(char *full_entry, char *var_value, char *str);
int			skip_assignment_whitespace(const char *line, int i);
char		*extract_word(char *str, char quote);
char		*extract_token(char *str);
int			here_doc_init(t_token *token, char *str, int i);
int			count_arguments(const char *str, int start);
int			check_syntax(char *line);
int			fill_quoted_argument(t_token *token,
				const char *str, int pos, int arg_i);
int			heredoc_expansion(t_token **token, char *line,
				t_env_list *env_list, char **envp);
int			get_next_arg_len(const char *str, int *pos);
int			fill_unquoted_argument(t_token *token,
				const char *str, int pos, int arg_i);
int			ft_checkwordarg(t_token **token, char *str, int i);
char		*extract_bash_file(char *str);
void		inside_token_free(t_token *token);
void		setup_signal_handling(void);
void		shell_loop(t_token **head,
				t_env_list *_env_ptr, char **envp, int *status);
void		free_list_copy(t_env_list *ptr);
int			checker(t_token **token, t_env_list *envp);
t_token		*reinit_token(t_token *prev_token);
/**
 * @brief Initializes the token structure
 * @return A pointer to the newly allocated token.
 */
t_token		*init_token(void);
/**
 * @brief Frees the memory allocated for the tokens and prints an error message.
 * @param str The string to be freed.
 * @param token The token to be freed.
 * @param error_message The error message to be printed.
 * @return This function does not return;
 * it frees the memory and exits the program.
 */
void		free_tokens_line(char *str, t_token *token, char *error_message);
/**
 * @brief Frees the memory allocated for the tokens.
 * @param token The token to be freed.
 * @retu	rn This function does not return; it frees the memory.
 */
void		free_token(t_token *token);
/**
 * @brief Moves the given token pointer to the head of the token list.
 *
 * This function traverses the token list backwards using the `prev` pointer
 * until it reaches the first token (where `prev` is `NULL`).
 *
 * @param token Pointer to the current token in the list. It will traverse
 * backw	ards until it reaches the head of the list.
 */
void		return_to_head(t_token *token);
/**
 * @brief Frees the memory allocated for
 * the given token and prints error messages.
 *
 * This function releases all dynamical
 * ly allocated memory inside a token structure,
 * including the token string, word, he
 * re-doc string, and arguments array. It also
 * prints an error message composed of
 * the provided `msg` and `cmd` to standard error.
 * After freeing the memory of the ent
 * ire token list, it reinitializes the token by
 * calling `init_token()`.
 *
 *
 * @param token Pointer to the token to be freed. This function will free
 * all tokens in the list starting from this one.
 * @param msg The error message to print before freeing the tokens.
 * @param cmd The command related to the error, to be printed after `msg`.
 */
void		free_inside_token( char *msg, char *cmd);
t_env_list	*env_copy(char **envp);

#endif
