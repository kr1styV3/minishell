/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_checkvar_util.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 20:41:41 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/07 20:46:57 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/env_variables.h"
#include "minishell.h"

t_env_list	*env_find_var(t_env_list *env, const char *var_name)
{
	t_env_list	*current;
	size_t		key_len;

	if (!env || !var_name)
		return (NULL);
	key_len = strlen(var_name);
	current = env;
	while (current)
	{
		if (ft_strncmp(current->value, var_name, key_len) == 0
			&& current->value[key_len] == '=')
		{
			return (current);
		}
		current = current->next;
	}
	return (NULL);
}

static int	extract_quoted_value(const char *line, int i, int *end)
{
	i++;
	*end = i;
	while (line[*end] && line[*end] != '"')
		(*end)++;
	if (line[*end] == '"')
		(*end)++;
	return (i);
}

static int	extract_unquoted_value(const char *line, int i, int *end)
{
	*end = i;
	while (line[*end] && line[*end] != ' ' && line[*end] != '\t')
		(*end)++;
	return (i);
}

static char	*copy_value_substr(const char *line, int start, int end)
{
	int		len;
	char	*val;

	len = end - start;
	val = malloc(len + 1);
	if (!val)
		return (NULL);
	ft_memcpy(val, &line[start], len);
	val[len] = '\0';
	return (val);
}

char	*extract_value(const char *line, int *p_i)
{
	int		i;
	int		start;
	int		end;
	char	*val;

	i = skip_assignment_whitespace(line, *p_i);
	if (i == -1)
		return (NULL);
	if (line[i] == '"')
		start = extract_quoted_value(line, i, &end);
	else
		start = extract_unquoted_value(line, i, &end);
	val = copy_value_substr(line, start, end);
	if (!val)
		return (NULL);
	*p_i = end;
	return (val);
}
