/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:01:19 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/23 15:15:16 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"
# include "parsing.h"
# include "executor.h"

typedef struct s_echo_ctx
{
	char		*str;
	int			i;
	int			len;
	t_token		*token;
	char		*output;
	t_env_list	*env;
	char		**envp;
	bool		flag;
}				t_echo_ctx;

int		build_echo_arguments(t_token *token,
			char *output, bool flag, char *str);
/**
 * @brief Extract a substring from the input until
 * a non-alphanumeric character is found.
 *
 * This function scans the input string for alphanumeric characters and spaces,
 * and returns the corresponding substring.
 *
 * @param str Input string.
 * @return char* A newly allocated
 *  string with the extracted characters, or NULL on failure.
 */
char	*extract_until_not_alfanum(char *str);

/**
 * @brief Execute the echo command with optional '-n' flag.
 *
 * This function handles the
 * echo command, including the '-n' flag that suppresses
 * the trailing newline, and processes any quotes or alphanumeric arguments.
 *
 * @param token Pointer to the token structure.
 * @param str Input string containing the command.
 * @param i Position in the input string to start processing.
 * @return int Length of the processed string, or -1 on error.
 */
int		ft_echo(t_token *token, char *str, int i, t_bau_args *meow_bau);
int		handle_plain_text(t_echo_ctx *ctx);
int		handle_env_var(t_echo_ctx *ctx, t_bau_args *bau_norm);
int		handle_single_quote(t_echo_ctx *ctx);
int		append_to_output(char **output,
			char *addition, char *str, t_token *token);
/**
 * @brief Extract the path from a given string, stopping at the first space.
 *
 * @param str Input string.
 * @return char* A newly allocated
 *  string containing the extracted path, or NULL on failure.
 */
char	*extract_path(char *str);

/**
 * @brief Change the current working directory (cd).
 *
 * This function changes the curre
 * nt working directory to the specified path. It also
 * supports tilde expansion by rep
 * lacing '~' with the value of the HOME environment variable.
 *
 * @param token Pointer to the token structure.
 * @param str Input string containing the command.
 * @param i Position in the input string to start processing.
 * @param env Array of environment variables.
 * @return int Length of the processed string, or -1 on error.
 */
int		ft_cd(t_token *token, char *str, int i, t_env_list *env);

/**
 * @brief Print the current working directory (pwd).
 *
 * This function retrieves and prints the current working directory.
 *
 * @return int 0 on success, or -1 if an error occurs.
 */
int		ft_pwd(t_token *token);

/**
 * @brief Display the li
 * st of environment variables (export) or sets a new variable
 *
 * This function prints the current environment variables to the standard output.
 *
 * @param env Array of environment variables.
 * @param str line read
 * @param i line position
 * @return int 0 on success.
 */
int		ft_export(t_token *token, t_env_list **env, char *str, int _i);

/**
 * @brief Unset an environment variable by its name (unset).
 *
 * This function removes an environment variable from the environment.
 *
 * @param token Pointer to the token structure.
 * @param str Input string containing the command.
 * @param i Position in the input string to start processing.
 * @param env Array of environment variables.
 * @return int Length of the processed string, or -1 on error.
 */
int		ft_unset(t_token *token, char *str, int i, t_env_list **env);

/**
 * @brief Display the current environment variables (env).
 *
 * This function prints the current environment variables to the standard output.
 *
 * @param env Array of environment variables.
 * @return int 0 on success.
 */
int		ft_env(t_token *token, t_env_list *env);

/**
 * @brief Exit the shell (exit).
 *
 * This function frees allocated resources and exits the shell.
 *
 * @param token Pointer to the token structure.
 * @param str Input string containing the command.
 * @return int This function does not return.
 */
int		ft_exit(char *str);

/**
 * @brief Process a built-in command and execute the appropriate function.
 *
 * This function checks for built-in c
 * ommands (echo, cd, pwd, export, unset, env, exit)
 * and executes the corresponding function.
 *
 * @param token Pointer to the token structure.
 * @param str Input string containing the command.
 * @param i Position in the input string to start processing.
 * @param state Pointer to the state machine's current state.
 * @param env Array of environment variables.
 * @return int Length of the process
 * ed string, or 0 if no built-in command is found.
 */
int		process_builtin(t_token **token, char *str, int i, t_bau_args *meow);

/**
 * @brief Check if a command is a built-in.
 *
 * This function checks whether the given command is a built-in command.
 *
 * @param cmd Command to check.
 * @return int 1 if the command is built-in, 0 otherwise.
 */
int		ft_isbuiltin(char *cmd);

#endif
