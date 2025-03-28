/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:55:24 by chrlomba          #+#    #+#             */
/*   Updated: 2025/03/21 18:31:55 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_VARIABLES_H
# define ENV_VARIABLES_H

# include "minishell.h"

char    *execute_and_capture_output(char *command, t_env_list *env, char **envp);
int     process_variable(t_token **token, char *str, int string_position, t_env_list *env, char **envp);


/*
 * Extracts a value from `line` starting at index *p_i.
 * Moves *p_i past the extracted portion.
 * Returns a newly allocated string containing the value, or NULL on failure.
 */
char	*extract_value(const char *line, int *p_i);

/*
 * Searches for var_name in env, looking for the pattern "var_name=" as a prefix.
 * Returns the index where it is found, or -1 if not found.
 */
t_env_list *env_find_var(t_env_list *env, const char *var_name);


/* Retrieves the value for the given key from the environment list.
** Returns a newly allocated string containing the value (i.e., the part after '='),
** or an empty string if not found.
*/
char    *ft_getenv(const char *key, t_env_list *env);

/* Replaces an existing environment variable entry.
** 'key' is the variable name and 'full_entry' is the new "KEY=value" string.
** Returns 0 on success or -1 on failure.
*/
int     env_replace_entry(t_env_list *env, const char *key, const char *full_entry);

/* Adds a new environment variable entry to the list.
** 'full_entry' should be in the format "KEY=value".
** Returns 0 on success or -1 on failure.
*/
int     env_add_entry(t_env_list **env, const char *full_entry);

/*
** check_var():
** Processes an input string to extract a variable and update the environment list.
** It uses token information to determine the key and then either adds or replaces
** the entry in the environment list.
*/
int     check_var(t_token **token, char *line, int *i, t_env_list **env);

#endif
