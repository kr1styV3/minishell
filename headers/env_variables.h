/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:55:24 by chrlomba          #+#    #+#             */
/*   Updated: 2024/12/22 21:08:43 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_VARIABLES_H
# define ENV_VARIABLES_H

# include "minishell.h"


int process_variable(t_token **token, char *str, int string_position, char **env);
char *execute_and_capture_output(char *command, char **env);


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
int		env_find_var(char **env, const char *var_name);

/*
 * Replaces an existing env entry at position idx with new_entry.
 * Frees the old string and strdup’s the new one.
 * Returns 0 on success, -1 on failure.
 */
int		env_replace_entry(char **env, int idx, const char *new_entry);

/*
 * Reallocates env to add new_entry, which is duplicated with strdup.
 * Returns the new pointer to env on success, or NULL on failure.
 * The resulting env is always NULL-terminated.
 */
char	**env_add_entry(char **env, const char *new_entry);

/*
 * Given:
 *   - str like "VAR=",
 *   - line containing the new value (possibly quoted),
 *   - index i where the value begins,
 *   - triple-pointer env_ptr so we can reallocate env if needed,
 *   check_var parses out VAR, extracts the value from line,
 *   and either replaces or adds "VAR=value" in *env_ptr.
 * Returns 0 on success, -1 on error.
 */
int		check_var(char *str, char *line, int i, char **env_ptr);


#endif
