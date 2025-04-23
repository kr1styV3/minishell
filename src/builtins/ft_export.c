/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 12:31:57 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/15 14:51:07 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/builtins.h"

/*
** export_cleanup:
** Frees the provided variable name and value, writes an error message,
** and returns -1.
*/
static int export_cleanup(char *var_n, char *var_v, char *err)
{
    free(var_n);
    free(var_v);
    write(2, err, ft_strlen(err));
    return (-1);
}

/*
** find_var:
** Iterates through the env list looking for an entry that starts with var_name
** followed immediately by '='.
** Returns a pointer to the matching node if found, or NULL otherwise.
*/
static t_env_list *find_var(const char *var_name, t_env_list *env)
{
    t_env_list *curr;
    size_t len;

    if (!var_name)
        return (NULL);
    len = ft_strlen(var_name);
    curr = env;
    while (curr)
    {
        if (ft_strncmp(curr->value, var_name, len) == 0 && curr->value[len] == '=')
            return (curr);
        curr = curr->next;
    }
    return (NULL);
}

/*
** export_update:
** Builds a new full environment entry (var_name concatenated with var_value).
** If an entry for var_name already exists, it is updated; otherwise, a new node
** is appended to the env list.
**
** Also, if an entry starting with "COLUMNS=" is found, its value is updated by
** incrementing its last digit.
**
** Returns 0 on success or -1 on failure.
*/
static int export_update(t_env_list **env, char *var_n, char *var_v)
{
    char *full_entry;
    t_env_list *existing;
    t_env_list *new_node;
    t_env_list *columns_node;

    full_entry = ft_strjoin(var_n, var_v);
    if (!full_entry)
        return (-1);
    existing = find_var(var_n, *env);
    if (existing)
    {
        free(existing->value);
        existing->value = full_entry;
        return (0);
    }
    new_node = malloc(sizeof(t_env_list));
    if (!new_node)
    {
        free(full_entry);
        return (-1);
    }
    new_node->value = full_entry;
    new_node->next = NULL;
    /* Update COLUMNS variable if found */
    columns_node = *env;
    while (columns_node)
    {
        if (ft_strncmp(columns_node->value, "COLUMNS=", 8) == 0)
        {
            char *columns = ft_strdup(columns_node->value);
            if (!columns)
                return (-1);
            int len = ft_strlen(columns);
            if (columns[len - 1] == '9')
            {
                columns[len - 1] = '0';
                if (len - 2 >= 0)
                    columns[len - 2] += 1;
            }
            else
                columns[len - 1] += 1;
            free(columns_node->value);
            columns_node->value = columns;
            break;
        }
        columns_node = columns_node->next;
    }
    /* Append new_node at the end of the list */
    if (*env == NULL)
        *env = new_node;
    else
    {
        t_env_list *tmp = *env;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_node;
    }
    return (0);
}

static int	export_set_var(t_env_list **env, t_token *token, char *str, int i)
{
	int		len;
	char	*var_name;
	char	*var_value;
	char	*tmp;

	len = skip_whitespaces(&str[i], NULL);
	if (!ft_isalnum(str[i + len]))
		return (-1);
	var_name = extract_token(&str[i + len]);
	if (!var_name)
		return (-1);
	len += ft_strlen(var_name);
	if (str[i + len] == '=')
	{
		tmp = ft_freejoin(var_name, "=");
		var_name = tmp;
		i += len + 1;
		var_value = extract_token(&str[i]);
		if (!var_value)
			return (export_cleanup(var_name, NULL, "malloc error\n"));
		if (export_update(env, var_name, var_value) < 0)
			return (export_cleanup(var_name, var_value,
					"failed to update env variable\n"));
		token->env_work = true;
		len += ft_strlen(var_value) + 1;
		free(var_value);
	}
	free(var_name);
	return (len);
}

static int	export_print_env(t_token *token, t_env_list *env, char *str)
{
	char		*line;

	line = ft_strdup(env->value);
	if (!line)
		return (free_tokens_line(str, token,
				"malloc error for internal process"), -1);
	env = env->next;
	while (env)
	{
		line = ft_freejoin(line, "\n");
		if (!line)
			return (free_tokens_line(str, token,
					"malloc error for internal process"), -1);
		line = ft_freejoin(line, env->value);
		if (!line)
			return (free_tokens_line(str, token,
					"malloc error for internal process"), -1);
		env = env->next;
	}
	line = ft_freejoin(line, "\n");
	if (!line)
		return (free_tokens_line(str, token,
				"malloc error for internal process"), -1);
	token->arg = (char **)ft_calloc(3, sizeof(char *));
	if (!token->arg)
		return (free(line), free_tokens_line(str, token,
				"malloc error for internal process"), -1);
	token->arg[0] = ft_strdup("export");
	if (!token->arg[0])
		return (free(line), free_tokens_line(str, token,
				"malloc error for internal process"), -1);
	token->arg[1] = ft_strdup(line);
	if (!token->arg[1])
		return (free(line), free_tokens_line(str, token,
				"malloc error for internal process"), -1);
	token->arg[2] = NULL;
	token->checker = false;
	free(line);
	return (0);
}


/*
** ft_export:
** Parses the input string starting at _i to extract a variable name and value.
** If an '=' is found, the variable name is appended with "=" and the value is
** extracted. The environment list is then updated (either replacing an existing
** entry or adding a new one). If no variable is provided, the entire environment
** list is printed.
**
** Updates the token's env_ptr and env_work fields accordingly.
**
** Returns the number of characters processed.
*/
int	ft_export(t_token *token, t_env_list **env, char *str, int _i)
{
	int	len;

	len = export_set_var(env, token, str, _i);
	if (len < 0)
	{
		if (export_print_env(token, *env, str) < 0)
			return (-1);
		return (0);
	}
	token->checker = false;
	token->exec = false;
	return (len);
}
